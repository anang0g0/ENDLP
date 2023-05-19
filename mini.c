#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include "lfsr.c"

unsigned char c = 0b11000110;
unsigned char v = 0b10100000;

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

unsigned char let(unsigned char c)
{
	return it(it(c));
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

unsigned int period = 0, count = 0;
unsigned char slf(unsigned char l)
{
	unsigned char lfs = l; // t=g(a)
	unsigned char m = lfs;
	FILE *fp;
	int i=0;
	fp=fopen("test.bin","wb");
	while (i<10000000)
	{
		unsigned char lfs2 = lfsr(lfs2^l);
		lfs ^= p0w(lfs^lfs2, (period) + 1); // s=(A^2r^2)^n
		lfs ^= be(Dot(lfs, (loo(m) ^ be(m) ^ c))); // s^n(A^2t+u) = s^n(A^2t+(At+c))
		++period;
		//printf("%d %d\n", lfs, period);
		fwrite(&lfs,sizeof(lfs),1,fp);
		i++;
	}

	return (unsigned char)(lfs);
}

void main(void)
{
	srand(clock());
	printf("%d\n", it(be(15)));
	unsigned char l = lfsr(v);

	l = slf(l);

	return;
}
