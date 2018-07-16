/**
 * Prva - https://open.kattis.com/problems/prva
 *
 * In an RxC crossword, where each element is a lowercase English alphabet
 * character or a # indicating a blocked element, find the lexicographically
 * smallest word with 2 or more letters.
 *
 * The input is such that a solution always exists.
 */
#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * copy_if_smaller() - Copy current word if length >= 2, and smaller than
 * smallest word.
 */
template<size_t W>
static void
copy_if_smaller(char (&smallest_word)[W], char (&current_word)[W])
{
        if (strlen(current_word) < 2)
                return;

        if (strcmp(current_word, smallest_word) < 0)
                strncpy(smallest_word, current_word, W);
}

/**
 * copy_next_letter() - Copy next letter unless '#'.
 */
static bool
copy_next_letter(char *current_word,
                 char *crossword,
                 int32_t word_i,
                 int32_t row_i,
                 int32_t col_i,
                 int32_t C)
{
        char next_letter = crossword[row_i*C + col_i];
        if (next_letter == '#')
                return true;

        assert(islower(next_letter));
        current_word[word_i] = next_letter;

        return false;
}

int main(void)
{
        int32_t R;
        int32_t C;
        char readbuffer[32];
        constexpr int32_t MAX_DIM = 20;
        char crossword[MAX_DIM*MAX_DIM];

        assert(fgets(readbuffer, sizeof(readbuffer), stdin) != NULL);
        assert(sscanf(readbuffer, "%d %d", &R, &C) == 2);
        assert((R >= 2) && (R <= MAX_DIM));
        assert((C > 0) && (C <= MAX_DIM));

        for (int32_t row_i = 0;
             row_i < R;
             ++row_i) {
                assert(fgets(readbuffer, sizeof(readbuffer), stdin) != NULL);

                for (int32_t col_i = 0;
                     col_i < C;
                     ++col_i) {
                        crossword[row_i*C + col_i] = readbuffer[col_i];
                }
        }

        /* Search rows, then columns, for the smallest word. */
        char smallest_word[MAX_DIM + 1];
        memset(smallest_word, 'z' + 1, MAX_DIM);
        smallest_word[MAX_DIM] = '\0';

        char current_word[sizeof(smallest_word)];
        for (int32_t row_i = 0;
             row_i < R;
             ++row_i) {
                for (int32_t col_i = 0;
                     col_i < C;
                     ) {
                        memset(current_word, 0, sizeof(current_word));

                        int32_t start_i = col_i;
                        while (col_i < C) {
                                bool blocked = copy_next_letter(current_word,
                                                                crossword,
                                                                col_i - start_i,
                                                                row_i,
                                                                col_i,
                                                                C);
                                ++col_i;

                                if (blocked)
                                        break;
                        }

                        copy_if_smaller(smallest_word, current_word);
                }
        }

        for (int32_t col_i = 0;
             col_i < C;
             ++col_i) {
                for (int32_t row_i = 0;
                     row_i < R;
                     ) {
                        memset(current_word, 0, sizeof(current_word));

                        int32_t start_i = row_i;
                        while (row_i < R) {
                                bool blocked = copy_next_letter(current_word,
                                                                crossword,
                                                                row_i - start_i,
                                                                row_i,
                                                                col_i,
                                                                C);
                                ++row_i;

                                if (blocked)
                                        break;
                        }

                        copy_if_smaller(smallest_word, current_word);
                }
        }

        printf("%s\n", smallest_word);

        return EXIT_SUCCESS;
}
