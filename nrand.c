#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

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

#define STREAM (256 * 8)

/* short program to understand linear feed shift register
 * and its use in stream ciphers.
 */

int k = 0;
unsigned char
lfsr(unsigned char c)
{

	unsigned char in_s, cs, cp, p, nbit, s[STREAM];
	int i, j; 

	in_s = c; /* this can be any 8 bit value */
	p = 0x71; /* max length polynomial x^8+x^4+x^3+x^2+1 = 0b01110001 */

	cs = in_s; /* copy initial state */

	// printf("\nByte values for lfsr with initial value of 0xb4, and bit mask 0x71.\n");
	// printf("Should correspond to primitive polynomial x^8+x^4+x^3+x^2+1.\n");

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
		printf(" %02x ", cs);
		return cs;
		if (cs == in_s)
		{
			printf("\nreached duplicate at %d.\n", k);
		}
		
	}
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
	unsigned char test[1000000] = {0}, t2[1000] = {0};
	int flg = 1, m = lfs, c2 = 0;
	// do
	int i = 3, ii = 0;
	int mm = 0;
	int dd = l;
	// int lfs2=l; // = lfs; //lfsr(lfs2);
	int ff = 1, flg2 = 1, ff2;
	while (1)
	{
		int lfs2 = lfsr(lfs2 + l);
		lfs = p0w(lfs2, (period) + 1); // s=(A^2r^2)^n
		// exit(1);
		lfs ^= Dot(lfs, (loo(m) ^ be(m) ^ c)); // s^n(A^2t+u) = s^n(A^2t+(At+c))
		++period;
		printf("%d %d\n", lfs, period);
		
		return lfs;

		if (lfs == l)
		{
			count++;
			if (count == 1)
				mm = period;
		}
		if (count == i - 1 && lfs == l)
		{
			if (flg == 0 && flg2 == 1)
			{
				ff2 = mm;
				flg2 = 0;
			}
			if (flg)
			{
				ff = mm;
				flg = 0;
			}
			//int k = 0, kk = 0;
			mm = period - mm;
			printf("m=%d %d\n", mm, ii);
			/*
			test[ii++] = mm;
			// if(ii>10000){
			//	printf("mbgame over\n");
			//	exit(1);
			// }
			if (ff == mm && flg == 0)
			{
				printf("ff=%d\n", mm);
				for (int i = 0; i < ii; i++)
					k += test[i];
				printf("mbike=%d\n", k);
				k = 0;
				ii = 0;
				// flg=-1;
			}
			if (ff2 == mm && flg2 == 0 && ff2 == ff)
			{
				printf("ff=%d\n", mm);
				for (int i = 0; i < ii; i++)
					kk += t2[i];
				printf("mbike2=%d\n", kk);
				exit(1);
			}
			i++;
			mm = period;
			if (i > 20000)
			{
				printf("l=%d\n", l);
				// exit(1);
			}
			*/
		}
	}

	return (unsigned char)(lfs);
}

void main(void)
{
	// srand(clock());
	printf("%d\n", it(be(15)));
	unsigned char l = rand() % 256;
	l=1;
	while(1){
	//l=lfsr(l);
	l = slf(l);
	k=0;
	}
	return;
}
