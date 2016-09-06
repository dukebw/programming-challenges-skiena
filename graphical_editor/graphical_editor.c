#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define MAX_COL_ROW 250
#define WHITE 0

#define EDITOR_SUCCESS 0
#define EDITOR_ERROR (-1)

struct canvas {
	char pixels[MAX_COL_ROW*MAX_COL_ROW];
	int32_t cols;
	int32_t rows;
};


static int32_t
create_mxn_image(struct canvas *canvas_in)
{
	int32_t status;
	uint32_t m;
	uint32_t n;

	status = scanf("%u %u\n", &m, &n);
	if ((EOF == status) || (MAX_COL_ROW < m) || (MAX_COL_ROW < n))
		return EDITOR_ERROR;

	memset(canvas_in->pixels, WHITE, m*n);
	canvas_in->cols = m;
	canvas_in->rows = n;

	return EDITOR_SUCCESS;
}

/*
 * Graphical editor accepting commands:
 *
 *    I M N           - Create a new MxN image with all pixels initially coloured
 *                      white (0).
 *
 *    C               - Clear the table by setting all pixels white (0). The size
 *                      remains unchanged.
 *
 *    L X Y C         - Colours the pixel (X,Y) in colour (C).
 *
 *    V X Y1 Y2 C     - Draw a vertical segment of colour (C) in column X, between
 *                      the rows Y1 and Y2 inclusive.
 *
 *    H X1 Y2 Y C     - Draw a horizontal segment of colour (C) in the row Y,
 *                      between the columns X1 and X2 inclusive.
 *
 *    K X1 Y1 X2 Y2 C - Draw a filled rectangle of colour C, where (X1,Y1) is
 *                      the upper-let and (X2,Y2) is the lower right corner.
 *
 *    F X Y C         - Fill the region R with the colour C, where R is defined
 *                      as follows. Pixel (X,Y) belongs to R. Any other pixel that is the same
 *                      colour as pixel (X,Y) and shares a common side with any pixel in R also
 *                      belongs to this region.
 *
 *    S Name          - Write the file name in MSDOS 8.3 format followed by the
 *                      contents of the current image.
 *
 *    X               - Terminate the session.
 *
 * Any command defined by a character other than I, C, L, V, H, K, F, S, or X is skipped.
 *
 * Pixel co-ordinates are represented by two integers: a column number between
 * 1..M and a row number between 1..N, where 1 <= M,N <= 250. The origin sits in the upper-left corner.
 *
 * Colours are specified by capital letters.
 */
int main(void)
{
	int32_t status = EDITOR_SUCCESS;
	char command;
	struct canvas user_canvas;

	while (EOF != (command = getchar())) {
		switch (command) {
		case 'I':
			status = create_mxn_image(&user_canvas);
			break;
		case 'C':
			break;
		case 'L':
			break;
		case 'V':
			break;
		case 'H':
			break;
		case 'K':
			break;
		case 'F':
			break;
		case 'S':
			break;
		case 'X':
			break;
		default:
			break;
		}

		if (EDITOR_SUCCESS != status)
			return status;
	}

	return EDITOR_SUCCESS;
}
