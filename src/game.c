//
// Created by Frityet on 2022-01-24.
//

#include "game.h"

#include <curl/curl.h>
#include <string.h>
#include <errno.h>
extern int errno;

#include <generic_list.h>

#define API_URL "https://random-word-api.herokuapp.com/word?number=1"

struct memory {
    uint8_t *data;
    size_t  size;
};

static size_t writemem(void *contents, size_t size, size_t bytecount, void *ptr) {
    size_t total = size * bytecount;
    struct memory *memory = (struct memory *)ptr;

    uint8_t *newmem = realloc(memory->data, memory->size + total + 1);
    if (newmem == NULL) {
        fprintf(stderr, "Could not allocate memory!\nReason: %s", strerror(errno));
    }

    memory->data = newmem;
    memcpy(&(memory->data[memory->size]), contents, total);
    memory->size += total;
    memory->data[memory->size] = 0;

    return total;
}

static unsigned char *get_random_word(char *defaultmsg)
{
    puts("Getting random word");
    CURL *curl = curl_easy_init();

    if (curl == NULL) {
        return defaultmsg;
    }

    struct memory mem = {0};
    mem.data = malloc(1);

    curl_easy_setopt(curl, CURLOPT_URL, API_URL);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writemem);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &mem);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    puts("Sending request to " API_URL);
    if (curl_easy_perform(curl) != CURLE_OK) {
        return defaultmsg;
    }
    curl_free(curl);
    puts("Done!");


    unsigned char *retstr = calloc(mem.size - 3, sizeof(char));
    for (int i = 2; i < mem.size - 2; ++i) {
        retstr[i - 2] = mem.data[i];
    }
    free(mem.data);
    return retstr;
}

struct game new_game(size_t trycount)
{
    struct game game;

    game.word               = get_random_word("hangman");
    game.wordlen            = strlen(game.word);
    game.hidden_word        = malloc(sizeof(char) * game.wordlen);
    memset(game.hidden_word, '_', game.wordlen);
    game.guessed_chars      = LIST(char);
    game.correct_chars      = LIST(char);
    game.guess_count        = trycount;
    game.guesses_remaining  = game.guess_count;
    game.gamestate          = GAMESTATE_NONE;

    return game;
}

bool guess_character(struct game *game, unsigned char c)
{
    bool correct = false;

    for (int i = 0; i < list_length(game->guessed_chars); ++i) {
        if (game->guessed_chars[i] == c)
            return false;
    }

    for (int i = 0; i < list_length(game->correct_chars); ++i) {
        if (game->guessed_chars[i] == c)
            return false;
    }

    for (int i = 0; i < game->wordlen; ++i) {
        if (game->word[i] == c) {
            game->hidden_word[i] = c;

            if (!correct)
                ADD_ITEM(game->correct_chars, c);

            correct = true;
        }
    }

    if (!correct) {
        game->guesses_remaining--;
        ADD_ITEM(game->guessed_chars, c);
    }

    return correct;
}
