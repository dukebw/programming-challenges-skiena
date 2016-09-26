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

enum {
	HALT = 1,
	MOV_I = 2,
	ADD_I = 3,
	MUL_I = 4,
	MOV = 5,
	ADD = 6,
	MUL = 7,
	LDR = 8,
	STR = 9,
	BNE = 0,
};

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

		if (sscanf(input_line, "%u\n", &ram_value) == EOF)
			return INTERPRETER_ERROR;

		ram[ram_index] = ram_value;
	}

	memset(ram + ram_index,
	       0,
	       (RAM_SIZE_WORDS - ram_index)*sizeof(uint32_t));

	return INTERPRETER_SUCCESS;
}

static uint32_t
get_opcode(uint32_t mem_word)
{
	return ((mem_word/100) % 10);
}

static int32_t
execute_program(uint32_t *ram, uint32_t *registers)
{
	uint32_t program_counter = 0;
	uint32_t left_operand;
	uint32_t right_operand;
	uint32_t opcode;

	for (opcode = get_opcode(ram[program_counter]);
	     opcode != HALT;
	     opcode = get_opcode(ram[program_counter])) {
		left_operand = (ram[program_counter]/10) % 10;
		right_operand = ram[program_counter] % 10;

		switch (opcode) {
		case MOV_I:
			registers[left_operand] = right_operand % 10;
			break;
		/* TODO(brendan): add other opcodes */
		default:
			return INTERPRETER_ERROR;
		}
	}

	return INTERPRETER_SUCCESS;
}

/*
 * 100 means halt
 * 2dn means set register d to n (between 0 and 9)
 * 3dn means add n to register d
 * 4dn means multiply register d by n
 * 5ds means set register d to the value of register s
 * 6ds means add the value of register s to the value of register d
 * 7ds means multiply register d by the value of register s
 * 8da means set register d to the value in RAM whose address is in register a
 * 9sa means set the value in RAM whose address is in register a to that of
 *     register s
 * 0ds means goto the location in register d unless register s contains 0
 *
 * All results are reduced modulo 1000
 */
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

			status = execute_program(ram, registers);
			if (status != INTERPRETER_SUCCESS)
				return status;
		}
	}

	return INTERPRETER_SUCCESS;
}
