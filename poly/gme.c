#include <stdio.h>
#include <stdlib.h>
// #include "global.h"
// #include "struct.h"
#include "mkmf.c"
#include "vc3000.c"

#define MOD 1129

typedef struct
{
	int u;
	int v;
} sem;

typedef struct
{
	vec u;
	unsigned short v;
} ham;

sem semi(sem a, sem b)
{
	sem n = {0};
	n.v = ((a.v * b.u) % MOD + b.v);
	n.u = (a.u * b.u) % MOD;
	n.v %= MOD;

	return n;
}

ham hami(ham a, ham b)
{
	ham n = {0};
	n.v = (xtrace(b.u, a.v) + b.v) % MOD;
	n.u = vadd(a.u,b.u); //vmul_2(a.u, b.u);
	n.v %= MOD;

	return n;
}

ham wow(){
	ham aa;
	aa.u=mkpol();
	aa.v=random();

	return aa;
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

vec minus(vec t)
{
	for (int i = 0; i < deg(t) + 1; i++)
		t.x[i] = MOD - t.x[i];

	return t;
}

vec koef(unsigned short a,vec u){
for(int i=0;i<deg(u)+1;i++)
u.x[i]=a*u.x[i]%MOD;

return u;
}

ham invs(ham a)
{
	ham s = {0};
	s.u = (koef(inv(a.v, MOD) , minus(a.u)));
	s.v = inv(a.v, MOD);
	// s.u%=MOD;

	return s;
}

ham conju(ham a, ham b)
{
	return hami(hami(invs(a), b), a);
}

ham c1[3] = {0};
ham c2[3] = {0};
ham d1[3] = {0};
ham d2[3] = {0};

ham aniki(int n)
{
	if (n == 0)
		return hami(hami(c1[0], c1[1]), c1[2]);
	if (n == 1)
		return hami(hami(c1[0], c2[1]), c1[2]);
	if (n == 2)
		return hami(hami(c2[0], c1[1]), c2[2]);
	if (n == 3)
		return hami(hami(c2[0], c2[1]), c2[2]);
}
ham aniky(int n)
{
	if (n == 0)
		return hami(hami(d1[0], d1[1]), d1[2]);
	if (n == 1)
		return hami(hami(d1[0], d2[1]), d1[2]);
	if (n == 2)
		return hami(hami(d2[0], d1[1]), d2[2]);
	if (n == 3)
		return hami(hami(d2[0], d2[1]), d2[2]);
}

void pham(ham a){
	printpol(a.u);
	printf(", ");
	printf(",%d\n",a.v);
}

int main()
{

	ham a, b, c, d, e, f, g, h, a1, b1, c1, d1, e1, f1, g1, h1;
	a=wow();
	b=wow();
	c=wow();
	d=wow();
	a1=wow();
	b1=wow();
	c1=wow();
	d1=wow();
	 b=invs(a);
	 c=hami(a,b);
	 pham(c);

	e=wow();
	pham(e);
	//exit(1);
	f=wow();
	g=hami(e,f);
	pham(g);
	//exit(1);
	g=wow();
	h=wow();
	e1=wow();
	f1=wow();
	//g1.v = 21;
	//g1.u = 22;
	//h1.v = 1;
	//h1.u = 6;

	int w1[3], w2[3], w3[3], w4[3];
	ham s[4] = {0};
	ham t[4] = {0};
	ham x, y;
	int p = 17;
	ham key[4];
	ham tmp1;
	tmp1=wow();

	ham aa[4];
	ham tt[3];
	ham a2, a3, b2, b3, c2, c3, d2, d3;

	x=wow();
	y=wow();
	int r1 = 0b00, r2 = 0b11;
	// alice's public key
	a1 = hami(hami(a, x), invs(b));
	a2 = hami(hami(b, x), invs(c));
	a3 = hami(hami(c, x), invs(d));
	b1 = hami(hami(a, y), invs(b));
	b2 = hami(hami(b, y), invs(c));
	b3 = hami(hami(c, y), invs(d));
	// bob's public key
	c1 = hami(hami(e, x), invs(f));
	c2 = hami(hami(f, x), invs(g));
	c3 = hami(hami(g, x), invs(h));
	d1 = hami(hami(e, y), invs(f));
	d2 = hami(hami(f, y), invs(g));
	d3 = hami(hami(g, y), invs(h));
	key[0] = aniki(r1);
	key[1] = aniky(r1);
	
	//sem ka1=semi(semi(a,x),invs(a));
	//sem ka2=semi(semi(a,y),invs(a));
	//sem kb1=semi(semi(b,x),invs(b));
	//sem kb2=semi(semi(b,y),invs(b));
	// for(int i=0;i<2;i++)
	pham(a1);
	pham(a2);
	pham(a3);
	//exit(1);

	ham tmp[16];
	tmp[0] = hami(hami(a1, a2), a3);
	tmp[1] = hami(hami(b1, b2), b3);
	tmp[2] = hami(hami(c1, c2), c3);
	tmp[3] = hami(hami(d1, d2), d3);
	printf("a1a2a3=%d %d\n", tmp[0].u, tmp[0].v);
	pham(tmp[0]);
	tmp[4] = hami(hami(x, x), x);
	printf("x^3= ");
	pham(tmp[4]);
	tmp[5] = hami(hami(invs(a), tmp[0]), d);
	printf("ans= ");
	pham(tmp[5]);
	tmp[6] = hami(hami(invs(e), tmp[2]), h);
	printf("ans2= ");
	pham(tmp[6]);
	//mkmf();
	//makefg();
	//de();

	return 0;
}
