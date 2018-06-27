/* Balanced parentheses checker. */
#ifdef HOMEGROWN
#include "homegrown_stack.hpp"
#else
#include "std_stack.hpp"
#endif /* HOMEGROWN */

#include <stack>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
        constexpr int32_t READ_BUF_MAX = 16;
        char read_buf[READ_BUF_MAX];

        assert(fgets(read_buf, READ_BUF_MAX, stdin) != NULL);

        uint32_t num_examples;
        assert(sscanf(read_buf, "%u", &num_examples) > 0);

        for (uint32_t ex_i = 0;
             ex_i < num_examples;
             ++ex_i) {
                bool popped_empty = false;

                assert(fgets(read_buf, READ_BUF_MAX, stdin) != NULL);

                uint32_t num_brackets = strlen(read_buf) - 1;

                struct stack bracket_stack;
                init_stack(&bracket_stack);

                /**
                 * Parse list, pushing for open and popping for close brackets.
                 *
                 * Balanced iff stack is empty at the end without trying to pop
                 * an empty stack.
                 */
                for (uint32_t bracket_i = 0;
                     bracket_i < num_brackets;
                     ++bracket_i) {
                        if (read_buf[bracket_i] == '(') {
                                push(&bracket_stack, read_buf[bracket_i]);
                        } else if (read_buf[bracket_i] == ')') {
                                popped_empty = pop(&bracket_stack);
                                if (popped_empty)
                                        break;
                        } else {
                                assert(false);
                        }
                }

                if (!popped_empty && is_empty(&bracket_stack))
                        printf("y\n");
                else
                        printf("n\n");
        }

        return EXIT_SUCCESS;
}
