#include <stdio.h>
#include <stdlib.h>
typedef struct
{
    int u;
    int v;
} sem;

sem semi(sem a, sem b)
{
    sem n = {0};
    n.u = ((a.v * b.u)%23 + a.u);
    n.v = (a.v * b.v)%23;
    n.u%=23;

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
    s.u = 23-((((inv(a.v, 23) * (a.u))))%23) ;
    s.v = inv(a.v,23) ;
    //s.u%=23;

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

    sem a,b,c,d;
    a.u=1;
    a.v=2;
    b.u=3;
    b.v=4;
    c.u=5;
    c.v=6;
    d.u=7;
    d.v=8;
    int w1[3], w2[3], w3[3], w4[3];
    sem s[4] = {0};
    sem t[4] = {0};
    sem x, y;
    int p = 17;
    sem key[4];
    sem tmp1;
    tmp1.u=7;
    tmp1.v=8;

    sem aa[4];
    aa[0].u=1;
    aa[0].v=2;
    aa[1].u=3;
    aa[1].v=4;
    aa[2].u=5;
    aa[2].v=6;
    sem tt[3];
    tt[0].u=23-18;
    tt[0].v=23-4;
    tt[1].u=23-20;
    tt[1].v=23-6;
    tt[2].u=23-21;
    tt[2].v=23-8;
    sem a1,a2,a3,b1,b2,b3;

    x.u = 9;
    x.v = 10;
    y.u = 11;
    y.v = 12;
/*
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
    */
    int r1=0b00,r2=0b11;
    a1=semi(semi(a,x),invs(b));
    a2=semi(semi(b,x),invs(c));
    a3=semi(semi(c,x),invs(d));
    b1=semi(semi(aa[0],y),tt[0]);
    b2=semi(semi(aa[1],y),tt[1]);
    b3=semi(semi(aa[2],y),tt[2]);
    key[0]=aniki(r1);
    key[1]=aniky(r1);
    //for(int i=0;i<2;i++)
    printf("%d %d\n",a1.u,a1.v);
    printf("%d %d\n",a2.u,a2.v);
    printf("%d %d\n",a3.u,a3.v);
 
    sem tmp,tmp2,tmp3;
    tmp=semi(semi(a1,a2),a3);
    printf("a1a2a3=%d %d\n",tmp.u,tmp.v);
    tmp2=semi(semi(x,x),x);
    printf("x^3=%d %d\n",tmp2.u,tmp2.v);
    tmp1=semi(semi(invs(a),tmp),d);
    printf("ans=%d %d\n",tmp1.u,tmp1.v);
 
    return 0;
}