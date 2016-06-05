#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_COL_OR_ROW_ELEMENTS 100

static void
GetMinMaxAdjacentIndexUnchecked(uint32_t *MinAdjacentElement,
                                uint32_t *MaxAdjacentElement,
                                uint32_t ElementIndex,
                                uint32_t NumElements)
{
    if (0 < ElementIndex)
    {
        *MinAdjacentElement = ElementIndex - 1;
    }
    else
    {
        *MinAdjacentElement = ElementIndex;
    }

    if ((NumElements - 1) > ElementIndex)
    {
        *MaxAdjacentElement = ElementIndex + 1;
    }
    else
    {
        *MaxAdjacentElement = ElementIndex;
    }
}

int main(void)
{
    uint32_t Status = 0;
    uint32_t NumRows;
    uint32_t NumColumns;
    char MinesweeperBoard[MAX_COL_OR_ROW_ELEMENTS][MAX_COL_OR_ROW_ELEMENTS];

    while ((EOF != scanf("%d %d\n", &NumRows, &NumColumns)) &&
           ((0 != NumRows) || (0 != NumColumns)))
    {
        if ((MAX_COL_OR_ROW_ELEMENTS < NumRows) || (MAX_COL_OR_ROW_ELEMENTS < NumColumns))
        {
            Status = 1;
            goto Error;
        }

        for (uint32_t RowIndex = 0;
             RowIndex < NumRows;
             ++RowIndex)
        {
            if (EOF == scanf("%s\n", &MinesweeperBoard[0][RowIndex]))
            {
                Status = 1;
                goto Error;
            }
        }

        for (uint32_t RowIndex = 0;
             RowIndex < NumRows;
             ++RowIndex)
        {
            for (uint32_t ColumnIndex = 0;
                 ColumnIndex < NumColumns;
                 ++ColumnIndex)
            {
                if ('*' != MinesweeperBoard[ColumnIndex][RowIndex])
                {
                    uint32_t MinAdjacentColumn;
                    uint32_t MaxAdjacentColumn;
                    GetMinMaxAdjacentIndexUnchecked(&MinAdjacentColumn, &MaxAdjacentColumn, ColumnIndex, NumColumns);

                    uint32_t MinAdjacentRow;
                    uint32_t MaxAdjacentRow;
                    GetMinMaxAdjacentIndexUnchecked(&MinAdjacentRow, &MaxAdjacentRow, RowIndex, NumRows);

                    uint32_t AdjacentMinesForTile = 0;
                    for (uint32_t AdjacentRowIndex = MinAdjacentRow;
                         AdjacentRowIndex < MaxAdjacentRow;
                         ++AdjacentRowIndex)
                    {
                        for (uint32_t AdjacentColumnIndex = MinAdjacentColumn;
                             AdjacentColumnIndex < MaxAdjacentColumn;
                             ++AdjacentColumnIndex)
                        {
                            if ('*' == MinesweeperBoard[AdjacentColumnIndex][AdjacentRowIndex])
                            {
                                ++AdjacentMinesForTile;
                            }
                        }
                    }

                    MinesweeperBoard[ColumnIndex][RowIndex] = '0' + AdjacentMinesForTile;
                }
            }
        }

        for (uint32_t RowIndex = 0;
             RowIndex < NumRows;
             ++RowIndex)
        {
            printf("%s\n", MinesweeperBoard[RowIndex]);
        }
    }

Error:
    return Status;
}
