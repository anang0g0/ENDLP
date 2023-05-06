#include <stdio.h>
typedef struct
{
    int u;
    int v;
} sem;

sem semi(sem a, sem b)
{
    sem n = {0};
    n.u = (a.v * b.u) + a.u;
    n.v = a.v + b.v;
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
    s.u = -inv(a.v, 23) * (a.u) % 23;
    s.v = -a.v ;

return s;
}

sem c1[3] = {0};
sem c2[3] = {0};
sem d1[3] = {0};
sem d2[3] = {0};

sem aniki(int n)
{
    if (n == 0)
    return semi(semi(c1[0],c1[1]),c1[2]);
    if (n == 1)
    return semi(semi(c1[0],c2[1]),c1[2]);
    if (n == 2)
    return semi(semi(c2[0],c1[1]),c2[2]);
    if (n == 3)
    return semi(semi(c2[0],c2[1]),c2[2]);
}
sem aniky(int n)
{
    if (n == 0)
    return semi(semi(d1[0],d1[1]),d1[2]);
    if (n == 1)
    return semi(semi(d1[0],d2[1]),d1[2]);
    if (n == 2)
    return semi(semi(d2[0],d1[1]),d2[2]);
    if (n == 3)
    return semi(semi(d2[0],d2[1]),d2[2]);
}
int main()
{

    int a[4] = {1, 2, 3, 4};
    int b[4] = {5, 6, 7, 8};
    int e[4] = {4, 3, 2, 1};
    int f[4] = {8, 7, 6, 5};
    int w1[3], w2[3], w3[3], w4[3];
    sem s[4] = {0};
    sem t[4] = {0};
    sem x, y;
    int p = 17;
    sem key[4];

    for (int i = 0; i < 3; i++)
    {
        w1[i] = inv(a[i + 1],23);
        w2[i] = inv(b[i + 1],23);
        w3[i] = inv(e[i + 1],23);
        w4[i] = inv(f[i + 1],23);
    }
    x.u = 9;
    x.v = 10;
    y.u = 11;
    y.v = 12;
    for (int i = 0; i < 4; i++)
    {
        s[i].u = a[i];
        s[i].v = b[i];
        t[i].u = e[i];
        t[i].v = f[i];
    }
    for (int i = 0; i < 3; i++)
    {
        c1[i] = semi(semi(s[i], x), invs(s[i + 1]));
        c2[i] = semi(semi(s[i], y), invs(s[i + 1]));
        d1[i] = semi(semi(t[i], x), invs(t[i + 1]));
        d2[i] = semi(semi(t[i], y), invs(t[i + 1]));
    }
    int r1=0b10,r2=0b11;
    key[0]=aniki(r1);
    key[1]=aniki(r2);
    key[2]=aniky(r1);
    key[3]=aniky(r2);
    for(int i=0;i<4;i++)
    printf("%d\n",w1[0]*key[i].u*a[3]%23);
    for(int i=0;i<4;i++)
    printf("%d\n",w2[0]*key[i].u*b[3]%23);
    for(int i=0;i<4;i++)
    printf("%d\n",w3[0]*key[i].u*e[3]%23);
    for(int i=0;i<4;i++)
    printf("%d\n",w4[0]*key[i].u*f[3]%23);

    printf("%d %d\n",semi(s[0],invs(s[0])).u%23,semi(s[0],invs(s[0])).v);

    return 0;
}