#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define MAX_NUMBER_OF_STUDENTS  1000
#define MAX_AMOUNT_PER_STUDENT  10000.00f

int main(void)
{
    int32_t Status = 0;

    // TODO(bwd): logic issue with steps 5-8
    // 1 While number of students != 0 do
    // 2     read in all pairs of floats for number of students
    // 3     calculate average
    // 4     compensation := (sum (abs(spent_for_student - avg))/2)
    // 5     if number of students even
    // 6         result := compensation - 0.01
    // 7     else
    // 8         result := compensation
    uint32_t NumberOfStudents;
    while ((EOF != scanf("%d\n", &NumberOfStudents)) &&
           (0 != NumberOfStudents))
    {
        if (MAX_NUMBER_OF_STUDENTS < NumberOfStudents)
        {
            Status = 1;
            goto Error;
        }

        float AmtsSpentByStudents[MAX_NUMBER_OF_STUDENTS];
        float TotalAmountSpent = 0.0f;
        for (uint32_t StudentIndex = 0;
             StudentIndex < NumberOfStudents;
             ++StudentIndex)
        {
            if ((EOF == scanf("%f\n", AmtsSpentByStudents + StudentIndex)) ||
                (MAX_AMOUNT_PER_STUDENT < AmtsSpentByStudents[StudentIndex]))
            {
                Status = 1;
                goto Error;
            }

            TotalAmountSpent += AmtsSpentByStudents[StudentIndex];
        }

        float AverageAmountSpent = TotalAmountSpent/(float)NumberOfStudents;
        float MinAmountExchanged = 0.0f;
        for (uint32_t StudentIndex = 0;
             StudentIndex < NumberOfStudents;
             ++StudentIndex)
        {
            MinAmountExchanged += fabs(AmtsSpentByStudents[StudentIndex] - AverageAmountSpent);
        }

        MinAmountExchanged = (MinAmountExchanged/2.0f) - 0.01f;

        printf("$%.2f\n", MinAmountExchanged);
    }

Error:
    return Status;
}
