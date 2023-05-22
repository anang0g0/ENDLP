#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define N 32

unsigned char x0[N] = {21, 8, 19, 12, 2, 11, 28, 20, 7, 5, 9, 1, 22, 27, 16, 31, 18, 3, 26, 29, 17, 15, 25, 14, 30, 13, 6, 4, 10, 24, 23, 0};
unsigned char x1[N] = {10, 7, 15, 11, 14, 30, 8, 18, 20, 21, 4, 0, 31, 28, 16, 13, 24, 23, 27, 9, 12, 22, 6, 3, 1, 5, 19, 29, 26, 25, 17, 2};
unsigned char x2[N] = {31, 11, 4, 17, 27, 9, 26, 8, 1, 10, 28, 5, 3, 25, 23, 21, 14, 20, 16, 2, 7, 0, 12, 30, 29, 22, 18, 13, 6, 19, 24, 15};

#define SIZE_OF_ARRAY(array) (sizeof(array) / sizeof(array[0]))
#define SWAP(type, a, b) \
  {                      \
    type work = a;       \
    a = b;               \
    b = work;            \
  }

/*
    Fisher-Yates shuffle による方法
    配列の要素をランダムシャッフルする
*/
void random_shuffle(unsigned char *array, size_t size)
{
  for (size_t i = size; i > 1; --i)
  {
    size_t a = i - 1;
    size_t b = rand() % i;
    SWAP(int, array[a], array[b]);
  }
}

void rp(unsigned char *a)
{
  int i, j, x;
  for (i = 0; i < N; i++)
  {
    a[i] = i;
  }
  for (i = 0; i < N - 2; i++)
  {
    // rand from i+1 to N-1
    j = (rand() % (N - 1 - i)) + i + 1;

    // swap a[i] and a[j]
    x = a[j];
    a[j] = a[i];
    a[i] = x;
  }
  if (a[N - 1] == N - 1)
  {
    a[N - 1] = a[N - 2];
    a[N - 2] = N - 1;
  }
}

#define ROTL8(x, shift) ((uint8_t)((x) << (shift)) | ((x) >> (8 - (shift))))
unsigned char be(unsigned char b)
{
  return b ^ ROTL8(b, 1) ^ ROTL8(b, 2) ^ ROTL8(b, 3) ^ ROTL8(b, 4) ^ 0x63;
}

unsigned int xor (void) {
  static unsigned int y = 2463534242;
  y = y ^ (y << 13);
  y = y ^ (y >> 17);
  return y = y ^ (y << 15);
}

    unsigned long long int xor64(void)
{
  static unsigned long long int x = 88172645463325252ULL;
  x = x ^ (x << 13);
  x = x ^ (x >> 7);
  return x = x ^ (x << 17);
}

static inline uint32_t rotl32(uint32_t x, int n)
{
  // http://blog.regehr.org/archives/1063
  return x << n | (x >> (-n & 31));
}

int data()
{
  unsigned long long int i, j = 0, k = 0;
  unsigned char salt[N] = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
  //{226, 180, 26, 143, 162, 169, 124, 58, 94, 148, 232, 95, 227, 204, 18, 170, 34, 249, 221, 20, 138, 84, 147, 71, 131, 190, 225, 166, 114, 133, 31, 252};
  //

  unsigned short aa = 0;
  FILE *fp;
  unsigned char w[N] = {0};
  unsigned int cnt = 0, flg = 0;

  fp = fopen("1.bin", "wb");

  // memcpy(a.d,salt,sizeof(salt));
  while (j < 10000000)
  {

    for (i = 0; i < N; i++)
      w[i] = x0[x1[x2[i]]];

    for (i = 0; i < N; i++)
    {
      salt[i] += be(salt[w[i]]); // normal
    }

    memcpy(x1, w, sizeof(x1));
    fwrite(salt, 1, N, fp);

    j++;
  }
  fclose(fp);

  return j;
}

void mk_perm()
{
  int i;
  rp(x0);
  rp(x1);
  for (i = 0; i < N; i++)
    x2[x0[i]] = i;
  for (i = 0; i < N; i++)
    printf("%d,", x0[i]);
  printf("\n");
  for (i = 0; i < N; i++)
    printf("%d,", x1[i]);
  printf("\n");
  for (i = 0; i < N; i++)
    printf("%d,", x2[i]);
  printf("\n");
  // exit(1);
}

int main()
{
  time_t t;

  srand(clock() + time(&t));

  // mk_perm();

  int n = data();
  printf("count=%d\n", n);

  return 0;
}
