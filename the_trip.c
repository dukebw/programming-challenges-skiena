#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define MAX_NUMBER_OF_STUDENTS  1000
#define MAX_AMOUNT_PER_STUDENT  10000.00f

int main(void)
{
    int32_t Status = 0;

    //       after bringing all students amounts to floor(avg) (students spending below) and ceiling(avg)
    //       (students above), how many pennies still need to be exchanged?
    //       Let n be # students, m total number of pennies spent, k students above avg, l students below
    //       j := m mod n is number of students whose compensated amount ends up as ceiling(avg)
    //       answer to above is abs(j - k), unless j == 0, in which case the answer is 0
    // ---
    // 1 While number of students != 0 do
    // 2     read in all pairs of floats for number of students
    // 3     average := (sum (amounts))/students)
    // 4     students_above_avg := # students' amounts above average
    // 5     pennies_mod_students := total pennies mod # students
    // 6     if (pennies_mod_students == 0)
    // 7         min_exchanged := (sum(abs(student amount - average)))/2
    // 8     else
    // 9         min_exchanged := ((sum(abs(student amount - floor (lower) or ceiling (above) average)))/2 +
    //                             abs(pennies_mod_students - students_above_avg))
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

            PenniesSpentByStudents[StudentIndex] = (int32_t)floorf(100.0f*NextAmountSpentDollars);

            TotalSpentPennies += PenniesSpentByStudents[StudentIndex];
        }

        uint32_t PenniesModStudents = TotalSpentPennies % NumberOfStudents;
        int32_t FloorAverage = TotalSpentPennies/NumberOfStudents;
        int32_t MinPenniesExchanged = 0;
        if (0 == PenniesModStudents)
        {
            for (uint32_t StudentIndex = 0;
                 StudentIndex < NumberOfStudents;
                 ++StudentIndex)
            {
                MinPenniesExchanged += abs(PenniesSpentByStudents[StudentIndex] - FloorAverage);
            }

            MinPenniesExchanged /= 2;
        }
        else
        {
            int32_t CeilAverage = FloorAverage + 1;

            int32_t StudentsAboveAverage = 0;
            for (uint32_t StudentIndex = 0;
                 StudentIndex < NumberOfStudents;
                 ++StudentIndex)
            {
                if (FloorAverage < PenniesSpentByStudents[StudentIndex])
                {
                    ++StudentsAboveAverage;
                }

                uint32_t AmountToAddToExchanged;
                if (FloorAverage >= PenniesSpentByStudents[StudentIndex])
                {
                    AmountToAddToExchanged = FloorAverage - PenniesSpentByStudents[StudentIndex];
                }
                else
                {
                    AmountToAddToExchanged = PenniesSpentByStudents[StudentIndex] - CeilAverage;
                }

                MinPenniesExchanged += AmountToAddToExchanged;
            }

            MinPenniesExchanged /= 2;
            MinPenniesExchanged += PenniesModStudents - StudentsAboveAverage;
        }

        float MinAmountExchanged = ((float)MinPenniesExchanged)/100.0f;
        printf("$%.2f\n", MinAmountExchanged);
    }

Error:
    return Status;
}
