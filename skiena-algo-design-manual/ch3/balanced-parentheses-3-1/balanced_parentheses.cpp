/* Balanced parentheses checker. */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * struct memory - Buffer for allocating arbitrary structs.
 * @data: The flat data buffer.
 * @used: Bytes used.
 */
struct memory {
        uint8_t data[1024];
        uint32_t used;
};

static struct memory MEMORY;

struct list {
        char value;
        struct list *next;
};

struct stack {
        struct list *top;
};

/**
 * push() - Push a value onto a stack.
 * @stk: Stack to push onto.
 * @val: Value to push.
 */
static void
push(struct stack *stk, char val)
{
        struct list *open_bracket = (struct list *)(MEMORY.data + MEMORY.used);
        open_bracket->next = stk->top;
        open_bracket->value = val;
        stk->top = open_bracket;

        MEMORY.used += sizeof(struct list);
        assert(MEMORY.used <= sizeof(MEMORY.data));
}

/**
 * pop() - Pop a value from a stack.
 * @stk: Stack to pop from.
 *
 * Returns true if pop of an empty stack was attempted, false otherwise.
 */
static bool
pop(struct stack *stk)
{
        if (stk->top == NULL)
                return true;

        stk->top = stk->top->next;

        MEMORY.used -= sizeof(struct list);

        return false;
}

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
                bracket_stack.top = NULL;

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

                if (!popped_empty && (bracket_stack.top == NULL))
                        printf("y\n");
                else
                        printf("n\n");
        }

        return EXIT_SUCCESS;
}
