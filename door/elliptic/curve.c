#include <stdio.h>
#include <stdlib.h>

void main()
{

int a=1;
int b=8;
int p=19;
int n=23;
int x=0;
int y=3;

            int count = 0;
            for (int i = 0; i < p; i++)
            {
                for (int j = 0; j < p; j++)
                {
                    if (((i * i) % p) == ((j * j * j + a * j + b) % p))
                    {
                        printf("%d %d %d %d %d\n", i, j, a, b, count);
                        count++;
                    }
                }
            }
            if (count == 23)
                exit(1);
}