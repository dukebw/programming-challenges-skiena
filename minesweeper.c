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
    char MinesweeperBoard[MAX_COL_OR_ROW_ELEMENTS + 1][MAX_COL_OR_ROW_ELEMENTS + 1];

    for (uint32_t FieldNumber = 1;
         (EOF != scanf("%d %d\n", &NumRows, &NumColumns)) &&
         ((0 != NumRows) || (0 != NumColumns));
         ++FieldNumber)
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
            if (EOF == scanf("%s\n", &MinesweeperBoard[RowIndex][0]))
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
                if ('*' != MinesweeperBoard[RowIndex][ColumnIndex])
                {
                    uint32_t MinAdjacentColumn;
                    uint32_t MaxAdjacentColumn;
                    GetMinMaxAdjacentIndexUnchecked(&MinAdjacentColumn, &MaxAdjacentColumn, ColumnIndex, NumColumns);

                    uint32_t MinAdjacentRow;
                    uint32_t MaxAdjacentRow;
                    GetMinMaxAdjacentIndexUnchecked(&MinAdjacentRow, &MaxAdjacentRow, RowIndex, NumRows);

                    uint32_t AdjacentMinesForTile = 0;
                    for (uint32_t AdjacentRowIndex = MinAdjacentRow;
                         AdjacentRowIndex <= MaxAdjacentRow;
                         ++AdjacentRowIndex)
                    {
                        for (uint32_t AdjacentColumnIndex = MinAdjacentColumn;
                             AdjacentColumnIndex <= MaxAdjacentColumn;
                             ++AdjacentColumnIndex)
                        {
                            if ('*' == MinesweeperBoard[AdjacentRowIndex][AdjacentColumnIndex])
                            {
                                ++AdjacentMinesForTile;
                            }
                        }
                    }

                    MinesweeperBoard[RowIndex][ColumnIndex] = '0' + AdjacentMinesForTile;
                }
            }
        }

        if (1 < FieldNumber)
        {
            printf("\n");
        }

        printf("Field #%u:\n", FieldNumber);

        for (uint32_t RowIndex = 0;
             RowIndex < NumRows;
             ++RowIndex)
        {
            MinesweeperBoard[RowIndex][NumColumns] = 0;
            printf("%s\n", MinesweeperBoard[RowIndex]);
        }
    }

Error:
    return Status;
}
