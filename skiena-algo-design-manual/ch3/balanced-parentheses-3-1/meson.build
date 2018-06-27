project('balanced-parentheses', 'cpp')

if get_option('homegrown')
        stack_src = 'homegrown_stack.cpp'
        extra_args = ['-DHOMEGROWN']
else
        stack_src = 'std_stack.cpp'
        extra_args = []
endif

executable('balanced_parentheses',
           'balanced_parentheses.cpp',
           stack_src,
           cpp_args : extra_args)
