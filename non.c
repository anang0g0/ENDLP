#include <stdio.h>
#include <stdlib.h>
// #include "vc3000.c"

#include "global-p.h"
#include "struct-p.h"
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
void random_shuffle(unsigned short *array, size_t size)
{
    for (size_t i = size; i > 1; --i)
    {
        size_t a = i - 1;
        size_t b = rand() % i;
        SWAP(int, array[a], array[b]);
    }
}

typedef struct {
short x[N];
short y[N];
} cem;

cem cemi(cem pi,cem tau){
cem tmp={0};
for(int i=0;i<N;i++){
    pi.x[i]=i;
    tau.x[i]=i;
}
random_shuffle(pi.x,N);
random_shuffle(tau.x,N);
for(int i=0;i<N;i++){
pi.y[i]=rand()%256;
tau.y[i]=random()%256;
}
for(int i=0;i<N;i++){
tmp.x[i]=tau.x[pi.y[i]];
tmp.x[i]+=pi.x[i];
tmp.y[i]=pi.y[tau.y[i]];
}

return tmp;
}


vec kess(){
vec x,v;

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

sem conju(sem a, sem b)
{
	return semi(semi(invs(a), b), a);
}

int main2()
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
	a1 = conju(a,x);
	b1 = conju(a,y);
	// bob's public key
	c1 = conju(b,x);
	d1 = conju(b,y);

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


void main(){
	vec a,b;
	sem x,y;

	for(int i=0;i<N;i++)
	a.x[i]=b.x[i]=i;
	random_shuffle(a.x,SIZE_OF_ARRAY(a.x));
	random_shuffle(b.x,SIZE_OF_ARRAY(b.x));

return;
}