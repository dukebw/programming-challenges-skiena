#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define MAX_NUMBER_OF_STUDENTS  1000
#define MAX_AMOUNT_PER_STUDENT  10000.00f

int main(void)
{
    int32_t Status = 0;

    // 1 While number of students != 0 do
    // 2     read in all pairs of floats for number of students
    // 3     truncated_average := floor((sum (amounts))/students)
    // 4     distance_from_avg := sum (abs(spent_for_student - truncated_average))
    // 5     compensation := distance_from_avg/2 +/- ???
    //
    // 2.99 3.00 15.00 14.99
    uint32_t NumberOfStudents;
    while ((EOF != scanf("%d\n", &NumberOfStudents)) &&
           (0 != NumberOfStudents))
    {
        if (MAX_NUMBER_OF_STUDENTS < NumberOfStudents)
        {
            Status = 1;
            goto Error;
        }

        int32_t PenniesSpentByStudents[MAX_NUMBER_OF_STUDENTS];
        uint32_t TotalSpentPennies = 0.0f;
        for (uint32_t StudentIndex = 0;
             StudentIndex < NumberOfStudents;
             ++StudentIndex)
        {
            float NextAmountSpentDollars;
            if ((EOF == scanf("%f\n", &NextAmountSpentDollars)) ||
                (MAX_AMOUNT_PER_STUDENT < NextAmountSpentDollars))
            {
                Status = 1;
                goto Error;
            }

            PenniesSpentByStudents[StudentIndex] = (uint32_t)floorf(100.0f*NextAmountSpentDollars);

            TotalSpentPennies += PenniesSpentByStudents[StudentIndex];
        }

        int32_t TruncatedAveragePennies = TotalSpentPennies/NumberOfStudents;
        uint32_t MinPenniesExchanged = 0.0f;
        for (uint32_t StudentIndex = 0;
             StudentIndex < NumberOfStudents;
             ++StudentIndex)
        {
            MinPenniesExchanged += abs(PenniesSpentByStudents[StudentIndex] - TruncatedAveragePennies);
        }

        MinPenniesExchanged = (MinPenniesExchanged/2);

        float MinAmountExchanged = ((float)MinPenniesExchanged)/100.0f;
        printf("$%.2f\n", MinAmountExchanged);
    }

Error:
    return Status;
}
