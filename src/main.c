#include <stdio.h>
#include <string.h>
#include <errno.h>
extern int errno;

#include <ansi.h>

#include "game.h"

void topbar(struct game game)
{
    ESC_SEQ(1;32m);
    PRINT_MSG_MIDDLE(1, "Hangman!", "f");
    PRINT_MSG_MIDDLE(3, "You have %zu/%zu guesses left!", game.guesses_remaining, game.guess_count);
}

void bottombar()
{
    ESC_SEQ(1;31m);
    PRINT_MSG_MIDDLE(25, "Enter guess!\n", "f");
    setcursorposition(26, 49);
}

void mainpanel(struct game game)
{
    ESC_SEQ(1;33m);
    setcursorposition(15, 50 - (game.wordlen / 2));
    puts(game.hidden_word);
    ESC_SEQ(1;0m);
}

void rendergame(struct game game)
{
    clearscreen();

    if (game.gamestate == GAMESTATE_WON) {
        ESC_SEQ(1;32m);
        PRINT_MSG_MIDDLE(1, "You won!", "f");
        PRINT_MSG_MIDDLE(2, "The word was: %s", game.word);
        PRINT_MSG_MIDDLE(3, "Congratulations!", "f");
    } else if (game.gamestate == GAMESTATE_LOST) {
        ESC_SEQ(1;31m);
        PRINT_MSG_MIDDLE(1, "You lost!", "f");
        PRINT_MSG_MIDDLE(2, "The word was: %s", game.word);
        PRINT_MSG_MIDDLE(3, "Try again!", "f");
    } else {
        topbar(game);
        mainpanel(game);
        bottombar();
    }
}

int main(int argc, char *argv[])
{
    long trycount;
    if (argc < 2) {
        trycount = 8;
    } else {
        char *end = NULL;
        trycount = strtol(argv[1], &end, 10);
        if (end == NULL) {
            fprintf(stderr, "Invalid input for trycount reverting to default (8)\n");
            trycount = 8;
        }
    }

    struct game game = newgame(trycount);

    rendergame(game);

    while (game.gamestate == GAMESTATE_NONE) {
        guesscharacter(&game, (char) ({
            int i = getchar();
            i > 127 ? 'a' : i;
        }));
        checkwin(&game);
        rendergame(game);
    }

    rendergame(game);

    cleanupgame(game);
}
