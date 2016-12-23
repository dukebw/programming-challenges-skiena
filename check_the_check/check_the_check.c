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

static uint32_t
get_location(uint32_t row, uint32_t column)
{
	return row*CHESSBOARD_DIM + column;
}

static int32_t
get_king_locations(uint32_t *white_king_location,
		   uint32_t *black_king_location,
		   char *chessboard,
		   uint32_t row_index)
{
	uint32_t column_index;
	uint32_t location;
	char piece;

	for (column_index = 0;
	     column_index < CHESSBOARD_DIM;
	     ++column_index) {
		location = get_location(row_index, column_index);
		piece = chessboard[location];
		if (piece == 'k') {
			if (*white_king_location != '\0')
				return CHECK_THE_CHECK_ERROR;

			*white_king_location = location;
		} else if (piece == 'K') {
			if (*black_king_location != '\0')
				return CHECK_THE_CHECK_ERROR;

			*black_king_location = location;
		} else if (piece != '\0') {
			return CHECK_THE_CHECK_ERROR;
		}
	}

	return CHECK_THE_CHECK_SUCCESS;
}

int main(void)
{
	int32_t status;
	int32_t row_index;
	char next_row[CHESSBOARD_DIM + 1];
	char chessboard[CHESSBOARD_DIM*CHESSBOARD_DIM];
	uint32_t white_king_location;
	uint32_t black_king_location;

	memset(chessboard, '\0', sizeof(chessboard));
	for (row_index = 0;
	     row_index < CHESSBOARD_DIM;
	     ++row_index) {
		if (fgets(next_row, sizeof(next_row), stdin) == NULL)
			return CHECK_THE_CHECK_ERROR;

		memcpy(chessboard + row_index*CHESSBOARD_DIM,
		       next_row,
		       CHESSBOARD_DIM);

		status = get_king_locations(&white_king_location,
					    &black_king_location,
					    chessboard,
					    row_index);
		if (status != CHECK_THE_CHECK_SUCCESS)
			return status;
	}

	/*
	 * TODO(brendan): Iterate through the chessboard, checking if each
	 * black piece can attack the white king and vice versa.
	 */

	return CHECK_THE_CHECK_SUCCESS;
}
