#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "struct.h"
#include "vc3000.c"

typedef struct
{
	int u;
	int v;
} sem;

typedef struct
{
	unsigned short u;
	vec v;
} ham;

sem semi(sem a, sem b)
{
	sem n = {0};
	n.v = ((a.v * b.u) % 23 + b.v);
	n.u = (a.u * b.u) % 23;
	n.v %= 23;

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
	s.v = 23 - ((((inv(a.u, 23) * (a.v)))) % 23);
	s.u = inv(a.u, 23);
	// s.u%=23;

	return s;
}

sem conj(sem a, sem b)
{
	return semi(semi(invs(a), b), a);
}

sem c1[3] = {0};
sem c2[3] = {0};
sem d1[3] = {0};
sem d2[3] = {0};

sem aniki(int n)
{
	if (n == 0)
		return semi(semi(c1[0], c1[1]), c1[2]);
	if (n == 1)
		return semi(semi(c1[0], c2[1]), c1[2]);
	if (n == 2)
		return semi(semi(c2[0], c1[1]), c2[2]);
	if (n == 3)
		return semi(semi(c2[0], c2[1]), c2[2]);
}
sem aniky(int n)
{
	if (n == 0)
		return semi(semi(d1[0], d1[1]), d1[2]);
	if (n == 1)
		return semi(semi(d1[0], d2[1]), d1[2]);
	if (n == 2)
		return semi(semi(d2[0], d1[1]), d2[2]);
	if (n == 3)
		return semi(semi(d2[0], d2[1]), d2[2]);
}
int main()
{

	sem a, b, c, d, e, f, g, h, a1, b1, c1, d1, e1, f1, g1, h1;
	a.v = 1;
	a.u = 2;
	b.v = 3;
	b.u = 4;
	c.v = 5;
	c.u = 6;
	d.v = 7;
	d.u = 8;
	a1.v = 1;
	a1.u = 2;
	b1.v = 3;
	b1.u = 4;
	c1.v = 5;
	c1.u = 6;
	d1.v = 7;
	d1.u = 8;
	// b=invs(a);
	// c=semi(a,b);
	// printf("%d %d\n",c.u,c.v);
	// exit(1);

	e.v = 9;
	e.u = 10;
	f.v = 11;
	f.u = 12;
	g.v = 13;
	g.u = 14;
	h.v = 15;
	h.u = 16;
	e1.v = 17;
	e1.u = 18;
	f1.v = 19;
	f1.u = 20;
	g1.v = 21;
	g1.u = 22;
	h1.v = 1;
	h1.u = 6;

	int w1[3], w2[3], w3[3], w4[3];
	sem s[4] = {0};
	sem t[4] = {0};
	sem x, y;
	int p = 17;
	sem key[4];
	sem tmp1;
	tmp1.v = 7;
	tmp1.u = 8;

	sem aa[4];
	aa[0].v = 1;
	aa[0].u = 2;
	aa[1].v = 3;
	aa[1].u = 4;
	aa[2].v = 5;
	aa[2].u = 6;
	sem tt[3];
	sem a2, a3, b2, b3, c2, c3, d2, d3;

	x.v = 9;
	x.u = 10;
	y.v = 11;
	y.u = 12;
	int r1 = 0b00, r2 = 0b11;
	// alice's public key
	a1 = semi(semi(a, x), invs(b));
	a2 = semi(semi(b, x), invs(c));
	a3 = semi(semi(c, x), invs(d));
	b1 = semi(semi(a, y), invs(b));
	b2 = semi(semi(b, y), invs(c));
	b3 = semi(semi(c, y), invs(d));
	// bob's public key
	c1 = semi(semi(e, x), invs(f));
	c2 = semi(semi(f, x), invs(g));
	c3 = semi(semi(g, x), invs(h));
	d1 = semi(semi(e, y), invs(f));
	d2 = semi(semi(f, y), invs(g));
	d3 = semi(semi(g, y), invs(h));
	key[0] = aniki(r1);
	key[1] = aniky(r1);
	
	sem ka1=semi(semi(a,x),invs(a));
	sem ka2=semi(semi(a,y),invs(a));
	sem kb1=semi(semi(b,x),invs(b));
	sem kb2=semi(semi(b,y),invs(b));
	// for(int i=0;i<2;i++)
	printf("%d %d\n", a1.u, a1.v);
	printf("%d %d\n", a2.u, a2.v);
	printf("%d %d\n", a3.u, a3.v);

	sem tmp[16];
	tmp[0] = semi(semi(a1, a2), a3);
	tmp[1] = semi(semi(b1, b2), b3);
	tmp[2] = semi(semi(c1, c2), c3);
	tmp[3] = semi(semi(d1, d2), d3);
	printf("a1a2a3=%d %d\n", tmp[0].u, tmp[0].v);
	tmp[4] = semi(semi(x, x), x);
	printf("x^3=%d %d\n", tmp[4].u, tmp[4].v);
	tmp[5] = semi(semi(invs(a), tmp[0]), d);
	printf("ans=%d %d\n", tmp[5].u, tmp[5].v);
	tmp[6] = semi(semi(invs(e), tmp[2]), h);
	printf("ans2=%d %d\n", tmp[6].u, tmp[6].v);

	return 0;
}
