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
#define WHITE 'O'
/*
 * 8 bytes for name, followed by . and 3 bytes for extension
 * (one byte for null)
 */
#define MSDOS_8_3_FILENAME_SIZE_BYTES 8
/* #define MSDOS_8_3_EXTENSION_SIZE_BYTES 3 */
#define MSDOS_8_3_EXTENSION_SIZE_BYTES 5
#define MSDOS_8_3_TOTAL_SIZE_BYTES \
	(MSDOS_8_3_FILENAME_SIZE_BYTES + 1 + MSDOS_8_3_EXTENSION_SIZE_BYTES + 1)

#define EDITOR_SUCCESS 0
#define EDITOR_ERROR (-1)

struct pixel {
	char colour;
	bool marked;
};

struct canvas {
	struct pixel pixels[MAX_ROWS*MAX_COLS];
	uint32_t cols;
	uint32_t rows;
};

static bool
is_colour_valid(char colour)
{
	return (colour >= 'A') && (colour <= 'Z');
}

static bool
is_coord_valid(uint32_t coord, uint32_t max)
{
	return (coord >= 1) && (coord <= max);
}

static bool
is_coord_pair_valid(uint32_t coord1, uint32_t coord2, uint32_t max)
{
	return is_coord_valid(coord1, max) && is_coord_valid(coord2, max);
}

static void
swap(uint32_t *a, uint32_t *b)
{
	uint32_t temp = *a;
	*a = *b;
	*b = temp;
}

static uint32_t
get_pixel_index(struct canvas *canvas_in, uint32_t col, uint32_t row)
{
	return (col - 1) + (row - 1)*canvas_in->cols;
}

/*
 * Take col/row values in [1..N].
 */
static void
set_pixel_colour(struct canvas *canvas_in,
		 uint32_t col,
		 uint32_t row,
		 char colour)
{
	uint32_t pixel_index = get_pixel_index(canvas_in, col, row);

	canvas_in->pixels[pixel_index].colour = colour;
}

static char
get_pixel_colour(struct canvas *canvas_in, uint32_t col, uint32_t row)
{
	uint32_t pixel_index = get_pixel_index(canvas_in, col, row);

	return canvas_in->pixels[pixel_index].colour;
}

static void
set_row_pixel_colour(struct canvas *canvas_in,
		     uint32_t row,
		     uint32_t x1,
		     uint32_t x2,
		     char colour)
{
	uint32_t col;

	for (col = x1;
	     col <= x2;
	     ++col) {
		set_pixel_colour(canvas_in, col, row, colour);
	}
}

static void
clear_canvas(struct canvas *canvas_in)
{
	uint32_t pixeL_index;

	for (pixeL_index = 0;
	     pixeL_index < (canvas_in->rows*canvas_in->cols);
	     ++pixeL_index) {
		canvas_in->pixels[pixeL_index].colour = WHITE;
	}
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
	    !is_coord_pair_valid(y1, y2, canvas_in->rows) ||
	    !is_colour_valid(colour))
		return EDITOR_ERROR;

	if (y1 > y2)
		swap(&y1, &y2);

	for (row = y1;
	     row <= y2;
	     ++row) {
		set_pixel_colour(canvas_in, x, row, colour);
	}

	return EDITOR_SUCCESS;
}

static int32_t
draw_horizontal_segment(struct canvas *canvas_in)
{
	int32_t status;
	uint32_t x1;
	uint32_t x2;
	uint32_t y;
	char colour;

	status = scanf("%u %u %u %c\n", &x1, &x2, &y, &colour);
	if ((status == EOF) ||
	    !is_coord_pair_valid(x1, x2, canvas_in->cols) ||
	    !is_coord_valid(y, canvas_in->rows) ||
	    !is_colour_valid(colour))
		return EDITOR_ERROR;

	if (x1 > x2)
		swap(&x1, &x2);

	set_row_pixel_colour(canvas_in, y, x1, x2, colour);

	return EDITOR_SUCCESS;
}

static int32_t
draw_filled_rectangle(struct canvas *canvas_in)
{
	int32_t status;
	uint32_t x1;
	uint32_t x2;
	uint32_t y1;
	uint32_t y2;
	uint32_t row;
	char colour;

	status = scanf("%u %u %u %u %c\n", &x1, &y1, &x2, &y2, &colour);
	if ((status == EOF) ||
	    !is_coord_pair_valid(x1, x2, canvas_in->cols) ||
	    !is_coord_pair_valid(y1, y2, canvas_in->rows) ||
	    !is_colour_valid(colour) ||
	    (x2 < x1) ||
	    (y2 < y1))
		return EDITOR_ERROR;

	for (row = y1;
	     row <= y2;
	     ++row) {
		set_row_pixel_colour(canvas_in, row, x1, x2, colour);
	}

	return EDITOR_SUCCESS;
}

static void
expand_region_r(struct canvas *canvas_in,
		uint32_t x,
		uint32_t y,
		char r_colour,
		char colour_to_fill);

static void
mark_and_maybe_expand_region_r(struct canvas *canvas_in,
			       uint32_t x,
			       uint32_t y,
			       char r_colour,
			       char colour_to_fill,
			       uint32_t pixel_index)
{
	canvas_in->pixels[pixel_index].marked = true;

	if (canvas_in->pixels[pixel_index].colour == r_colour)
		expand_region_r(canvas_in, x, y, r_colour, colour_to_fill);
}

static void
x_maybe_expand_region_r(struct canvas *canvas_in,
			uint32_t x,
			uint32_t y,
			char r_colour,
			char colour_to_fill)
{
	uint32_t pixel_index = get_pixel_index(canvas_in, x, y);

	if (is_coord_valid(x, canvas_in->cols) &&
	    !canvas_in->pixels[pixel_index].marked) {
		mark_and_maybe_expand_region_r(canvas_in,
					       x,
					       y,
					       r_colour,
					       colour_to_fill,
					       pixel_index);
	}
}

static void
y_maybe_expand_region_r(struct canvas *canvas_in,
			uint32_t x,
			uint32_t y,
			char r_colour,
			char colour_to_fill)
{
	uint32_t pixel_index = get_pixel_index(canvas_in, x, y);

	if (is_coord_valid(y, canvas_in->rows) &&
	    !canvas_in->pixels[pixel_index].marked) {
		mark_and_maybe_expand_region_r(canvas_in,
					       x,
					       y,
					       r_colour,
					       colour_to_fill,
					       pixel_index);
	}
}

static void
expand_region_r(struct canvas *canvas_in,
		uint32_t x,
		uint32_t y,
		char r_colour,
		char colour_to_fill)
{
	x_maybe_expand_region_r(canvas_in, x + 1, y, r_colour, colour_to_fill);
	y_maybe_expand_region_r(canvas_in, x, y + 1, r_colour, colour_to_fill);
	x_maybe_expand_region_r(canvas_in, x - 1, y, r_colour, colour_to_fill);
	y_maybe_expand_region_r(canvas_in, x, y - 1, r_colour, colour_to_fill);

	set_pixel_colour(canvas_in, x, y, colour_to_fill);
}

static int32_t
fill_in_region_r(struct canvas *canvas_in)
{
	int32_t status;
	uint32_t x;
	uint32_t y;
	uint32_t pixel_index;
	char colour_to_fill;

	status = scanf("%u %u %c\n", &x, &y, &colour_to_fill);
	if ((status == EOF) ||
	    !is_coord_valid(x, canvas_in->cols) ||
	    !is_coord_valid(y, canvas_in->rows) ||
	    !is_colour_valid(colour_to_fill))
		return EDITOR_ERROR;

	for (pixel_index = 0;
	     pixel_index < (canvas_in->rows*canvas_in->cols);
	     ++pixel_index) {
		canvas_in->pixels[pixel_index].marked = false;
	}

	pixel_index = get_pixel_index(canvas_in, x, y);

	expand_region_r(canvas_in,
			x,
			y,
			canvas_in->pixels[pixel_index].colour,
			colour_to_fill);

	return EDITOR_SUCCESS;
}

static int32_t
read_msdos_8_3_filename_and_print(void)
{
	uint32_t name_index;
	char filename[MSDOS_8_3_TOTAL_SIZE_BYTES];

	do {
		filename[0] = getchar();
	} while (isspace(filename[0]));

	for (name_index = 1;
	     (name_index < MSDOS_8_3_FILENAME_SIZE_BYTES);
	     ++name_index) {
		filename[name_index] = getchar();

		if (filename[name_index] == '.')
			break;
	}

	if (filename[name_index] != '.')
		return EDITOR_ERROR;

	++name_index;

	/* TODO(brendan): print newline properly if extension is too long */
	if (fgets(filename + name_index,
		  MSDOS_8_3_EXTENSION_SIZE_BYTES + 1,
		  stdin) == 0)
		return EDITOR_ERROR;

	printf("%s", filename);

	return EDITOR_SUCCESS;
}

static int32_t
write_image(struct canvas *canvas_in)
{
	int32_t status;
	uint32_t row;
	uint32_t col;

	status = read_msdos_8_3_filename_and_print();
	if (status != EDITOR_SUCCESS)
		return status;

	for (row = 1;
	     row <= canvas_in->rows;
	     ++row) {
		for (col = 1;
		     col <= canvas_in->cols;
		     ++col) {
			printf("%c", get_pixel_colour(canvas_in, col, row));
		}

		printf("\n");
	}

	return EDITOR_SUCCESS;
}

/*
 * Graphical editor accepting commands:
 *
 *    I M N           - Create a new MxN image with all pixels initially
 *                      coloured white (0).
 *
 *    C               - Clear the table by setting all pixels white (0). The
 *                      size remains unchanged.
 *
 *    L X Y C         - Colours the pixel (X,Y) in colour (C).
 *
 *    V X Y1 Y2 C     - Draw a vertical segment of colour (C) in column X,
 *                      between the rows Y1 and Y2 inclusive.
 *
 *    H X1 Y2 Y C     - Draw a horizontal segment of colour (C) in the row Y,
 *                      between the columns X1 and X2 inclusive.
 *
 *    K X1 Y1 X2 Y2 C - Draw a filled rectangle of colour C, where (X1,Y1) is
 *                      the upper-left and (X2,Y2) is the lower right corner.
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

	while (scanf("%c", &command) != EOF) {
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
			status = draw_horizontal_segment(&user_canvas);
			break;
		case 'K':
			status = draw_filled_rectangle(&user_canvas);
			break;
		case 'F':
			status = fill_in_region_r(&user_canvas);
			break;
		case 'S':
			status = write_image(&user_canvas);
			break;
		case 'X':
			goto success_exit;
		default:
			break;
		}

		if (status != EDITOR_SUCCESS)
			(void)status; /* return status; */
	}

success_exit:
	return EDITOR_SUCCESS;
}
