#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * The board starts with every cavity filled (with a pebble), except for one.
 *
 * Moves are made by jumping a pebble A over an adjacent pebble B into an empty
 * cavity, and removing B.
 *
 * Given an arbitrary 1x12 board, find a sequence of moves such that as few
 * pebbles as possible are left.
 */
int main(void)
{
        constexpr int32_t NUM_PEBBLES = 12;
        constexpr int32_t READ_BUF_MAX = 16;
        bool pebbles[NUM_PEBBLES];  /* true iff a cavity has a pebble */
        char read_buf[READ_BUF_MAX];

        FILE *in_handle = fopen("pebblesolitaire.in", "r");
        assert(in_handle != NULL);

        assert(fgets(read_buf, READ_BUF_MAX, in_handle) != NULL);

        int32_t num_games = atoi(read_buf);
        assert((num_games > 0) && (num_games <= 10));

        for (int32_t game_i = 0;
             game_i < num_games;
             ++game_i) {
                assert(fgets(read_buf, READ_BUF_MAX, in_handle) != NULL);

                assert(strlen(read_buf) == NUM_PEBBLES);
                for (int32_t pebble_i = 0;
                     pebble_i < NUM_PEBBLES;
                     ++pebble_i) {
                        switch (read_buf[pebble_i]) {
                        case '-':
                                pebbles[pebble_i] = false;
                                break;
                        case 'o':
                                pebbles[pebble_i] = true;
                                break;
                        default:
                                assert(false);
                        }
                }

                /**
                 * Recursive strategy:
                 *
                 * Base case: if no two pebbles are adjacent, return the number
                 * of pebbles.
                 *
                 * Recursive step: return the minimum pebbles from making each
                 * possible move.
                 */
        }

        return EXIT_SUCCESS;
}
