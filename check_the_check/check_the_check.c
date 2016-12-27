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

#define WHITE_KING 'K'
#define BLACK_KING 'k'

enum {
	EMPTY = '.',
	PAWN = 'p',
	KNIGHT = 'n',
	BISHOP = 'b',
	ROOK = 'r',
	QUEEN = 'q',
	KING = 'k'
};

const char BLACK_KING_LABEL[] = "black";
const char WHITE_KING_LABEL[] = "white";

static uint32_t
get_location(uint32_t row, uint32_t column)
{
	return row*CHESSBOARD_DIM + column;
}

static bool
is_valid_board_dimension(int32_t dimension)
{
	return (dimension >= 0) && (dimension < CHESSBOARD_DIM);
}

static const char *
check_for_king(const char *chessboard,
	       int32_t row_index,
	       int32_t column_index,
	       char king_to_attack)
{
	char piece;
	uint32_t location;

	if (!is_valid_board_dimension(row_index) ||
	    !is_valid_board_dimension(column_index))
		return NULL;

	location = get_location(row_index, column_index);
	piece = chessboard[location];

	if (piece == king_to_attack) {
		if (piece == BLACK_KING)
			return BLACK_KING_LABEL;
		else if (piece == WHITE_KING)
			return WHITE_KING_LABEL;
	}

	return NULL;
}

static const char *
check_for_pawn_attack(const char *chessboard,
		      int32_t row_to_attack,
		      int32_t pawn_column,
		      char king_to_attack)
{
	const char *king_in_check = check_for_king(chessboard,
						   row_to_attack,
						   pawn_column - 1,
						   king_to_attack);
	if (king_in_check)
		return king_in_check;

	return check_for_king(chessboard,
			      row_to_attack,
			      pawn_column + 1,
			      king_to_attack);
}

static int32_t
read_chessboard(char *chessboard, uint32_t chessboard_size_bytes)
{
	int32_t row_index;
	char next_row[CHESSBOARD_DIM + 1];

	memset(chessboard, '\0', chessboard_size_bytes);

	for (row_index = 0;
	     row_index < CHESSBOARD_DIM;
	     ++row_index) {
		if (fgets(next_row, sizeof(next_row), stdin) == NULL)
			return CHECK_THE_CHECK_ERROR;

		memcpy(chessboard + row_index*CHESSBOARD_DIM,
		       next_row,
		       CHESSBOARD_DIM);
	}

	return CHECK_THE_CHECK_SUCCESS;
}

const char *
check_attacks_for_square(const char *chessboard, uint32_t row_index, uint32_t column_index)
{
	const char *king_in_check = NULL;
	uint32_t location = get_location(row_index, column_index);
	char piece = chessboard[location];
	bool is_piece_black = islower(piece);

	switch (tolower(piece)) {
	case KING:
	case EMPTY:
		break;
	case PAWN:
		if (is_piece_black) {
			king_in_check = check_for_pawn_attack(chessboard,
							      row_index + 1,
							      column_index,
							      WHITE_KING);
			if (king_in_check)
				return king_in_check;
		} else {
			king_in_check = check_for_pawn_attack(chessboard,
							      row_index - 1,
							      column_index,
							      BLACK_KING);
			if (king_in_check)
				return king_in_check;
		}
		break;
	case KNIGHT:
		break;
	case BISHOP:
		break;
	case ROOK:
		break;
	case QUEEN:
		break;
	default:
		return NULL;
	}

	return king_in_check;
}

static const char *
find_king_in_check(const char *chessboard)
{
	int32_t row_index;
	int32_t column_index;
	const char *king_in_check;

	for (row_index = 0;
	     row_index < CHESSBOARD_DIM;
	     ++row_index) {
		for (column_index = 0;
		     column_index < CHESSBOARD_DIM;
		     ++column_index) {
			king_in_check = check_attacks_for_square(chessboard,
								 row_index,
								 column_index);
			if (king_in_check)
				return king_in_check;
		}
	}

	return NULL;
}

int main(void)
{
	int32_t status;
	uint32_t game_count;

	for (game_count = 1;
	     ;
	     ++game_count) {
		char chessboard[CHESSBOARD_DIM*CHESSBOARD_DIM];

		status = read_chessboard(chessboard, sizeof(chessboard));
		if (status != CHECK_THE_CHECK_SUCCESS)
			return status;

		const char *king_in_check = find_king_in_check(chessboard);

		printf("Game #%d: ", game_count);
		if (king_in_check)
			printf("%s king is in check.\n", king_in_check);
		else
			printf("no king is in check.\n");
	}

	return CHECK_THE_CHECK_SUCCESS;
}
