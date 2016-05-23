#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/*
   n even -> n/2
   n odd -> 3n + 1
   Given any two numbers i and j, compute maximum cycle length over all numbers between i and j (inclusive)
*/
int main(int argc, char **argv)
{
    if (3 != argc)
    {
        return 1;
    }

    int32_t FirstArg = atoi(argv[1]);
    int32_t SecondArg = atoi(argv[2]);
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

    printf("MaxCycles: %u\n", MaxCycles);

    return 0;
}
