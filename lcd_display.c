#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define ROWS (2 + 3)
#define COLUMNS (1 + 2)

typedef struct lcd_digit
{
    char Layout[ROWS][COLUMNS];
} lcd_digit;

static lcd_digit GlobalOne =
{
    .Layout =
    {
        {' ', ' ', ' '},
        {' ', ' ', '|'},
        {' ', ' ', ' '},
        {' ', ' ', '|'},
        {' ', ' ', ' '},
    },
};

int main(void)
{
    uint32_t Dashes;
    uint32_t NumberToDisplay;
    while ((EOF != scanf("%u %u\n", &Dashes, &NumberToDisplay)) &&
           (Dashes >= 1) &&
           (Dashes <= 10) &&
           (NumberToDisplay <= 99999999))
    {
        for (uint32_t RowIndex = 0;
             RowIndex < ROWS;
             ++RowIndex)
        {
            for (uint32_t ColumnIndex = 0;
                 ColumnIndex < COLUMNS;
                 ++ColumnIndex)
            {
                printf("%c", GlobalOne.Layout[RowIndex][ColumnIndex]);
            }
            printf("\n");
        }
    }
}
