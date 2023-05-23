#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <omp.h>

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

typedef union {
unsigned char a[128];
unsigned d[32];
unsigned long long u[16];
} AU;

#define STREAM (256 * 8)

/* short program to understand linear feed shift register
 * and its use in stream ciphers.
 */

AU lfsr(unsigned char a)
{

	unsigned char in_s, cs, cp, p, nbit, s[STREAM];
	int i, j, k = 0;

	in_s = a; // 0xb4; /* this can be any 8 bit value */
	p = 0x71; /* max length polynomial x^8+x^4+x^3+x^2+1 = 0b01110001 */

	cs = in_s; /* copy initial state */
	AU u={0};
	// printf("\nByte values for lfsr with initial value of 0xb4, and bit mask 0x71.\n");
	// printf("Should correspond to primitive polynomial x^8+x^4+x^3+x^2+1.\n");

	int kk=0;
	while (k < STREAM)
	{
		for (j = 0; j < 8; j++, k++)
		{
			cp = nbit = cs & p;

			for (i = 1; i < 8; i++)
			{ /* xor all bits together */
				nbit ^= (cp >> i);
			}
			s[k] = cs & 0x01;
			cs = (cs >> 1) | (nbit << 7); /*  rotate in new bit */
		}
		//printf(" %02x %d\n", cs,kk);
		u.a[kk%128]^=be(cs);
		kk++;
		//return cs;
		if (cs == in_s)
		{
			//printf("\nreached duplicate at %d.\n", k);
		}
	}
return u;
}

static inline uint32_t rotl32(uint32_t x, int n)
{
	// http://blog.regehr.org/archives/1063
	return x << n | (x >> (-n & 31));
}

unsigned int xor (void) {
	static unsigned int y = 2463534242;
	y = y ^ (y << 13);
	y = y ^ (y >> 17);
	return y = y ^ (y << 15);
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

typedef union {
unsigned char c[8];
unsigned short s[4];
unsigned int x[2];
unsigned long long u;
} UI;


// 思いつきで作った乱数
unsigned int period = 0, counti = 0;
UI serial(UI seed)
{
	unsigned char lfs = be(seed.c[0]); // t=g(a)
	unsigned char m = lfs;
	unsigned int kount = 0;
	UI u;

	FILE *fp;
	int i = 1;
	counti = seed.x[0]+seed.x[1]^0x12345678;
	//fp = fopen("test.bin", "wb");
	AU lfs2 = lfsr(lfs2.a[0] ^ lfs);
	while (i < 8)
	{
		counti = rotl32(counti ^ (lfs2.d[i]), counti % 32);
		kount ^= rotl32(counti ^ (lfs2.d[i+8]), counti % 32);
		i++;
	}
	u.x[0]=counti;
	u.x[1]=kount;
	return u;
}

// 半直積乱数高速版
AU slf(UI seed)
{
	unsigned char lfs = be(seed.c[0]); // t=g(a)
	unsigned char m = lfs;
	unsigned int kount = 0;
	AU u;

	FILE *fp;
	int i = 0;
	counti = seed.x[0]+seed.x[1]^0x12345678;
	//fp = fopen("test.bin", "wb");

	//for(int i=0;i<256;i++)
	//printf("%d,",lfs2.a[i]);
	//exit(1);
	AU lfs2 = lfsr(it(lfs2.a[0]) ^ lfs);
	#pragma omp pallarel 
	while (i < 256)
	{
		lfs=be(lfs2.a[i%128]);
		counti = rotl32(counti ^ (lfs), counti % 32);
		lfs ^= (p0w(loo(Dot(lfs, lfs)), (counti >> 1) + 1)); // s=(A^2r^2)^n
		lfs ^= (Dot(lfs, (loo(m) ^ be(m) ^ c)));			  // s^n(A^2t+u) = s^n(A^2t+(At+c))
		u.a[i%128]^=lfs;
		i++;
	}
	return u;
}



void main(void)
{
	UI n;
	AU m={0};
	FILE *fp=fopen("test.bin","wb");
	printf("初期値を入れてください = ");
	scanf("%llu", &n.u);
	printf("%d\n", it(be(15)));
	unsigned long long l;

	for(int i=0;i<20000000;i++)
	{
	//part 1
	//m = slf(n);
	//printf("%llu\n",m.u[i%16]);
	//fwrite(m.u,8,16,fp);
	//n.u=m.u[0];

	// part 2
	n=serial(n);
	fwrite(&n.u,8,1,fp);

	
	}
	fclose(fp);

	return;
}
