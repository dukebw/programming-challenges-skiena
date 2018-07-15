/**
 * Planina - https://open.kattis.com/problems/planina
 *
 * 0. Start with four points, forming a square.
 * 1. Add points in the middle of the sides of the square.
 * 2. Add a point in the exact center of the square.
 *
 * Repeat 1 and 2 for N iterations.
 *
 * Some points belong to multiple squares. How much memory is required after N
 * iterations?
 *
 * Solution: (2**N + 1)**2
 */
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
        char read_buffer[4];

        assert(fgets(read_buffer, sizeof(read_buffer), stdin) != NULL);
        assert(strlen(read_buffer) <= 3);

        int32_t N;
        assert(sscanf(read_buffer, "%d", &N) == 1);
        assert((N >= 1) && (N <= 15));

        int32_t num_side_points = (1 << N) + 1;
        printf("%d\n", num_side_points*num_side_points);

        return EXIT_SUCCESS;
}
