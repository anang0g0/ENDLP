#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>


#include <stdio.h>
#include <stdlib.h>

#include "global-p.h"
#include "struct-p.h"

#define N 16
#define Pr 11

#define SIZE_OF_ARRAY(array) (sizeof(array) / sizeof(array[0]))
#define SWAP(type, a, b) \
    {                    \
        type work = a;   \
        a = b;           \
        b = work;        \
    }

unsigned short inv(unsigned short a, unsigned short n)
{
    unsigned short d = n;
    unsigned short x = 0;
    unsigned short s = 1;
    unsigned short r, t, q;

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
    // gcd = d;

    return ((x + n) % (n / d));
}

/*
    Fisher-Yates shuffle による方法
    配列の要素をランダムシャッフルする
*/
void random_shuffle(unsigned *array, size_t size)
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
} sem;

sem semi(sem pi, sem tau)
{
    sem tmp = {0};
    for (int i = 0; i < N; i++)
    {
        pi.x[i] = i;
        tau.x[i] = i;
    }
    random_shuffle(pi.x, N);
    random_shuffle(tau.x, N);
    for (int i = 0; i < N; i++)
    {
        pi.y[i] = rand() % 256;
        tau.y[i] = random() % 256;
    }
    for (int i = 0; i < N; i++)
    {
        tmp.x[i] = tau.x[pi.y[i]];
        tmp.x[i] += pi.x[i];
        tmp.y[i] = pi.y[tau.y[i]];
    }

    return tmp;
}

sem inv_sem(sem x)
{
    sem a = {0};
}

void leo()
{
    unsigned P[16][N], Q[N], inv_Q[N] = {0}, R[N] = {0};
    unsigned int z[N], y[N] = {0}, x[N] = {0}; //{137, 1832, 1763, 881, 1518, 3949, 3431, 2765, 2394, 1887, 2596, 3765, 3895, 4014, 3198, 656, 2354, 1930, 2251, 3834, 2953, 1706, 1411, 2725, 2651, 3595, 3940, 3376, 1452, 2292, 1291, 225};
    unsigned r[N] = {0};
    long long sabun = 0, s1 = 0, s3 = 0;
    unsigned sys[N] = {0}, sys2[N], sys4[N];
    unsigned pi[N] = {0}, inv_P[N], tau[N] = {0}, inv_tau[N] = {0};
    unsigned char a = 0;
    unsigned char aa[256][2560] = {0}, bb[256][256] = {0};
    // srand(clock());
    // chash();
    // int j=7;
    int j = 0;
    for (int j = 0; j < 16; j++)
    {
        for (int i = 0; i < N; i++)
            r[i] = P[j][i] = i;
        random_shuffle(P[j], SIZE_OF_ARRAY(P[j]));
    }
    for (int k = 0; k < 16; k++)
    {
        for (int ii = 0; ii < N; ii++)
            printf("%d,", P[k][ii]);
        printf("\n");
    }

    // random_shuffle(Q, SIZE_OF_ARRAY(Q));
    for (int k = 0; k < 2; k++)
    {
        for (int i = 0; i < N; i++)
            x[i] = r[P[10][i]];
        for (int j = 0; j < N; j++)
            printf("%d,", r[j]);
        printf("\n");
        memcpy(r, x, sizeof(r));
    }
    return;
}

int main1()
{
    unsigned P[N], Q[N], inv_Q[N] = {0}, R[N] = {0};
    unsigned char z[N], y[N] = {0}, x[N] = {0}; //{137, 1832, 1763, 881, 1518, 3949, 3431, 2765, 2394, 1887, 2596, 3765, 3895, 4014, 3198, 656, 2354, 1930, 2251, 3834, 2953, 1706, 1411, 2725, 2651, 3595, 3940, 3376, 1452, 2292, 1291, 225};
    unsigned r[N] = {0};
    long long sabun = 0, s1 = 0, s3 = 0;
    unsigned sys[N] = {0}, sys2[N], sys4[N];
    unsigned pi[N] = {0}, inv_P[N], tau[N] = {0}, inv_tau[N] = {0};
    unsigned char a = 0;
    unsigned char aa[256][2560] = {0}, bb[256][256] = {0};
    // srand(clock());
    // chash();
    // int j=7;
    for (int i = 0; i < N; i++)
        r[i] = Q[i] = P[i] = i;
    random_shuffle(P, SIZE_OF_ARRAY(P));
    random_shuffle(Q, SIZE_OF_ARRAY(Q));
    for (int k = 0; k < 2; k++)
    {
        for (int i = 0; i < N; i++)
            x[i] = r[P[i]];
        memcpy(r, x, sizeof(r));
        for (int j = 0; j < N; j++)
            printf("%d,", x[j]);
        printf("\n");
    }
    exit(1);

    for (int i = 0; i < N; i++)
    {
        int j = 0;
        a = i;

        while (j < 2560)
        {
            aa[i][j] = a;
            printf("%d ", a);
            // a=abox(a);
            printf("%d\n", a);
            j++;
        }
        // printf("\n");
    }

    unsigned char tmp[2] = {0};
    int count = 0;
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 2; j++)
            tmp[j] = aa[i][j];
        for (int k = 0; k < 256; k++)
        {
            for (int j = 0; j < 255; j++)
            {
                if (aa[k][j] == tmp[0] && aa[k][j + 1] == tmp[1])
                {
                    // printf("%d %d %d\n",k,aa[k][j],aa[k][j+1]);
                    bb[k][0]++;
                    break;
                }
            }
            // bb[i][1]=tmp[1];
        }
    }
    // for(int i=0;i<256;i++)
    // printf("%d %d\n",i,bb[i][0]);
    exit(1);

    /*
    for(int i=0;i<N;i++){
    r[i]=rand()%Pr;
    //x[i]=;
    }
    */
    for (int i = 0; i < N; i++)
        tau[i] = Q[i] = P[i] = i;
    random_shuffle(P, SIZE_OF_ARRAY(P));
    random_shuffle(Q, SIZE_OF_ARRAY(Q));
    random_shuffle(tau, SIZE_OF_ARRAY(tau));
    for (int i = 0; i < N; i++)
    {
        inv_Q[Q[i]] = i;
        inv_P[P[i]] = i;
        inv_tau[tau[i]] = i;
        printf("%d,", tau[i]);
    }
    // exit(1);
    unsigned t[N] = {0};
    for (int i = 0; i < N; i++)
    {
        R[tau[i]] = i;        // inv_tau
        t[i] = inv_P[tau[i]]; // tau*pi^{-1}
        printf("%d,%d\n", R[Q[i]], tau[i]);
    }
    unsigned w[N] = {0};
    printf("\n");
    // exit(1);
    for (int j = 0; j < N; j++)
    {
        sabun = 0;
        s1 = 0;
        s3 = 0;
        unsigned rr[N] = {0};
        for (int i = 0; i < N; i++)
        {
            // sabun += (x[P[i]] ^ inv(x[(i + 1) % N],N));
            y[i] = x[P[i]];             // x_pi
            w[i] = x[P[inv_tau[P[i]]]]; // x_{P*inv_tau*P}
            z[i] = r[tau[i]];           // y_pi
            rr[i] = r[P[i]];
            // printf("%d,",t[i]);
        }
        // exit(1);
        // memcpy(x, y, sizeof(x));
        // memcpy(r, z, sizeof(r));
        unsigned tt[N] = {0};
        unsigned s[N] = {0};
        unsigned u[N] = {0};
        for (int i = 0; i < N; i++)
            tt[i] = w[i] ^ rr[i]; // z
        for (int i = 0; i < N; i++)
        {
            s[i] = tt[t[i]]; // z_{tau*pi^{-1}}
            /// printf("%d,",s[i]);
        }
        for (int i = 0; i < N; i++)
            s[i] ^= y[i];
        // printf("\n");
        // exit(1);
        for (int i = 0; i < N; i++)
        {
            printf("%d %d\n", s[i], z[i]);
            sabun += s[i] ^ y[(i)];
            s1 += (r[i] ^ r[(i + 1) % N]);
            s3 += t[i];
        }
        exit(1);
        sys[j] = sabun % Pr;
        sys2[j] = s1 % Pr;
        sys4[j] = s3 % Pr;
    }
    for (int i = 0; i < N; i++)
        printf("%d,%d %d %d\n", sys[i], sys2[i], (sys[i] + sys2[i]) % Pr, sys4[i]);
    printf("\n");
}

int main()
{
    unsigned P[N], Q[N], inv_Q[N] = {0}, R[N] = {0}, Z[N] = {0};
    unsigned z[N], y[N] = {0};
    unsigned xx[N] = {0};
    unsigned r[N] = {0};
    unsigned s[N] = {0}, w[N] = {0};
    long long sabun = 0, s1 = 0, s3 = 0, s4 = 0;
    unsigned sys[N] = {0}, sys2[N] = {0}, sys4[N] = {0}, sys1[N] = {0};

    srand(clock());
    leo();
    exit(1);

    for (int i = 0; i < N; i++)
    {
        Q[i] = P[i] = i;
        xx[i] = rand() % Pr;
        printf("%d,", xx[i]);
    }
    // exit(1);
    random_shuffle(P, SIZE_OF_ARRAY(P));
    random_shuffle(Q, SIZE_OF_ARRAY(Q));
    for (int i = 0; i < N; i++)
        inv_Q[Q[i]] = i;
    for (int i = 0; i < N; i++)
    {
        R[i] = P[inv_Q[i]];
        printf("%d,%d\n", R[Q[i]], P[i]);
    }
    for (int i = 0; i < N; i++)
        Z[i] = i;
    printf("\n");
    unsigned v[N] = {0};
    for (int i = 0; i < N; i++)
    {
        printf("%d,", P[i]);
        s[i] = rand() % Pr;
        r[i] = s[i] + xx[i];
        v[i] = xx[P[i]];
    }
    printf("\n");
    unsigned t[N] = {0};
    for (int i = 0; i < N; i++)
        t[i] = s[Q[i]];
    /*
for (int i = 0; i < N; i++)
{
    r[i] = ((s[i] ^ s[(i + 1) % N]) + (x[i] ^ x[(i + 1) % N]));
    printf("%d %d\n", r[i], ((s[i] ^ s[(i + 1) % N]) + (x[i] ^ x[(i + 1) % N])));
}
printf("\n");
//exit(1);
*/
    unsigned x[N] = {0};
    for (int j = 0; j < N; j++)
    {
        sabun = 0;
        s1 = 0;
        s3 = 0;
        s4 = 0;
        for (int i = 0; i < N - 1; i++)
        {
            s3 += (s[i] ^ s[(i + 1) % N]) + (xx[i] ^ xx[(i + 1) % N]);
            s4 += (r[i] + r[(i + 1)]);
            // sabun += (xx[P[i]] ^ xx[(i + 1) % N]);
            sabun += (xx[i] + xx[(i + 1)]);
            // printf("%d,%d\n",j,xx[i]);
            s1 += (s[i] + s[(i + 1)]);

            // sabun%=Pr;
            // s1%=Pr;
            // s4%=Pr;
            // s3%=Pr;

            y[i] = xx[P[i]];
            z[i] = r[P[i]];
            w[i] = s[P[i]];
        }
        memcpy(xx, y, sizeof(xx));
        memcpy(r, z, sizeof(Z));
        memcpy(s, w, sizeof(w));
        sys[j] = (sabun);  // % Pr;
        sys2[j] = s1;      // % Pr;
        sys4[j] = s4 % Pr; // sys[j]+sys2[j] % Pr;
    }
    // exit(1);
    unsigned u[N] = {0};
    // for (int i = 0; i < N; i++)
    //     u[i] = t[R[i]];
    for (int i = 0; i < N; i++)
        printf("%d ", (sys[i] + sys2[i]) % Pr); //((t[i]+t[(i+1)%N]) ^ (x[i]+x[(i+1)%N]))%Pr);
    printf("\n");
    // for (int i = 0; i < N; i++)
    //     printf("%d ", (sys2[i])); /// sys2[i],sys[i],sys4[i]);
    // printf("\n");
    for (int i = 0; i < N; i++)
        printf("%d ", sys4[i]);
    printf("\n");
}
