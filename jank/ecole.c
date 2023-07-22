/* generate GF(2^n) using irreducible polynomial */
//ゼフ対数表を作るためのプログラム。正規基底を生成します。


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define O 256

uint8_t gf[O],fg[O];
uint8_t sbox[O]={0};
uint8_t invs[O]={0};

#define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))

void init_sbox(uint8_t sbox[256]) {
	uint8_t p = 1, q = 1;
	
	/* loop invariant: p * q == 1 in the Galois field */
	do {
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
  printf("unsigned short sbox[%d]={",O);
for(int i=0;i<O;i++)
printf("%x,",sbox[i]);
printf("};\n");
}


/* generate Galois Field over GF(2^?) */
static const char *normal[14]={
  "1011",
  //"11001", /* GF(16) */
  "10011",
  "110111",
  "1100001",
  "11000001",
  "100011011", //AES
  //"110101001",
//"100011011",
  "1100110001",
  "11000010011",
  "110000001101",
  "1100101000001", /* 4096 */
//"11011000000001", /* 8192 */
  "10000000011011",/* Classic McEliece */
  "110000100010001",
  "1100000000000001",
  "11010000000010001"};




void makefg(int n){
int i,j;

for(i=0;i<n;i++){
  for(j=0;j<n;j++){
    if(gf[i]==j)
      fg[j]=i;
  }
}
  printf("unsigned short fg[%x]={",O);
  for(i=0;i<O;i++)
  printf("%d,",fg[i]);
printf("};\n");

 
return;
}

void isbox(){
int i,j;

for(i=0;i<O;i++){
      invs[gf[i]]=i;
  }

  printf("\nunsigned short invs[%d]={",O);
  for(i=0;i<O;i++)
  printf("%x,",invs[i]);
printf("};\n");

 
return;
}


void mkgf(int n)
{
  int i,j,bit,count=0;
  unsigned int pol,N,M,L;


  for(i=0;i<13;i++)
    pol=strtoul(normal[i],(char **)NULL,2);


  /* define pol */
  switch(n){

  case 8:
    pol=strtoul(normal[0],(char **)NULL,2);
    printf("%d\n",n);
    break;

  case 16:
    pol=strtoul(normal[1],(char **)NULL,2);
    printf("%d\n",n);
    break;

  case 32:
    pol=strtoul(normal[2],(char **)NULL,2);
printf("%d\n",n);
    break;

  case 64:
    pol=strtoul(normal[3],(char **)NULL,2);
    printf("%d\n",n);
    break;

  case 128:
   pol=strtoul(normal[4],(char **)NULL,2);
    printf("%d\n",n);
    break;

  case 256:
    pol=strtoul(normal[5],(char **)NULL,2);
    printf("%d\n",n);
    break;

  case 512:
    pol=strtoul(normal[6],(char **)NULL,2);
    printf("%d\n",n);
    break;

  case 1024:
    pol=strtoul(normal[7],(char **)NULL,2);
    printf("%d\n",n);
    break;

  case 2048:
    pol=strtoul(normal[8],(char **)NULL,2);
    printf("%d\n",n);
    break;

  case 4096:
    pol=strtoul(normal[9],(char **)NULL,2);
    printf("%d\n",n);
    break;

  case 8192:
   pol=strtoul(normal[10],(char **)NULL,2);
    printf("%d\n",n);
    break;

  default : /* 16384 */
    pol=strtoul(normal[11],(char **)NULL,2);
    printf("%d\n",n);
    break;
  }


  L=1;
  while(pol>L){
    L=(L<<1);
    count++;
  }
  L=(L>>1);
  N=pol^L;
  M=pol;
  //  printf("%u %u %u\n",N,L,M);

  gf[0]=0;
  bit=1;
  for(i=1;i<L;i++){
    if(bit>L-1){
      bit=bit-L;
      bit=bit^N;
    }
    gf[i]=bit;
    bit=(bit<<1);
  }
  printf("unsigned short gf[%d]={",O);
  for(i=0;i<L;i++)
    printf("%u,",gf[i]);
  
  printf("};\n");

}


int main(){
  int i,j,k;

init_sbox(gf);
isbox();
//exit(1);
  mkgf(O);
  makefg(O);


return 0;
}
