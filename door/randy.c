#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define N 32

#define ROTL8(x, shift) ((uint8_t)((x) << (shift)) | ((x) >> (8 - (shift))))
unsigned char be(unsigned char b)
{
    return b ^ ROTL8(b, 1) ^ ROTL8(b, 2) ^ ROTL8(b, 3) ^ ROTL8(b, 4) ^ 0x63;
}

typedef union {
unsigned char c[8];
unsigned long long u;
} UI ;

UI data(UI L)
{
	unsigned char x0[N] = {21, 8, 19, 12, 2, 11, 28, 20, 7, 5, 9, 1, 22, 27, 16, 31, 18, 3, 26, 29, 17, 15, 25, 14, 30, 13, 6, 4, 10, 24, 23, 0};
    unsigned char salt[N] = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    //{226, 180, 26, 143, 162, 169, 124, 1158, 94, 148, 232, 95, 227, 204, 18, 170, 34, 249, 221, 20, 138, 84, 147, 71, 131, 190, 225, 166, 114, 133, 31, 252};
	for(int i=0;i<8;i++)
	salt[i]+=L.c[i];

    //FILE *fp= fopen("1.bin", "wb");
	int j=0;

    while (j < 8)
    {

        for (int i = 0; i < N; i++)
        {
            salt[i] ^= ROTL8(be(salt[x0[i]]), salt[i] % 8); // normal
        }
        //fwrite(salt, 1, N, fp);
        L.c[j]=salt[j];
        j++;
    }
    //fclose(fp);

    return L;
}

int main()
{
    time_t t;
	UI nn={0};
	//printf("初期値を入力してください = ");
    //scanf("%llu",&nn.u);
    nn.u=time(&t)*clock();
    while(1){
    unsigned long long n = data(nn).u;
    nn.u=n;
    printf("%llu\n", n);
    }

    return 0;
}
