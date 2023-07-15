#include <stdio.h>
#include <stdlib.h>
//#include "vc3000.c"

typedef struct
{
	int u;
	int v;
} sem;

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

	sem a, b, c, d, e, f, g,h,a1, b1, c1, d1, e1, f1, g1,h1;
	a.u = 1;
	a.v = 2;
	b.u = 3;
	b.v = 4;
	c.u = 5;
	c.v = 6;
	d.u = 7;
	d.v = 8;
	/*
	a1.u = 10;
	a1.v = 12;
	b1.u = 13;
	b1.v = 14;
	c1.u = 15;
	c1.v = 16;
	d1.u = 17;
	d1.v = 18;
	*/
	e.u=21;
	e.v=13;
	f.u=11;
	f.v=12;
	g.u=13;
	g.v=14;
	h.u=15;
	h.v=16;
/*
	e1.u=17;
	e1.v=18;
	f1.u=19;
	f1.v=20;
	g1.u=21;
	g1.v=22;
	h1.u=1;
	h1.v=6;
*/

	int w1[3], w2[3], w3[3], w4[3];
	sem s[4] = {0};
	sem t[4] = {0};
	sem x, y;
	int p = 17;
	sem key[4];
	sem tmp1;
	tmp1.u = 7;
	tmp1.v = 8;

	sem aa[4];
	aa[0].u = 1;
	aa[0].v = 2;
	aa[1].u = 3;
	aa[1].v = 4;
	aa[2].u = 5;
	aa[2].v = 6;
	sem tt[3];
	sem  a2, a3, b2, b3,c2,c3,d2,d3;

	x.u = 19;
	x.v = 20;
	y.u = 21;
	y.v = 22;
	int r1 = 0b00, r2 = 0b11;
	//alice's public key
	a1 = semi(semi(invs(a), x), a);
	a2 = semi(semi(b, x), invs(c));
	a3 = semi(semi(c, x), invs(d));
	b1 = semi(semi(invs(a), y), (a));
	b2 = semi(semi(b, y), invs(c));
	b3 = semi(semi(c, y), invs(d));
	// bob's public key	
	c1 = semi(semi(invs(e), x), (e));
	c2 = semi(semi(f, x), invs(g));
	c3 = semi(semi(g, x), invs(h));
	d1 = semi(semi(invs(e), y), (e));
	d2 = semi(semi(f, y), invs(g));
	d3 = semi(semi(g, y), invs(h));
	key[0] = aniki(r1);
	key[1] = aniky(r1);

	printf("%d %d\n", a1.u, a1.v);
	printf("%d %d\n", b1.u, b1.v);
	printf("%d %d\n", c1.u, c1.v);
	printf("%d %d\n", d1.u, d1.v);

	sem tmp[16];
/*
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
	tmp[7]=semi(a,invs(a));
	printf("%d %d\n",tmp[7].u,tmp[7].v);
	*/
key[0]=semi(semi(a1,b1),semi(a1,b1));
key[1]=semi(semi(c1,d1),semi(c1,d1));
	printf("%d %d\n", key[0].u, key[0].v);
	printf("%d %d\n", key[1].u, key[1].v);
tmp[0]=semi(semi((a),key[0]),invs(a));
tmp[1]=semi(semi((e),key[1]),invs(e));
	printf("%d %d\n", tmp[0].u, tmp[0].v);
	printf("%d %d\n", tmp[1].u, tmp[1].v);

	return 0;
}
