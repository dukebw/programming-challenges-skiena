/* C++ std library stack. */
#ifndef STD_STACK_H
#define STD_STACK_H

#include <stack>

struct stack {
        std::stack<char> stk;
};

/**
 * init_stack() - Initialize the stack data structure.
 * @stk: Stack to initialize.
 */
void init_stack(struct stack *stk);

/**
 * is_empty() - Is the stack empty?
 * @stk: Stack to check if empty.
 */
bool is_empty(struct stack *stk);

/**
 * push() - Push a value onto a stack.
 * @stk: Stack to push onto.
 * @val: Value to push.
 */
void push(struct stack *stk, char val);

/**
 * pop() - Pop a value from a stack.
 * @stk: Stack to pop from.
 *
 * Returns true if pop of an empty stack was attempted, false otherwise.
 */
bool pop(struct stack *stk);

#endif /* STD_STACK_H */
