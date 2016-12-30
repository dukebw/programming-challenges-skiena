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

#define ARRAY_LENGTH(array) (sizeof(array)/sizeof((array)[0]))

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

static char
get_piece(const char *chessboard, uint32_t row, uint32_t column)
{
	return chessboard[row*CHESSBOARD_DIM + column];
}

static bool
is_valid_board_dimension(int32_t dimension)
{
	return (dimension >= 0) && (dimension < CHESSBOARD_DIM);
}

static bool
are_valid_board_dimensions(uint32_t row, uint32_t column)
{
	return (is_valid_board_dimension(row) &&
		is_valid_board_dimension(column));
}

static const char *
get_king_label(char piece)
{
	if (piece == BLACK_KING)
		return BLACK_KING_LABEL;
	else if (piece == WHITE_KING)
		return WHITE_KING_LABEL;

	return NULL;
}

static const char *
check_for_king(const char *chessboard,
	       int32_t row_index,
	       int32_t column_index,
	       char king_to_attack)
{
	char piece;

	if (!are_valid_board_dimensions(row_index, column_index))
		return NULL;

	piece = get_piece(chessboard, row_index, column_index);

	if (piece == king_to_attack) {
		return get_king_label(piece);
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
	char piece = get_piece(chessboard, row_index, column_index);
	bool is_piece_black = islower(piece);
	char king_to_attack = is_piece_black ? WHITE_KING : BLACK_KING;

	switch (tolower(piece)) {
	case KING:
	case EMPTY:
		break;
	case PAWN:
	{
		int32_t row_to_attack;
		if (is_piece_black) {
			row_to_attack = row_index + 1;
		} else {
			row_to_attack = row_index - 1;
		}
		king_in_check = check_for_pawn_attack(chessboard,
						      row_to_attack,
						      column_index,
						      king_to_attack);
		if (king_in_check)
			return king_in_check;
		break;
	}
	case KNIGHT:
	{
		int32_t attack_locations[][2] = {{1, 2}, {-1, 2}, {1, -2}, {-1, -2},
						 {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};

		for (uint32_t attack_index = 0;
		     attack_index < ARRAY_LENGTH(attack_locations);
		     ++attack_index) {
			king_in_check = check_for_king(chessboard,
						       attack_locations[attack_index][0],
						       attack_locations[attack_index][1],
						       king_to_attack);
			if (king_in_check)
				return king_in_check;
		}
		break;
	}
	case BISHOP:
	{
		int32_t directions[][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

		for (uint32_t direction_index = 0;
		     direction_index < ARRAY_LENGTH(directions);
		     ++direction_index) {
			for (uint32_t step_index = 1;
			     ;
			     ++step_index) {
				uint32_t attack_row = row_index + directions[direction_index][0]*step_index;
				uint32_t attack_column = row_index + directions[direction_index][1]*step_index;
				if (!are_valid_board_dimensions(attack_row, attack_column))
					break;

				char piece_to_attack = get_piece(chessboard, attack_row, attack_column);
				if (piece_to_attack == king_to_attack)
					return get_king_label(piece_to_attack);
				else if (piece_to_attack != EMPTY)
					break;
			}
		}
		break;
	}
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
