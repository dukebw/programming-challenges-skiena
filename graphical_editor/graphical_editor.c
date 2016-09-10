#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_COLS 250
#define MAX_ROWS MAX_COLS
#define WHITE '0'
/* 8 bytes for name, followed by . and 3 bytes for extension */
#define MSDOS_8_3_FILENAME_SIZE_BYTES (8 + 4)

#define EDITOR_SUCCESS 0
#define EDITOR_ERROR (-1)

struct canvas {
	char pixels[MAX_ROWS*MAX_COLS];
	uint32_t cols;
	uint32_t rows;
};

static __inline bool
is_colour_valid(char colour)
{
	    return (colour >= 'A') && (colour <= 'Z');
}

static __inline bool
is_coord_valid(uint32_t coord, uint32_t max)
{
	return (coord >= 1) && (coord <= max);
}

static void
swap(uint32_t *a, uint32_t *b)
{
	uint32_t temp = *a;
	*a = *b;
	*b = temp;
}

static __inline void
set_pixel_colour(struct canvas *canvas_in,
		 uint32_t col,
		 uint32_t row,
		 char colour)
{
	canvas_in->pixels[(col - 1) + (row - 1)*canvas_in->cols] = colour;
}

static __inline void
clear_canvas(struct canvas *canvas_in)
{
	memset(canvas_in->pixels, WHITE, canvas_in->rows*canvas_in->cols);
}

static int32_t
create_mxn_image(struct canvas *canvas_in)
{
	int32_t status;
	uint32_t m;
	uint32_t n;

	status = scanf("%u %u\n", &m, &n);
	if ((status == EOF) ||
	    !is_coord_valid(m, MAX_COLS) ||
	    !is_coord_valid(n, MAX_ROWS))
		return EDITOR_ERROR;

	canvas_in->cols = m;
	canvas_in->rows = n;
	clear_canvas(canvas_in);

	return EDITOR_SUCCESS;
}

static int32_t
colour_pixel(struct canvas *canvas_in)
{
	int32_t status;
	uint32_t x;
	uint32_t y;
	char colour;

	status = scanf("%u %u %c\n", &x, &y, &colour);
	if ((status == EOF) ||
	    !is_coord_valid(x, canvas_in->cols) ||
	    !is_coord_valid(y, canvas_in->rows) ||
	    !is_colour_valid(colour))
		return EDITOR_ERROR;

	set_pixel_colour(canvas_in, x, y, colour);

	return EDITOR_SUCCESS;
}

static int32_t
draw_vertical_segment(struct canvas *canvas_in)
{
	int32_t status;
	uint32_t x;
	uint32_t y1;
	uint32_t y2;
	uint32_t row;
	char colour;

	status = scanf("%u %u %u %c\n", &x, &y1, &y2, &colour);
	if ((status == EOF) ||
	    !is_coord_valid(x, canvas_in->cols) ||
	    !is_coord_valid(y1, canvas_in->rows) ||
	    !is_coord_valid(y2, canvas_in->rows) ||
	    !is_colour_valid(colour))
		return EDITOR_ERROR;

	if (y1 > y2) {
		swap(&y1, &y2);
	}

	for (row = y1;
	     row <= y2;
	     ++row) {
		set_pixel_colour(canvas_in, x, row, colour);
	}

	return EDITOR_SUCCESS;
}

static int32_t
write_image(struct canvas *canvas_in)
{
	uint32_t row;
	uint32_t col;
	char filename[MSDOS_8_3_FILENAME_SIZE_BYTES];

	do
	{
		filename[0] = getchar();
	} while (isspace(filename[0]));

	if (fgets(filename + 1, sizeof(filename) - 1, stdin) == 0)
		return EDITOR_ERROR;

	printf("%s", filename);

	for (row = 0;
	     row < canvas_in->rows;
	     ++row) {
		for (col = 0;
		     col < canvas_in->cols;
		     ++col) {
			printf("%c",
			       (canvas_in->pixels[col + row*canvas_in->cols]));
		}

		printf("\n");
	}

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
 *                      as follows. Pixel (X,Y) belongs to R. Any other pixel
 *                      that is the same colour as pixel (X,Y) and shares a
 *                      common side with any pixel in R also
 *                      belongs to this region.
 *
 *    S Name          - Write the file name in MSDOS 8.3 format followed by the
 *                      contents of the current image.
 *
 *    X               - Terminate the session.
 *
 * Any command defined by a character other than I, C, L, V, H, K, F, S, or X is
 * skipped.
 *
 * Pixel co-ordinates are represented by two integers: a column number between
 * 1..M and a row number between 1..N, where 1 <= M,N <= 250. The origin sits in
 * the upper-left corner.
 *
 * Colours are specified by capital letters.
 */
int main(void)
{
	int32_t status = EDITOR_SUCCESS;
	char command;
	struct canvas user_canvas;

	memset(&user_canvas, 0, sizeof(user_canvas));

	while (EOF != scanf("%c", &command)) {
		switch (command) {
		case 'I':
			status = create_mxn_image(&user_canvas);
			break;
		case 'C':
			clear_canvas(&user_canvas);
			break;
		case 'L':
			status = colour_pixel(&user_canvas);
			break;
		case 'V':
			status = draw_vertical_segment(&user_canvas);
			break;
		case 'H':
			break;
		case 'K':
			break;
		case 'F':
			break;
		case 'S':
			status = write_image(&user_canvas);
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
