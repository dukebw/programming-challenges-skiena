#include "std_stack.hpp"

void init_stack(struct stack *stk)
{
        (void)stk;
}

bool is_empty(struct stack *stk)
{
        return (stk == NULL) || stk->stk.empty();
}

bool pop(struct stack *stk)
{
        if (stk->stk.empty())
                return true;

        stk->stk.pop();

        return false;
}

void push(struct stack *stk, char val)
{
        stk->stk.push(val);
}
