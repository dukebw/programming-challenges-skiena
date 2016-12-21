#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define CHECK_THE_CHECK_SUCCESS 0
#define CHECK_THE_CHECK_ERROR (-1)
#define CHESSBOARD_DIM 8

int main(void)
{
	int32_t row_index;
	char next_row[CHESSBOARD_DIM + 1];

	for (row_index = 0;
	     row_index < CHESSBOARD_DIM;
	     ++row_index) {
		if (fgets(next_row, sizeof(next_row), stdin) == NULL)
			return CHECK_THE_CHECK_ERROR;
	}

	return CHECK_THE_CHECK_SUCCESS;
}
