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

/**
 * Some shit required for cURL
 *
 * I just copied this
**/

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
    memory->data[memory->size] = '\0';

    return total;
}

static char *get_random_word(char *defaultmsg)
{
    //This should probably be on another thread
    //But that takes too much work
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

    //This wacky shit right here removes the json parts of the string returned by the API
    //There is 4 bytes of which we need to shave off, but we subtract 3 as one is used for \0
    //Afterwards we just copy the inside word, which is the whole string starting at index 2,
    //and ending at the length - 2
    char *retstr = calloc(mem.size - 3, sizeof(char));
    for (int i = 2; i < mem.size - 2; ++i) {
        retstr[i - 2] = mem.data[i];
    }
    free(mem.data);
    return retstr;
}

struct game newgame(size_t trycount)
{
    struct game game;

    game.word               = get_random_word("hangman");
    game.wordlen            = strlen(game.word);
    game.hidden_word        = malloc(sizeof(char) * game.wordlen);
    memset(game.hidden_word, '_', game.wordlen);
    //memset doesn't copy null char or smth like that
    game.hidden_word[game.wordlen + 1] = '\0';

    //List macro defined in src/list.h
    //this piece of shit segfaults all the time
    //wouldn't recommend!

    //I should also be dynamically adding more memory to the list as needed
    //but that segfaults all the time, so fuck that!
    // We are gonna alloc a fuck ton of memory and let it go

    //I am well aware how shitty this practice is
    game.guessed_chars      = create_list(sizeof(char), 1024);
    game.correct_chars      = create_list(sizeof(char), 1024);
    game.guess_count        = trycount;
    game.guesses_remaining  = game.guess_count;
    game.gamestate          = GAMESTATE_NONE;

    return game;
}

bool guesscharacter(struct game *game, char c)
{
    bool correct = false;

    for (int i = 0; i < list_length(game->guessed_chars); ++i) {
        if (game->guessed_chars[i] == c)
            return false;
    }

    for (int i = 0; i < game->wordlen; ++i) {
        if (game->word[i] == c) {
            game->hidden_word[i] = c;

            //Fun fact, the lack of {} here before caused a segfault
            //how? The macro had a multi-line expansion, making most
            //of the code outside the if statement, causing it to break
            //adding ({}) to the macro fixed it, but that's a compiler
            //extension, which is bad I guess (who the fuck uses anything
            //other than GCC/Clang anyway?)
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

bool checkwin(struct game *game)
{
    //I know this is slow
    //I also don't care
    if (strncmp(game->hidden_word, game->word, game->wordlen) == 0) {
        game->gamestate = GAMESTATE_WON;
        return true;
    } else {
        if (game->guesses_remaining == 0)
            game->gamestate = GAMESTATE_LOST;
        return false;
    }
}
