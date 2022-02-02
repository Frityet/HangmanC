//
// Created by Frityet on 2022-01-24.
//

#ifndef HANGMANC_GAME_
#define HANGMANC_GAME_

#include <stddef.h>
#include <stdlib.h>

#include <common.h>
#include <dictionary.h>

struct game {
    unsigned char   *word, *hidden_word, *guessed_chars, *correct_chars;
    size_t          wordlen;
    size_t          guess_count, guesses_remaining;
    bool            default_word;
    enum {
       GAMESTATE_WON,
       GAMESTATE_NONE,
       GAMESTATE_LOST
    } gamestate;
};


struct game new_game(size_t trycount);

bool guess_character(struct game *game, unsigned char c);

static inline void cleanup_game(struct game game)
{
    free(game.hidden_word);
    free(game.guessed_chars);
    if (!game.default_word)
        free(game.word);
}

#endif //HANGMANC_GAME_
