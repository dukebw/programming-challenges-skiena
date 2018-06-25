/**
 * https://open.kattis.com/problems/pebblesolitaire
 */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * count_min_pebbles(): Recursively count the minimum number of pebbles
 * possible by making game moves.
 * @pebbles: The pebbles array representing the game.
 *
 * Recursive strategy:
 *
 * Base case: if no two pebbles are adjacent, return the number of pebbles.
 *
 * Recursive step: return the minimum pebbles from making each possible move.
 */
template<int32_t N>
static uint32_t
count_min_pebbles(bool (&pebbles)[N])
{
        /* For the base case, check pebbles one by one for adjacent pebbles. */
        bool are_any_adjacent = false;
        uint32_t num_pebbles = 0;
        for (uint32_t i = 0;
             i < N;
             ++i) {
                if (pebbles[i]) {
                        if ((i < (N - 1)) && pebbles[i + 1]) {
                                are_any_adjacent = true;
                                break;
                        }

                        ++num_pebbles;
                }
        }

        /* Base case. */
        if (!are_any_adjacent)
                return num_pebbles;

        /**
         * Recursive step.
         *
         * For each pebble, for each (left and/or right) move possible, count
         * minimum pebbles for the board resulting from taking the move.
         *
         * Keep track of the globally minimum pebbles.
         */
        uint32_t min_pebbles = N;
        for (uint32_t i = 0;
             i < N;
             ++i) {
                if (!pebbles[i])
                        continue;

                uint32_t next_min_pebbles;
                if ((i < (N - 2)) && pebbles[i + 1] && !pebbles[i + 2]) {
                        pebbles[i] = false;
                        pebbles[i + 1] = false;
                        pebbles[i + 2] = true;

                        next_min_pebbles = count_min_pebbles(pebbles);

                        pebbles[i] = true;
                        pebbles[i + 1] = true;
                        pebbles[i + 2] = false;

                        if (next_min_pebbles < min_pebbles)
                                min_pebbles = next_min_pebbles;
                }

                if ((i >= 2) && pebbles[i - 1] && !pebbles[i - 2]) {
                        pebbles[i] = false;
                        pebbles[i - 1] = false;
                        pebbles[i - 2] = true;

                        next_min_pebbles = count_min_pebbles(pebbles);

                        pebbles[i] = true;
                        pebbles[i - 1] = true;
                        pebbles[i - 2] = false;

                        if (next_min_pebbles < min_pebbles)
                                min_pebbles = next_min_pebbles;
                }
        }

        return min_pebbles;
}

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
        /**
         * Game boards are read from stdin into read_buf, then parsed into
         * pebbles, which represents a pebble with true and a cavity with
         * false.
         */
        constexpr int32_t NUM_PEBBLES = 12;
        constexpr int32_t READ_BUF_MAX = 16;
        bool pebbles[NUM_PEBBLES];
        char read_buf[READ_BUF_MAX];

        /**
         * The first line indicates the number of games to follow, where each
         * game is a line of 12 '-' or 'o' characters followed by a newline.
         */
        assert(fgets(read_buf, READ_BUF_MAX, stdin) != NULL);

        int32_t num_games = atoi(read_buf);
        assert((num_games > 0) && (num_games <= 10));

        for (int32_t game_i = 0;
             game_i < num_games;
             ++game_i) {
                assert(fgets(read_buf, READ_BUF_MAX, stdin) != NULL);

                /* Parse this game into pebbles. */
                assert(strlen(read_buf) == (NUM_PEBBLES + 1));
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

                uint32_t min_pebbles = count_min_pebbles(pebbles);
                printf("%u\n", min_pebbles);
        }

        return EXIT_SUCCESS;
}
