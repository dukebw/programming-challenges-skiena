/**
 * Anagram counting - https://open.kattis.com/problems/anagramcounting
 *
 * Given up to 200 lines of input, each containing one word of up to 100
 * upper and lowercase letters, find the number of anagrams for each word.
 */
#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>

template<typename T, size_t N>
constexpr size_t
array_len(T (&)[N])
{
        return N;
}

constexpr uint32_t PRIMES[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
constexpr size_t NUM_PRIMES = array_len(PRIMES);

/**
 * count_primes() - Record in primes_counts the prime factors in N! :=
 * num_factorial.
 */
template<size_t N>
static void
count_primes(uint32_t (&primes_counts)[N], uint32_t num_factorial)
{
        static_assert(N == NUM_PRIMES, "prime count array must match primes");

        for (uint32_t factor = num_factorial;
             factor > 1;
             --factor) {
                uint32_t remaining_factor = factor;
                for (uint32_t prime_i = 0;
                     prime_i < NUM_PRIMES;
                     ++prime_i) {
                        while ((remaining_factor % PRIMES[prime_i]) == 0) {
                                remaining_factor /= PRIMES[prime_i];
                                ++primes_counts[prime_i];
                        }
                }
                assert(remaining_factor == 1);
        }
}

/**
 * get_char_index() - Return an index such that the array format is
 * ABC...Zabc...z.
 */
static uint32_t
get_char_index(char next_char)
{
        assert(isalpha(next_char));

        if (isupper(next_char))
                return next_char - 'A';

        return 26 + (next_char - 'a');
}

int main(void)
{
        /**
         * For a word of length N, the number of anagrams is N! divided by the
         * product of the counts of each unique character in the word.
         */
        constexpr uint32_t MAX_WORD_LEN = 100;
        constexpr uint32_t MAX_NUM_WORDS = 200;

        /**
         * primes_counts represents a number by the occurrences of prime
         * numbers in its factorized representation.
         *
         * Since we represent a number up to 100!, all numbers in the
         * factorized representation contain primes <= 100.
         */
        for (uint32_t line_num = 0;
             line_num < MAX_NUM_WORDS;
             ++line_num) {
                uint32_t primes_counts[NUM_PRIMES];
                memset(primes_counts, 0, sizeof(primes_counts));

                uint32_t char_repeats[52];
                memset(char_repeats, 0, sizeof(char_repeats));

                char read_buffer[MAX_WORD_LEN + 2];
                if (fgets(read_buffer, sizeof(read_buffer), stdin) == NULL)
                        break;

                uint32_t word_len = strlen(read_buffer) - 1;
                for (uint32_t buf_i = 0;
                     buf_i < word_len;
                     ++buf_i) {
                        char next_char = read_buffer[buf_i];

                        uint32_t char_i = get_char_index(next_char);
                        ++char_repeats[char_i];
                }

                count_primes(primes_counts, word_len);

                for (uint32_t char_i = 0;
                     char_i < array_len(char_repeats);
                     ++char_i) {
                        if (char_repeats[char_i] <= 1)
                                continue;

                        uint32_t div_primes_counts[NUM_PRIMES];
                        memset(div_primes_counts,
                               0,
                               sizeof(div_primes_counts));

                        count_primes(div_primes_counts, char_repeats[char_i]);

                        for (uint32_t prime_i = 0;
                             prime_i < NUM_PRIMES;
                             ++prime_i) {
                                assert(primes_counts[prime_i] >=
                                       div_primes_counts[prime_i]);

                                primes_counts[prime_i] -= div_primes_counts[prime_i];
                        }
                }

                /**
                 * 2**32 - 1 has 10 digits, save numbers % 10**9 (use a power
                 * of 10 since this is what we want to print.)
                 */
                std::vector<uint64_t> base_muls{1};
                for (uint32_t prime_i = 0;
                     prime_i < NUM_PRIMES;
                     ++prime_i) {
                        for (uint32_t count_i = 0;
                             count_i < primes_counts[prime_i];
                             ++count_i) {
                                for (int32_t base_i = base_muls.size() - 1;
                                     base_i >= 0;
                                     --base_i) {
                                        uint64_t might_overflow = base_muls[base_i]*PRIMES[prime_i];
                                        if (might_overflow >= 1'000'000'000) {
                                                base_muls[base_i] = might_overflow % 1'000'000'000;
                                                uint64_t carry = might_overflow / 1'000'000'000;
                                                uint32_t carry_i = base_i + 1;
                                                while (carry > 0) {
                                                        if (carry_i >= base_muls.size()) {
                                                                base_muls.push_back(carry);
                                                                break;
                                                        }

                                                        might_overflow = base_muls[carry_i] + carry;
                                                        base_muls[carry_i] = might_overflow % 1'000'000'000;
                                                        carry = might_overflow / 1'000'000'000;
                                                        ++carry_i;
                                                }
                                        } else {
                                                base_muls[base_i] = might_overflow;
                                        }
                                }
                        }
                }

                printf("%lu", base_muls[base_muls.size() - 1]);
                for (int32_t base_i = base_muls.size() - 2;
                     base_i >= 0;
                     --base_i) {
                        printf("%09lu", base_muls[base_i]);
                }
                printf("\n");
        }

        return EXIT_SUCCESS;
}
