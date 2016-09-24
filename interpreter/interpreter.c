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
#define RAM_SIZE_WORDS 1000
#define NUM_REGISTERS 10
#define RAM_MAX_VALUE 999

static int32_t
read_ram(uint32_t *ram)
{
	uint32_t ram_index;
	uint32_t ram_value;
	char input_line[3 + 1 + 1];

	for (ram_index = 0;
	     ram_index < RAM_SIZE_WORDS;
	     ++ram_index) {
		if (fgets(input_line, sizeof(input_line), stdin) == 0)
			return INTERPRETER_ERROR;

		if (input_line[0] == '\n')
			break;

		if ((sscanf(input_line, "%u\n", &ram_value) == EOF) ||
		    (ram_value > RAM_MAX_VALUE))
			return INTERPRETER_ERROR;

		ram[ram_index] = ram_value;
	}

	memset(ram + ram_index,
	       0,
	       (RAM_SIZE_WORDS - ram_index)*sizeof(uint32_t));

	return INTERPRETER_SUCCESS;
}

int main(void)
{
	int32_t status;
	int32_t num_test_cases;
	uint32_t ram[RAM_SIZE_WORDS];
	uint32_t registers[NUM_REGISTERS];

	while (scanf("%d\n\n", &num_test_cases) != EOF) {
		for (;
		     num_test_cases > 0;
		     --num_test_cases) {
			memset(registers, 0, sizeof(registers));

			status = read_ram(ram);
			if (status != INTERPRETER_SUCCESS)
				return status;
		}
	}

	return INTERPRETER_SUCCESS;
}
