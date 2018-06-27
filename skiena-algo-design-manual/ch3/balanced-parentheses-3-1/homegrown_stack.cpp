#include "homegrown_stack.hpp"
#include <stdint.h>
#include <assert.h>

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

void init_stack(struct stack *stk)
{
        if (stk == NULL)
                return;

        stk->top = NULL;
}

bool is_empty(struct stack *stk)
{
        return (stk == NULL) || (stk->top == NULL);
}

void push(struct stack *stk, char val)
{
        struct list *open_bracket = (struct list *)(MEMORY.data + MEMORY.used);
        open_bracket->next = stk->top;
        open_bracket->value = val;
        stk->top = open_bracket;

        MEMORY.used += sizeof(struct list);
        assert(MEMORY.used <= sizeof(MEMORY.data));
}

bool pop(struct stack *stk)
{
        if (stk->top == NULL)
                return true;

        stk->top = stk->top->next;

        MEMORY.used -= sizeof(struct list);

        return false;
}
