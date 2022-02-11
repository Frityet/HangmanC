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
    char            *word, *hidden_word, *guessed_chars, *correct_chars;
    size_t          wordlen;
    size_t          guess_count, guesses_remaining;
    bool            default_word;
    enum {
       GAMESTATE_WON,
       GAMESTATE_NONE,
       GAMESTATE_LOST
    } gamestate;
};


struct game newgame(size_t trycount);

bool guesscharacter(struct game *game, char c);

static inline void cleanupgame(struct game game)
{
    free(game.hidden_word);
    free_list(game.guessed_chars), free_list(game.correct_chars);
    if (!game.default_word)
        free(game.word);
}

bool checkwin(struct game *game);

#endif //HANGMANC_GAME_
