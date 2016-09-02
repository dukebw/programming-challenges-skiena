#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define ROWS (2 + 3)
#define COLUMNS (1 + 2)

typedef struct lcd_digit
{
    char Row0;
    char Row1[2];
    char Row2;
    char Row3[2];
    char Row4;
} lcd_digit;

static lcd_digit GlobalDigits[] =
{
    {
        .Row0 = ' ',
        .Row1 =
        {
            ' ', '|',
        },
        .Row2 = ' ',
        .Row3 =
        {
            ' ', '|',
        },
        .Row4 = ' ',
    },
};

static void
PrintMiddleOfRow(char MiddleOfRowChar, uint32_t Dashes)
{
    for (uint32_t DashesIndex = 0;
         DashesIndex < Dashes;
         ++DashesIndex)
    {
        printf("%c", MiddleOfRowChar);
    }
}

static void
PrintEvenRow(char MiddleOfRowChar, uint32_t Dashes)
{
    printf(" ");
    PrintMiddleOfRow(MiddleOfRowChar, Dashes);
    printf(" \n");
}

static void
PrintOddRow(char LeftSideChar, char RightSideChar, uint32_t Dashes)
{
    for (uint32_t DashesIndex = 0;
         DashesIndex < Dashes;
         ++DashesIndex)
    {
        printf("%c", LeftSideChar);
        PrintMiddleOfRow(' ', Dashes);
        printf("%c\n", RightSideChar);
    }
}

int main(void)
{
    uint32_t Dashes;
    uint32_t NumberToDisplay;
    while ((EOF != scanf("%u %u\n", &Dashes, &NumberToDisplay)) &&
           (Dashes >= 1) &&
           (Dashes <= 10) &&
           (NumberToDisplay <= 99999999))
    {
        PrintEvenRow(GlobalDigits[0].Row0, Dashes);
        PrintOddRow(GlobalDigits[0].Row1[0], GlobalDigits[0].Row1[1], Dashes);
        PrintEvenRow(GlobalDigits[0].Row2, Dashes);
        PrintOddRow(GlobalDigits[0].Row3[0], GlobalDigits[0].Row3[1], Dashes);
        PrintEvenRow(GlobalDigits[0].Row4, Dashes);
    }
}
