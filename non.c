#include <stdio.h>
#include <stdlib.h>
// #include "vc3000.c"

#include "global-p.h"
#include "struct-p.h"
#include "lfsr.c"
typedef struct
{
	int u;
	int v;
} sem;

#define SIZE_OF_ARRAY(array) (sizeof(array) / sizeof(array[0]))
#define SWAP(type, a, b) \
	{                    \
		type work = a;   \
		a = b;           \
		b = work;        \
	}

/*
	Fisher-Yates shuffle による方法
	配列の要素をランダムシャッフルする
*/
void random_shuffle(short *array, size_t size)
{
	for (size_t i = size; i > 1; --i)
	{
		size_t a = i - 1;
		size_t b = rand() % i;
		SWAP(int, array[a], array[b]);
	}
}

typedef struct
{
	short x[N];
	short y[N];
} cem;

cem cemi(cem pi, cem tau)
{
	cem tmp = {0};

	for (int i = 0; i < N; i++)
	{
		tmp.y[i] = tau.y[pi.x[i]];
	}
	for (int i = 0; i < N; i++)
	{
		pi.y[i] += tmp.y[i];
		// pi.y[i]%=N;
	}
	for (int i = 0; i < N; i++)
		tmp.x[i] = pi.x[i];
	for (int i = 0; i < N; i++)
		tmp.y[i] = pi.y[i];

	return tmp;
}

cem kemi(cem pi, cem tau)
{
	cem tmp = {0};

	for (int i = 0; i < N; i++)
		tmp.x[tau.x[i]] = i;
	for (int i = 0; i < N; i++)
	{
		printf("tau=%d %d %d %d %d %d\n", pi.y[i], pi.y[pi.x[i]], pi.y[tau.x[i]], tau.y[i], tau.y[tau.x[i]], tau.y[pi.x[i]]);
		pi.y[i] -= tau.y[pi.x[i]];
	}

	for (int i = 0; i < N; i++)
	{
		printf("pi=%d %d %d %d\n", pi.y[i], tau.y[i], pi.x[i], tau.x[i]);
		tmp.y[i] = pi.y[i];
	}
	for (int i = 0; i < N; i++)
		tmp.x[i] = pi.x[tau.x[i]];
	// for(int i=0;i<N;i++)
	// tmp.y[i] = pi.y[i]; //[pi.x[i]];

	return tmp;
}

sem semi(sem a, sem b)
{
	sem n = {0};
	n.u = ((a.v * b.u) % 23 + a.u);
	n.v = (a.v * b.v) % 23;
	n.u %= 23;

	return n;
}

// invert of integer
int inv(int a, int n)
{
	int d, x, s, q, r, t;

	d = n;
	x = 0;
	s = 1;
	while (a != 0)
	{
		q = d / a;
		r = d % a;
		d = a;
		a = r;
		t = x - q * s;
		x = s;
		s = t;
	}
	//  gcd = d;  // $\gcd(a, n)$

	return ((x + n) % (n / d));
}

sem invs(sem a)
{
	sem s = {0};
	s.u = 23 - ((((inv(a.v, 23) * (a.u)))) % 23);
	s.v = inv(a.v, 23);
	// s.u%=23;

	return s;
}

cem invc(cem a)
{
	cem s = {0};

	for (int i = 0; i < N; i++)
		s.x[a.x[i]] = i;
	for (int i = 0; i < N; i++)
		s.y[i] = -(a.y[s.x[i]]);

	return s;
}

cem c1[3] = {0};
cem c2[3] = {0};
cem d1[3] = {0};
cem d2[3] = {0};

cem aniki(int n)
{
	if (n == 0)
		return cemi(cemi(c1[0], c1[1]), c1[2]);
	if (n == 1)
		return cemi(cemi(c1[0], c2[1]), c1[2]);
	if (n == 2)
		return cemi(cemi(c2[0], c1[1]), c2[2]);
	if (n == 3)
		return cemi(cemi(c2[0], c2[1]), c2[2]);
}
cem aniky(int n)
{
	if (n == 0)
		return cemi(cemi(d1[0], d1[1]), d1[2]);
	if (n == 1)
		return cemi(cemi(d1[0], d2[1]), d1[2]);
	if (n == 2)
		return cemi(cemi(d2[0], d1[1]), d2[2]);
	if (n == 3)
		return cemi(cemi(d2[0], d2[1]), d2[2]);
}

sem conju(sem a, sem b)
{
	return semi(semi(invs(a), b), a);
}

int kpk()
{

	sem a, b, c, d, e, f, g, h, a1, b1, c1, d1, e1, f1, g1, h1;
	a.u = 11;
	a.v = 2;
	b.u = 13;
	b.v = 4;
	c.u = 5;
	c.v = 6;
	d.u = 7;
	d.v = 8;

	e.u = 21;
	e.v = 13;
	f.u = 11;
	f.v = 12;
	g.u = 13;
	g.v = 14;
	h.u = 15;
	h.v = 16;

	sem x, y;
	int p = 17;
	sem key[4];
	sem tmp1;
	tmp1.u = 7;
	tmp1.v = 8;

	x.u = 19;
	x.v = 20;
	y.u = 21;
	y.v = 22;
	int r1 = 0b00, r2 = 0b11;
	// alice's public key
	a1 = conju(a, x);
	b1 = conju(a, y);
	// bob's public key
	c1 = conju(b, x);
	d1 = conju(b, y);

	printf("Alice's Pubkey1 = %d %d\n", a1.u, a1.v);
	printf("Alice's Pubkey2 = %d %d\n", b1.u, b1.v);
	printf("Bob's Pubkey1 = %d %d\n", c1.u, c1.v);
	printf("Bob's Pubkey2 = %d %d\n", d1.u, d1.v);

	sem tmp[16];

	key[0] = semi(semi(a1, a1), semi(b1, b1));
	key[1] = semi(semi(c1, c1), semi(d1, d1));
	printf("Alice's encrypted key = (%d,%d)\n", key[0].u, key[0].v);
	printf("Bob's encrypted key = (%d,%d)\n", key[1].u, key[1].v);
	tmp[0] = semi(semi((a), key[0]), invs(a));
	tmp[1] = semi(semi((b), key[1]), invs(b));
	printf("decrypted key-A = (%d,%d)\n", tmp[0].u, tmp[0].v);
	printf("decrypted key-B = (%d,%d)\n", tmp[1].u, tmp[1].v);

	return 0;
}

cem konju(cem a, cem b)
{

	return cemi(cemi(invc(a), b), a);
}

/*
 * S-box transformation table
 */
static const unsigned char s_box[256] = {
	// 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,	 // 0
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,	 // 1
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,	 // 2
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,	 // 3
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,	 // 4
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,	 // 5
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,	 // 6
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,	 // 7
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,	 // 8
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,	 // 9
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,	 // a
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,	 // b
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,	 // c
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,	 // d
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,	 // e
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}; // f

/*
 * Inverse S-box transformation table
 */
static const unsigned char inv_s_box[256] = {
	// 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,	 // 0
	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,	 // 1
	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,	 // 2
	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,	 // 3
	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,	 // 4
	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,	 // 5
	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,	 // 6
	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,	 // 7
	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,	 // 8
	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,	 // 9
	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,	 // a
	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,	 // b
	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,	 // c
	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,	 // d
	0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,	 // e
	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d}; // f

void f()
{
	unsigned char c[8] = {1, 1, 0, 0, 0, 1, 1, 0};
	unsigned char v[8] = {1, 0, 1, 0, 0, 0, 0, 0};
	unsigned char a[8] = {
		0b10001111,
		0b11000111,
		0b11100011,
		0b11110001,
		0b11111000,
		0b01111100,
		0b00111111,
		0b00011111};
	unsigned char b[8] = {
		0b00100101,
		0b10010010,
		0b01001001,
		0b10100100,
		0b01010010,
		0b00101001,
		0b10010100,
		0b01001010};
}

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

unsigned char ml(unsigned char c, int u)
{
	for (int i = 0; i < u; i++)
		c = loo(Dot(c, c));
	return c;
}

unsigned int period = 0, count = 0;
unsigned char lll(unsigned char l)
{
	unsigned char lfs = l; // 0xACE1u;

	int flg = 1, m=lfs, c2 = 0;
	// do
	int i=3;
	int mm =0;
	while (1)
	{
		lfs = lfsr(lfs);
		lfs = ml(lfs, period + 1);
		lfs ^= Dot(lfs, (loo(m) ^ be(m)));
		++period;
		printf("%0x %d\n", lfs, period);
		if (lfs == l){
			count++;
			if(count==1)
			mm=period;
		}
		if (count == i-1 && lfs == l)
		{	
			mm = period-mm;
			printf("m=%d\n", mm);
			i++;
			mm=period;
			if(i>100)
			 exit(1);
		}
		
	}

	return (unsigned char)(lfs);
}

void initialize_aes_sbox(uint8_t sbox[256])
{
	uint8_t p = 1, q = 1;

	/* loop invariant: p * q == 1 in the Galois field */
	do
	{
		/* multiply p by 3 */
		p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);

		/* divide q by 3 (equals multiplication by 0xf6) */
		q ^= q << 1;
		q ^= q << 2;
		q ^= q << 4;
		q ^= q & 0x80 ? 0x09 : 0;

		/* compute the affine transformation */
		uint8_t xformed = q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);

		sbox[p] = xformed ^ 0x63;
	} while (p != 1);

	/* 0 is a special case since it has no inverse */
	sbox[0] = 0x63;
}

void lf(void)
{
	// lfsr();
	// srand(clock());
	printf("%d\n", it(be(15)));
	unsigned char l = rand() % 256;
	unsigned char m;
	int count = 0, flg = 1;
	m = l;
	l = lll(l);

	return;
}

#define ROTL8(x, shift) ((uint8_t)((x) << (shift)) | ((x) >> (8 - (shift))))
void main()
{
	cem a, b, c, d, e, f, g, h;
	cem x, y;

	srand(clock());
	lf();
	exit(1);

	for (int i = 0; i < N; i++)
	{
		b.y[i] = (i + 2) % N;
		a.y[i] = i + 1;
		x.y[i] = rand() % N;
		y.y[i] = random() % N;
	}
	for (int i = 0; i < N; i++)
	{
		a.x[i] = i;
		b.x[i] = i;
		x.x[i] = i;
		y.x[i] = i;
	}
	random_shuffle(a.x, SIZE_OF_ARRAY(a.x));
	random_shuffle(b.x, SIZE_OF_ARRAY(b.x));
	random_shuffle(x.x, SIZE_OF_ARRAY(x.x));
	random_shuffle(y.x, SIZE_OF_ARRAY(y.x));

	c = konju(a, x);
	d = konju(a, y);
	g = cemi(y, x);
	h = cemi(b, invc(b));
	h = kemi(g, (x));
	e = konju(b, x);
	f = konju(b, y);

	for (int i = 0; i < N; i++)
		printf("%d, %d %d %d\n", g.y[i], y.y[i], x.y[i], h.y[i]);
	printf("\n");
	exit(1);
	g = cemi(cemi(a, c), invc(a));
	for (int i = 0; i < N; i++)
		printf("%d, %d %d %d\n", g.x[i], g.y[i], h.x[i], h.y[i]);
	printf("\n");

	return;
}