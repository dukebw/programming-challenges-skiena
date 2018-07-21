#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define MAX_NUMBER_OF_STUDENTS 1000
#define MAX_AMOUNT_PER_STUDENT 10000.00f

int main(void)
{
    int32_t Status = 0;

    int32_t NumberOfStudents;
    while ((EOF != scanf("%d\n", &NumberOfStudents)) &&
           (0 != NumberOfStudents))
    {
        if (MAX_NUMBER_OF_STUDENTS < NumberOfStudents)
        {
            Status = 1;
            goto Error;
        }

        int32_t PenniesSpentByStudents[MAX_NUMBER_OF_STUDENTS];
        uint32_t TotalSpentPennies = 0;
        for (int32_t StudentIndex = 0;
             StudentIndex < NumberOfStudents;
             ++StudentIndex)
        {
            double NextAmountSpentDollars;
            if ((EOF == scanf("%lf\n", &NextAmountSpentDollars)) ||
                (MAX_AMOUNT_PER_STUDENT < NextAmountSpentDollars))
            {
                Status = 1;
                goto Error;
            }

            PenniesSpentByStudents[StudentIndex] = (int32_t)roundf(100.0f*NextAmountSpentDollars);

            TotalSpentPennies += PenniesSpentByStudents[StudentIndex];
        }

        int32_t FloorAverage = TotalSpentPennies/NumberOfStudents;
        int32_t CeilAverage = (FloorAverage + 1);
        int32_t MinPenniesExchanged = 0;
        int32_t StudentsBelowAverage = 0;
        int32_t PenniesGivenByAboveAvg = 0;
        for (int32_t StudentIndex = 0;
             StudentIndex < NumberOfStudents;
             ++StudentIndex)
        {
            if (FloorAverage > PenniesSpentByStudents[StudentIndex])
            {
                ++StudentsBelowAverage;
                MinPenniesExchanged += (FloorAverage - PenniesSpentByStudents[StudentIndex]);
            }
            else if (CeilAverage < PenniesSpentByStudents[StudentIndex])
            {
                PenniesGivenByAboveAvg += (PenniesSpentByStudents[StudentIndex] - CeilAverage);
            }
        }

        if (PenniesGivenByAboveAvg > MinPenniesExchanged)
        {
            MinPenniesExchanged += (PenniesGivenByAboveAvg - MinPenniesExchanged);
        }

        double MinAmountExchanged = ((double)MinPenniesExchanged)/100.0f;
        printf("$%.2f\n", MinAmountExchanged);
    }

Error:
    return Status;
}
