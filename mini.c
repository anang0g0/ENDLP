#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

unsigned char c = 0b11000110;

#define ROTL8(x, shift) ((uint8_t)((x) << (shift)) | ((x) >> (8 - (shift))))
unsigned char be(unsigned char b)
{
	return b ^ ROTL8(b, 1) ^ ROTL8(b, 2) ^ ROTL8(b, 3) ^ ROTL8(b, 4) ^ 0x63;
}

unsigned char it(unsigned char s)
{
	return ROTL8(s, 1) ^ ROTL8(s, 3) ^ ROTL8(s, 6) ^ 5;
}

unsigned char loo(unsigned char c)
{
	return be(be(c));
}

#define STREAM (256 * 8)

static inline uint32_t rotl32(uint32_t x, int n)
{
	// http://blog.regehr.org/archives/1063
	return x << n | (x >> (-n & 31));
}

int Shift(int e)
{
	int shifted = (e << 1) ^ (((e & 0x80) != 0) ? 0x1B : 0x00);
	return shifted;
}

static unsigned char Dot(int a, int b)
{

	int mask = 0x1;
	int product = 0;

	while (mask != 0)
	{
		if ((b & mask) != 0)
		{
			product ^= a;
		}

		a = Shift(a);
		mask <<= 1;
	}

	return (unsigned char)product;
}

int p0w(int c, int n)
{
	int d = 1;
	while (n > 0)
	{
		if (n % 2)
			d = loo(Dot(d, c));
		// d *= c;
		c = loo(Dot(c, c));
		// c *= c;
		n >>= 1;
	}
	return d;
}

typedef union
{
	unsigned char c[8];
	unsigned short s[4];
	unsigned int x[2];
	unsigned long long u;
} UI;

unsigned int period = 0, counti = 0;
unsigned long long slf(unsigned long long l)
{
	unsigned char lfs = l % 256 + 1; // t=g(a)
	unsigned char m = lfs;
	// unsigned int kount = 0;
	UI u;

	FILE *fp;
	int i = 1;
	counti = be(lfs) ^ l;
	fp = fopen("test.bin", "wb");
	while (i < 64)
	{
		unsigned char lfs2 = l % 256 + 1; // fsr(lfs2 ^ lfs);

		counti = rotl32(counti ^ it(lfs2), counti % 32);
		// printf("%d\n",counti>>1);
		lfs ^= (p0w(loo(Dot(lfs, lfs2)), (counti >> 1) + 1)); // s=(A^2r^2)^n
		lfs ^= (Dot(lfs, (loo(m) ^ be(m) ^ c)));			  // s^n(A^2t+u) = s^n(A^2t+(At+c))
		++period;
		u.c[i % 8] ^= lfs;
		// printf("%d %d\n", lfs, period);
		// fwrite(&lfs, sizeof(lfs), 1, fp);
		i++;
	}

	return u.u;
}

void main(void)
{
	unsigned long long int n;
	unsigned long long l;

	printf("初期値を入れてください = ");
	scanf("%llu", &n);

	while (1)
	{
		l = slf(n);
		printf("%llu\n", l);
		n ^= l;
	}

	return;
}
