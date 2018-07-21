/**
 * Identifying map tiles - https://open.kattis.com/problems/maptiles2
 *
 * For a map of zoom level n, there are 4^n tiles, which can be represented by
 * either quadkey (which specifies quadrants in each zoom level from 0...n), or
 * coordinates.
 *
 * For a given quadkey, output n and coordinates.
 */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
        char read_buffer[32];

        while (fgets(read_buffer, sizeof(read_buffer), stdin) != NULL) {
                int32_t quadkey_len = strlen(read_buffer) - 1;
                /* 1 <= length(s) <= 30 */
                assert((quadkey_len > 0) && (quadkey_len <= 30));

                /**
                 * Starting range of coordinates is (x1, y1) to (x2, y2)
                 * where (x1, y1) := (0, 0) and (x2, y2) := (2^n - 1, 2^n - 1)
                 *
                 * mx := (x1 + x2 + 1)/2 - 1
                 * my := (y1 + y2 + 1)/2 - 1
                 * Quadrant 0: range is (x1, y1) to (mx, my)
                 * Quadrant 1: range is (mx + 1, y1) to (x2, my)
                 * Quadrant 2: range is (x1, my + 1) to (mx, y2)
                 * Quadrant 3: range is (mx + 1, my + 1) to (x2, y2)
                 */
                uint32_t x1 = 0;
                uint32_t y1 = 0;
                uint32_t x2 = (1 << quadkey_len) - 1;
                uint32_t y2 = (1 << quadkey_len) - 1;
                for (int32_t i = 0;
                     i < quadkey_len;
                     ++i) {
                        uint32_t mx = (x1 + x2 + 1)/2 - 1;
                        uint32_t my = (y1 + y2 + 1)/2 - 1;

                        switch (read_buffer[i]) {
                        case '0':
                                x2 = mx;
                                y2 = my;
                                break;
                        case '1':
                                x1 = mx + 1;
                                y2 = my;
                                break;
                        case '2':
                                y1 = my + 1;
                                x2 = mx;
                                break;
                        case '3':
                                x1 = mx + 1;
                                y1 = my + 1;
                                break;
                        default:
                                assert(false);
                        }
                }

                printf("%d %u %u\n", quadkey_len, x1, y1);
        }

        return EXIT_SUCCESS;
}
