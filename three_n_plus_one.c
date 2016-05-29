#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_LINE_SIZE_BYTES 64

/*
   n even -> n/2
   n odd -> 3n + 1
   Given any two numbers i and j, compute maximum cycle length over all numbers between i and j (inclusive)
   */
int main(void)
{
    int32_t FirstArg;
    int32_t SecondArg;
    while (EOF != scanf("%d %d", &FirstArg, &SecondArg))
    {
        int32_t LowerNumber;
        int32_t HigherNumber;
        if (FirstArg < SecondArg)
        {
            LowerNumber = FirstArg;
            HigherNumber = SecondArg;
        }
        else
        {
            LowerNumber = SecondArg;
            HigherNumber = FirstArg;
        }

        uint32_t MaxCycles = 0;
        {
            int32_t NextInitializer;
            for (NextInitializer = LowerNumber;
                 NextInitializer <= HigherNumber;
                 ++NextInitializer)
            {
                int32_t SequenceNumber = NextInitializer;
                uint32_t CyclesForNumber = 1;
                while (1 != SequenceNumber)
                {
                    if (0x1 & SequenceNumber)
                    {
                        SequenceNumber = 3*SequenceNumber + 1;
                    }
                    else
                    {
                        SequenceNumber /= 2;
                    }

                    ++CyclesForNumber;
                }

                if (CyclesForNumber > MaxCycles)
                {
                    MaxCycles = CyclesForNumber;
                }
            }
        }

        printf("%d %d %u\n", FirstArg, SecondArg, MaxCycles);
    }

    return 0;
}
