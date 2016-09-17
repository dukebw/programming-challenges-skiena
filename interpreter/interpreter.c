#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define INTERPRETER_SUCCESS 0
#define INTERPRETER_ERROR (-1)

int main(void)
{
	int32_t num_test_cases;
	uint32_t ram_index;
	uint32_t ram_value;

	while (scanf("%d\n", &num_test_cases) != EOF) {
		for (;
		     num_test_cases >= 0;
		     --num_test_cases) {
			for (ram_index = 0;
			     ram_index < 1000;
			     ++ram_index) {
				/* TODO(brendan): test for double newline end case */
				if (scanf("%u\n", &ram_value) == EOF)
					return INTERPRETER_ERROR;
			}
		}
	}

	return INTERPRETER_SUCCESS;
}
