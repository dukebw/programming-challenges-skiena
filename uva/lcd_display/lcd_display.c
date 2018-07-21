#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>

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

static lcd_digit
GlobalDigits[] =
{
    {
        .Row0 = '-',
        .Row1 =
        {
            '|', '|',
        },
        .Row2 = ' ',
        .Row3 =
        {
            '|', '|',
        },
        .Row4 = '-',
    },
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
    {
        .Row0 = '-',
        .Row1 =
        {
            ' ', '|',
        },
        .Row2 = '-',
        .Row3 =
        {
            '|', ' ',
        },
        .Row4 = '-',
    },
    {
        .Row0 = '-',
        .Row1 =
        {
            ' ', '|',
        },
        .Row2 = '-',
        .Row3 =
        {
            ' ', '|',
        },
        .Row4 = '-',
    },
    {
        .Row0 = ' ',
        .Row1 =
        {
            '|', '|',
        },
        .Row2 = '-',
        .Row3 =
        {
            ' ', '|',
        },
        .Row4 = ' ',
    },
    {
        .Row0 = '-',
        .Row1 =
        {
            '|', ' ',
        },
        .Row2 = '-',
        .Row3 =
        {
            ' ', '|',
        },
        .Row4 = '-',
    },
    {
        .Row0 = '-',
        .Row1 =
        {
            '|', ' ',
        },
        .Row2 = '-',
        .Row3 =
        {
            '|', '|',
        },
        .Row4 = '-',
    },
    {
        .Row0 = '-',
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
    {
        .Row0 = '-',
        .Row1 =
        {
            '|', '|',
        },
        .Row2 = '-',
        .Row3 =
        {
            '|', '|',
        },
        .Row4 = '-',
    },
    {
        .Row0 = '-',
        .Row1 =
        {
            '|', '|',
        },
        .Row2 = '-',
        .Row3 =
        {
            ' ', '|',
        },
        .Row4 = '-',
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
PrintDigitEvenRow(char MiddleOfRowChar, uint32_t Dashes)
{
    printf(" ");
    PrintMiddleOfRow(MiddleOfRowChar, Dashes);
    printf(" ");
}

static void
PrintDigitOddRow(char LeftSideChar, char RightSideChar, uint32_t Dashes)
{
    printf("%c", LeftSideChar);
    PrintMiddleOfRow(' ', Dashes);
    printf("%c", RightSideChar);
}

static void
PrintRow(lcd_digit *Digits, uint32_t *DigitsToDisplay, uint32_t TotalDigits, uint32_t Dashes, uint32_t RowNumber)
{
    assert(TotalDigits > 0);

    for (int32_t DigitIndex = (TotalDigits - 1);
         DigitIndex >= 0;
         --DigitIndex)
    {
        uint32_t NextDigit = DigitsToDisplay[DigitIndex];
        switch (RowNumber)
        {
            case 0:
            {
                PrintDigitEvenRow(Digits[NextDigit].Row0, Dashes);
                break;
            }
            case 1:
            {
                PrintDigitOddRow(Digits[NextDigit].Row1[0], Digits[NextDigit].Row1[1], Dashes);
                break;
            }
            case 2:
            {
                PrintDigitEvenRow(Digits[NextDigit].Row2, Dashes);
                break;
            }
            case 3:
            {
                PrintDigitOddRow(Digits[NextDigit].Row3[0], Digits[NextDigit].Row3[1], Dashes);
                break;
            }
            case 4:
            {
                PrintDigitEvenRow(Digits[NextDigit].Row4, Dashes);
                break;
            }
            default:
            {
                assert(false);
                break;
            }
        }

        if (0 < DigitIndex)
        {
            printf(" ");
        }
    }

    printf("\n");
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
        uint32_t DigitsToDisplay[8];
        uint32_t TotalDigits = 0;
        do
        {
            uint32_t NextDigit = (NumberToDisplay % 10);
            DigitsToDisplay[TotalDigits++] = NextDigit;
            NumberToDisplay /= 10;
        } while (0 < NumberToDisplay);

        for (uint32_t LineIndex = 0;
             LineIndex < (2*Dashes + 3);
             ++LineIndex)
        {
            // 0                            -> 0
            // 1..Dashes                    -> 1
            // Dashes + 1                   -> 2
            // (Dashes + 2)..(2*Dashes + 1) -> 3
            // (2*Dashes + 2)               -> 4
            uint32_t RowIndex;
            if (0 == LineIndex)
            {
                RowIndex = 0;
            }
            else if (Dashes >= LineIndex)
            {
                RowIndex = 1;
            }
            else if ((Dashes + 1) == LineIndex)
            {
                RowIndex = 2;
            }
            else if ((2*Dashes + 1) >= LineIndex)
            {
                RowIndex = 3;
            }
            else
            {
                assert((2*Dashes + 2) == LineIndex);
                RowIndex = 4;
            }

            PrintRow(GlobalDigits, DigitsToDisplay, TotalDigits, Dashes, RowIndex);
        }

        printf("\n");
    }
}
