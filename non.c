#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "global-p.h"
#include "struct-p.h"
// #include "lfsr2.c"
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

sem tdp(sem a, sem b, sem c)
{
	return semi(a, semi(b, invs(c)));
}

sem csp(sem a,sem b)
{
	return semi(semi(invs(a),b),a);
}

void psem(sem a)
{
	printf("%d, %d\n", a.u , a.v);
}

int kpk()
{
	sem a, b, c, d, e, f, g, h, a1,a2,a3, b1,b2,b3, c1,c2,c3, d1,d2,d3, e1, f1, g1, h1;
	a.u = 11;
	a.v = 2;
	b.u = 13;
	b.v = 4;
	c.u = 5;
	c.v = 6;
	d.u = 7;
	d.v = 8;

	e.u = 0;
	e.v = 4;
	f.u = 11;
	f.v = 12;
	g.u = 13;
	g.v = 14;
	h.u = 15;
	h.v = 16;

	sem x, y, zx,zy, wx,wy,zz,ww;
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
	a1 = tdp(a, x,(b));
	a2 = tdp(b, x,(c));
	a3 = tdp(c,x,(d));
	b1 = tdp(a, y,(b));
	b2 = tdp(b, y,(c));
	b3 = tdp(c, y,(d));

	// csp version
	zx=csp(a,x);
	zy=csp(a,y);
	wx=csp(b,x);
	wy=csp(b,y);
	psem(zx);
	psem(zy);
	psem(wx);
	psem(wy);
	zz=semi(zx,zy);
	ww=semi(wx,wy);
	zz=semi(semi(a,zz),invs(a));
	ww=semi(semi(b,ww),invs(b));
	psem(zz);
	psem(ww);
	//exit(1);

	// bob's public key
	c1 = tdp(e, x,(f));
	c2 = tdp(f, x,(g));
	c3 = tdp(g, x,(h));
	d1 = tdp(e, y,(f));
	d2 = tdp(f, y,(g));
	d3 = tdp(g, y,(h));
	sem pi,phi;
	phi.u=12;
	phi.v=6;
	pi=semi(d,phi);
	printf("e=%d %d\n",pi.u,pi.v);
	//exit(1);
	
	/*
	pi=semi(invs(d),a);
	phi=semi(invs(h),e);
	printf("p1=%d %d\n",pi.u,pi.v);
	printf("p2=%d %d\n",phi.u,phi.v);
	for(int i=0;i<23;i++){
	for(int j=0;j<23;j++){
	e.u=i;
	e.v=j;
	phi=semi(invs(h),e);
	if(phi.u==12 && phi.v==6){
	printf("e=%d %d\n",e.u,e.v);
	phi=semi(invs(h),e);
	printf("phi=%d %d\n",phi.u,phi.v);
	exit(1);
	}
	}
	}
	exit(1);
	*/
	printf("Alice's Pubkey1 = %d %d\n", a1.u, a1.v);
	printf("Alice's Pubkey1 = %d %d\n", a2.u, a2.v);
	printf("Alice's Pubkey1 = %d %d\n", a3.u, a3.v);
	printf("Alice's Pubkey2 = %d %d\n", b1.u, b1.v);
	printf("Alice's Pubkey2 = %d %d\n", b2.u, b2.v);
	printf("Alice's Pubkey2 = %d %d\n", b3.u, b3.v);
	printf("Bob's Pubkey1 = %d %d\n", c1.u, c1.v);
	printf("Bob's Pubkey1 = %d %d\n", c2.u, c2.v);
	printf("Bob's Pubkey1 = %d %d\n", c3.u, c3.v);
	printf("Bob's Pubkey2 = %d %d\n", d1.u, d1.v);
	printf("Bob's Pubkey2 = %d %d\n", d2.u, d2.v);
	printf("Bob's Pubkey2 = %d %d\n", d3.u, d3.v);

	sem tmp[16];
	tmp[3].u=12;
	tmp[3].v=6;
	pi=semi(invs(d),a);
	phi=semi(invs(h),e);
	printf("%d %d\n",pi.u,pi.v);
	printf("%d %d\n",phi.u,phi.v);
	//exit(1);

	key[0] = semi(semi(a1, a2), a3);
	key[1] = semi(semi(c1, c2), c3);
	printf("Alice's encrypted key = (%d,%d)\n", key[0].u, key[0].v);
	printf("Bob's encrypted key = (%d,%d)\n", key[1].u, key[1].v);
	tmp[0] = semi(semi(invs(a), key[0]), (d));
	tmp[1] = semi(semi(invs(e), key[1]), (h));
	printf("decrypted key-A = (%d,%d)\n", tmp[0].u, tmp[0].v);
	printf("decrypted key-B = (%d,%d)\n", tmp[1].u, tmp[1].v);
	tmp[5] = semi(semi(x, x), x);
	tmp[4]=semi(semi((a),key[5]),invs(d));
	key[1] = semi(semi(c1, c2), c3);
	printf("x^3=%d %d\n",tmp[5].u,tmp[5].v);
	//exit(1);
	return 0;
}

cem konju(cem a, cem b)
{

	return cemi(cemi(invc(a), b), a);
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


#define ROTL8(x, shift) ((uint8_t)((x) << (shift)) | ((x) >> (8 - (shift))))
void main()
{
	cem a, b, c, d, e, f, g, h;
	cem x, y;

	// srand(clock());
	// lf();
	kpk();
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
	// exit(1);
	g = cemi(cemi(a, c), invc(a));
	for (int i = 0; i < N; i++)
		printf("%d, %d %d %d\n", g.x[i], g.y[i], h.x[i], h.y[i]);
	printf("\n");
	kpk();

	return;
}
