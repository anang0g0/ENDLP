// ecc.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
// jdbl,jadd�͎ˉe���W�ł��Bx=X/Z;y=Y/Z;��affine���W�ɕϊ��ł��܂��B
// 2016.2.18 ecdsa ��ǉ����܂����B

#include <NTL/ZZ.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "hash2.c"
#include "seed2.cpp"
#include "sha256.cpp"

#define _SHA256_WITH_MAIN

#ifdef _SHA256_WITH_MAIN
#include <cstdio>
#include <cstring>
#endif

#define _HAS_ITERATOR_DEBUGGING 0

#include <string.h>
#ifdef WIN32
#include <io.h>
#endif

#pragma comment(lib, "ntl.a")

NTL_CLIENT

// def M32(x) (((1 << 32)-1)&(x)) end

typedef struct
{
	ZZ x;
	ZZ y;
} PO;

ZZ KEYDEF;
ZZ KEYDEF2;
ZZ GROUP_GENERATOR;
ZZ K;

// from RFC2409
// dsa.rb Ruby version
//@GROUP_GENERATOR=2

// Default modulus value by zebedee
ZZ DFLT_MODULUS;

/*
=begin
//...If you want to abandon man-in-the-middle-attack you must exchange
//a Public key with it's own signature on DH key exchange.
//Here is a signature method NR & DSA with blind.
=end
*/

// GCD
ZZ gcd(ZZ xx, ZZ yy)
{
	ZZ tt;

	while (yy != 0)
	{
		tt = xx % yy;
		xx = yy;
		yy = tt;
	}

	return xx;
}

// invert of integer
ZZ inv(ZZ a, ZZ n)
{
	ZZ d, x, s, q, r, t;

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

ZZ pow_mod(ZZ x, ZZ n, ZZ p)
{
	if (n == 0)
		return to_ZZ("1");
	if ((n & 1) == 1)
		return (pow_mod(x, n - 1, p) * x) % p;
	x = pow_mod(x, n / 2, p);
	return (ZZ)((x * x) % p);
}

// jj=aa^bb mod oo
ZZ exp(ZZ aa, ZZ bb, ZZ oo)
{
	ZZ ii, jj, kk[8192];
	int j, c[8192], count = 0, i;
	ii = oo;
	j = 0;
	jj = 0;
	//  kk[4096]; //prime is 4096 bit table
	//  c[8192]  //mod is 8192 bit table
	count = 0;

	for (i = 0; i < 8192; i++)
	{
		kk[i] = 0;
	}
	while (ii > 0)
	{
		ii = (ii >> 1);
		j = j + 1;
	}

	kk[0] = aa;

	//  cout << j << "\n";

	// ex.1000=2**3+2**5+2**6+2**7+2**8+2**9 makes a array c=[3,5,6,7,8,9]
	for (i = 0; i < j + 1; i++)
	{
		if (bit(bb, i) != 0)
		{ // testbit(bb,i)
			c[count] = i;
			count = count + 1;
		}
	}
	//    cout << bb << endl;
	//    cout << count << "\n";
	// exit(1);
	for (i = 1; i < c[count - 1] + 1; i++)
	{
		kk[i] = kk[i - 1] * kk[i - 1] % oo;
	}

	jj = 1;
	for (i = 0; i < count; i++)
	{
		jj = kk[c[i]] * jj % oo;
		if (jj == 0)
		{
			//	print i,"\n"
		}
	}

	return jj;
}

// p>q=(p-1)/2
void dsa(ZZ m, ZZ s56)
{
	ZZ rr, a_r, a_s, Pub_key;

	cin >> K;
	// k is select atrandom
	if ((KEYDEF - 1) % K == 0)
	{
		cout << "bad k\n";
		exit(1);
	}
	rr = pow_mod(GROUP_GENERATOR, K, KEYDEF);
	a_r = rr % KEYDEF2;
	cout << a_r << "\n";
	a_s = (K * m + s56 * a_r) % KEYDEF2;
	cout << a_s << "\n";
}

// verify DSA signature
void vera(void)
{
	ZZ tt, v, q, m, a_r, a_s, Pub_key;

	cin >> Pub_key;
	cin >> m;
	cin >> a_r;
	cin >> a_s;
	//  m=sha(argc,argv);
	tt = pow_mod(GROUP_GENERATOR, a_s, KEYDEF); // g^{(km+xr)%mod}=g^km*g^xr
	q = pow_mod(Pub_key, a_r, KEYDEF);			// g^{r*x}
	q = inv(q, KEYDEF);							// g^{-xr}
	tt = tt * q % KEYDEF;						// g^{(km+xr)%mod-xr}
	m = inv(m, KEYDEF2);
	tt = pow_mod(tt, m, KEYDEF); // g^k
	v = tt % KEYDEF2;

	if (a_r != v)
	{
		cout << "baka\n";
	}
	if (a_r == v)
	{
		cout << "That's True!\n";
	}
}

void mp_init(int n)
{

	if (n == 1024)
	{
		KEYDEF = to_ZZ("101459693097608319042758415833807677670847628544715540724500026735349301068515392099767184641851031376113522183648115338953972239406909609350543459251533166139494901562515248706419170211273185584865994044063658192209029166200554581462882695234974056443041028708526103161707004900985186431045546342831775691541");
		KEYDEF2 = to_ZZ("776378143341645114049424495943980057469202238087");
		// cout << n << endl;
	}
	// 2048-bitMODPGroup
	if (n == 2048)
	{
		// DSA2-2048
		KEYDEF = to_ZZ("30981723345974435010161909523496552974699533919934322379085614661498636798097568515214479822262348814635668373190596212356380711940945934922154014218679943787759116924209774502274268759042349702152136214836347166635609098903049845488225777238895118712670994018901432361245969980840078167888393860223326555502924507123955659138424915625337173774451718035807621447511725643561397746198018458567222530032058288731896581568162423781624070371332903949092124341245711835360848611974141912660194445611902431634123654857731781595663275337139561826932358567214803181529834262455954351466450805559661991067483130544841970662773");
		KEYDEF2 = to_ZZ("87887965594022234641736188661435007486940516303779074733210399408420032394343");
	}

	// 3072-bitMODPGroup
	if (n == 3072)
	{
		// DSA2-3072
		KEYDEF = to_ZZ("3268470001596555685058361448517594259852327289373621024658735136696086397532371469771539343923030165357102680953673099920140531685895962914337283929936606946054169620100988870978124749211273448893822273457310556591818639255714375162549119727203843057453108725240320611822327564102565670538516259921126103868685909602654213513456013263604608261355992328266121535954955860230896921190144484094504405550995009524584190435021785232142953886543340776477964177437292693777245368918022174701350793004000567940200059239843923046609830997768443610635397652600287237380936753914127667182396037677536643969081476599565572030244212618673244188481261912792928641006121759661066004079860474019965998840960514950091456436975501582488835454404626979061889799215263467208398224888341946121760934377719355124007835365528307011851448463147156027381826788422151698720245080057213877012399103133913857496236799905578345362183817511242131464964979");
		KEYDEF2 = to_ZZ("93911948940456861795388745207400704369329482570245279608597521715921884786973");
	}

	// 4096-bit
	if (n == 4096)
	{
		KEYDEF = to_ZZ("FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E088A67CC74020BBEA63B139B22514A08798E3404DDEF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7EDEE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3DC2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F83655D23DCA3AD961C62F356208552BB9ED529077096966D670C354E4ABC9804F1746C08CA18217C32905E462E36CE3BE39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9DE2BCBF6955817183995497CEA956AE515D2261898FA051015728E5A8AAAC42DAD33170D04507A33A85521ABDF1CBA64ECFB850458DBEF0A8AEA71575D060C7DB3970F85A6E1E4C7ABF5AE8CDB0933D71E8C94E04A25619DCEE3D2261AD2EE6BF12FFA06D98A0864D87602733EC86A64521F2B18177B200CBBE117577A615D6C770988C0BAD946E208E24FA074E5AB3143DB5BFCE0FD108E4B82D120A92108011A723C12A787E6D788719A10BDBA5B2699C327186AF4E23C1A946834B6150BDA2583E9CA2AD44CE8DBBBC2DB04DE8EF92E8EFC141FBECAA6287C59474E6BC05D99B2964FA090C3A2233BA186515BE7ED1F612970CEE2D7AFB81BDD762170481CD0069127D5B05AA993B4EA988D8FDDC186FFB7DC90A6C08F4DF435C934063199FFFFFFFFFFFFFFFF");
	}
	if (n == 6144)
	{
		// 6192-bit
		KEYDEF = to_ZZ("FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E088A67CC74020BBEA63B139B22514A08798E3404DDEF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7EDEE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3DC2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F83655D23DCA3AD961C62F356208552BB9ED529077096966D670C354E4ABC9804F1746C08CA18217C32905E462E36CE3BE39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9DE2BCBF6955817183995497CEA956AE515D2261898FA051015728E5A8AAAC42DAD33170D04507A33A85521ABDF1CBA64ECFB850458DBEF0A8AEA71575D060C7DB3970F85A6E1E4C7ABF5AE8CDB0933D71E8C94E04A25619DCEE3D2261AD2EE6BF12FFA06D98A0864D87602733EC86A64521F2B18177B200CBBE117577A615D6C770988C0BAD946E208E24FA074E5AB3143DB5BFCE0FD108E4B82D120A92108011A723C12A787E6D788719A10BDBA5B2699C327186AF4E23C1A946834B6150BDA2583E9CA2AD44CE8DBBBC2DB04DE8EF92E8EFC141FBECAA6287C59474E6BC05D99B2964FA090C3A2233BA186515BE7ED1F612970CEE2D7AFB81BDD762170481CD0069127D5B05AA993B4EA988D8FDDC186FFB7DC90A6C08F4DF435C93402849236C3FAB4D27C7026C1D4DCB2602646DEC9751E763DBA37BDF8FF9406AD9E530EE5DB382F413001AEB06A53ED9027D831179727B0865A8918DA3EDBEBCF9B14ED44CE6CBACED4BB1BDB7F1447E6CC254B332051512BD7AF426FB8F401378CD2BF5983CA01C64B92ECF032EA15D1721D03F482D7CE6E74FEF6D55E702F46980C82B5A84031900B1C9E59E7C97FBEC7E8F323A97A7E36CC88BE0F1D45B7FF585AC54BD407B22B4154AACC8F6D7EBF48E1D814CC5ED20F8037E0A79715EEF29BE32806A1D58BB7C5DA76F550AA3D8A1FBFF0EB19CCB1A313D55CDA56C9EC2EF29632387FE8D76E3C0468043E8F663F4860EE12BF2D5B0B7474D6E694F91E6DCC4024FFFFFFFFFFFFFFFF");
	}
	if (n == 8192)
	{

		// 7.8192�r�b�gMODP�O���[�vInEnglish
		// ���̃O���[�v�ɂ́AID18�����蓖�Ă��Ă���B
		// �f���iprime�j:2^8192-2^8128-1+2^64*{[2^8062pi]+4743158}

		KEYDEF = to_ZZ("FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E088A67CC74020BBEA63B139B22514A08798E3404DDEF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7EDEE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3DC2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F83655D23DCA3AD961C62F356208552BB9ED529077096966D670C354E4ABC9804F1746C08CA18217C32905E462E36CE3BE39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9DE2BCBF6955817183995497CEA956AE515D2261898FA051015728E5A8AAAC42DAD33170D04507A33A85521ABDF1CBA64ECFB850458DBEF0A8AEA71575D060C7DB3970F85A6E1E4C7ABF5AE8CDB0933D71E8C94E04A25619DCEE3D2261AD2EE6BF12FFA06D98A0864D87602733EC86A64521F2B18177B200CBBE117577A615D6C770988C0BAD946E208E24FA074E5AB3143DB5BFCE0FD108E4B82D120A92108011A723C12A787E6D788719A10BDBA5B2699C327186AF4E23C1A946834B6150BDA2583E9CA2AD44CE8DBBBC2DB04DE8EF92E8EFC141FBECAA6287C59474E6BC05D99B2964FA090C3A2233BA186515BE7ED1F612970CEE2D7AFB81BDD762170481CD0069127D5B05AA993B4EA988D8FDDC186FFB7DC90A6C08F4DF435C93402849236C3FAB4D27C7026C1D4DCB2602646DEC9751E763DBA37BDF8FF9406AD9E530EE5DB382F413001AEB06A53ED9027D831179727B0865A8918DA3EDBEBCF9B14ED44CE6CBACED4BB1BDB7F1447E6CC254B332051512BD7AF426FB8F401378CD2BF5983CA01C64B92ECF032EA15D1721D03F482D7CE6E74FEF6D55E702F46980C82B5A84031900B1C9E59E7C97FBEC7E8F323A97A7E36CC88BE0F1D45B7FF585AC54BD407B22B4154AACC8F6D7EBF48E1D814CC5ED20F8037E0A79715EEF29BE32806A1D58BB7C5DA76F550AA3D8A1FBFF0EB19CCB1A313D55CDA56C9EC2EF29632387FE8D76E3C0468043E8F663F4860EE12BF2D5B0B7474D6E694F91E6DBE115974A3926F12FEE5E438777CB6A932DF8CD8BEC4D073B931BA3BC832B68D9DD300741FA7BF8AFC47ED2576F6936BA424663AAB639C5AE4F5683423B4742BF1C978238F16CBE39D652DE3FDB8BEFC848AD922222E04A4037C0713EB57A81A23F0C73473FC646CEA306B4BCBC8862F8385DDFA9D4B7FA2C087E879683303ED5BDD3A062B3CF5B3A278A66D2A13F83F44F82DDF310EE074AB6A364597E899A0255DC164F31CC50846851DF9AB48195DED7EA1B1D510BD7EE74D73FAF36BC31ECFA268359046F4EB879F924009438B481C6CD7889A002ED5EE382BC9190DA6FC026E479558E4475677E9AA9E3050E2765694DFC81F56E880B96E7160C980DD98EDD3DFFFFFFFFFFFFFFFFF");
	}
}

int dsa2(int argc, char *argv[])
{

	int n, i, j, read_size;
	FILE *fp;
	char m[32], s[32];
	ZZ xx, s56, s128, s64, s168, gay, yy, md, Pub_key;
	ZZ ee;

	// fp = fopen("key.bin", "rb");

	cout << "decide MODP size n" << endl;
	// cin >> n;
	n = 3072;

	mp_init(n);
	//  cout << "n=" << n << "\n";

	if (n != 1024 && n != 2048 && n != 3072)
	{
		cout << "You can select from next parametors 1024,2048\n";
		cout << "3072. First [./dsa file] Next you need to input\n";
		cout << "one of these dsa key size.\n";
		cout << "Note this version's signature is not secure because of \n";
	}

	// default:print"Chooseyourkeysize","\n"

	// chooserandomsecretkeys(largerthans128-bit)
	s128 = to_ZZ("340282055605427970793192457633571187455");
	s168 = to_ZZ("373779281620604392915322569464050882390986703503105");
	// fread(m, 1, 32, fp);
	// read_size = fread(s, 1, 32, fp);
	//  cout<<read_size<<endl;

	gay = 0;
	for (i = 0; i < 32; i++)
		gay = (gay << 8) ^ s[i];
	// to_ZZ("3618502788666131106986593281527206111185510860325500770110842981586944655359");

	// cout<<"kissecretnumberwhichissenderchoosedatrandom"<<endl;

	//=to_ZZ("22387645283764583276458327645823764528376458327645832764528376452837645238764528376453287645");
	// freopen("con","r",stdin);

	GROUP_GENERATOR = pow_mod(gay, (KEYDEF - 1) / KEYDEF2, KEYDEF);
	if ((KEYDEF - 1) % KEYDEF2 != 0)
		exit(1);

	yy = pow_mod(GROUP_GENERATOR, gay, KEYDEF); // PublicKey
	cout << yy << endl;
	Pub_key = yy;
	if (n == 1024)
	{
		ee = sha2(argc, argv);
	}
	else // if(strcmp(argv[1],"3"))
		md = sha2(argc, argv);
	cout << md << endl;
	// printf("%08X%08X%08X%08X%08X\n",ee.MD[0],ee.MD[1],ee.MD[2],ee.MD[3],ee.MD[4]);
	// e=("0x"+STATUS).hex
	/*
	if(n==1024){
	md=to_ZZ(ee.MD[0]);
	md=(md<<32)^to_ZZ(ee.MD[1]);
	md=(md<<32)^to_ZZ(ee.MD[2]);
	md=(md<<32)^to_ZZ(ee.MD[3]);
	md=(md<<32)^to_ZZ(ee.MD[4]);
	}
	*/
	// cout<<argv[1]<<"'sDSASignatureis\n";
	if (!strcmp(argv[1], "1"))
		dsa(md, gay);
	if (!strcmp(argv[1], "2"))
		vera();

	return 0;
}

NTL_CLIENT

int A = 0;
int B = 0;

typedef struct
{
	ZZ x;
	ZZ y;
	int f;
} po;

typedef struct
{
	ZZ x;
	ZZ y;
	ZZ z;
} ten;

typedef struct
{
	ZZ v;
	po u;
} sem;
typedef struct
{
	int v;
	int u;
} kem;

typedef struct
{
	char *name;
	ZZ p;
	ZZ seedE;
	ZZ r;
	ZZ a;
	ZZ b;
	po G;
	ZZ n;
	int h;
	ZZ inv;
} cv;
typedef struct
{
	unsigned int MD[8];
} arr;

typedef struct
{
	po G[2][2];
} ellmat;

cv CRV;
po le[640];
po ll[640];
po Pub_key;
// po T; // eadd
// po D; // edbl
//  ten P;       // ead2
//  ten Q;       // edl2
po N[640];	 // le_temp
po M[640];	 // ll_temp
po Gen_key; // randompoint
ZZ a3[100000];
int a2[100000], a4[100000];
//,pp=0;
// chargg[32];

// poPub_key;
// unsignedlonglongintg3[256][2],g4[256][2];
// intE_K=0;
ZZ I[9000];

void init_curve(int n)
{

	if (n == 224)
	{
		//"{CBD61FC1-F50D-4","P-224","p=2^224-2^96+1",
		CRV.name = "eccp224";
		CRV.p = to_ZZ("26959946667150639794667015087019630673557916260026308143510066298881");
		CRV.seedE = to_ZZ("0xbd71344799d5c7fcdc45b59fa3b9ab8f6a948bc5");
		CRV.r = to_ZZ("0x5b056c7e11dd68f40469ee7f3c7a7d74f7d121116506d031218291fb");
		CRV.a = to_ZZ("-3");
		CRV.b = to_ZZ("0xb4050a850c04b3abf54132565044b0b7d7bfd8ba270b39432355ffb4");
		CRV.G.x = to_ZZ("0xb70e0cbd6bb4bf7f321390b94a03c1d356c21122343280d6115c1d21");
		CRV.G.y = to_ZZ("0xbd376388b5f723fb4c22dfe6cd4375a05a07476444d5819985007e34");
		//CRV.G.z = to_ZZ("1");
		CRV.n = to_ZZ("26959946667150639794667015087019625940457807714424391721682722368061");
		CRV.h = 1;
		CRV.inv = to_ZZ("13479973333575319897333507543509815336778958130013154071755033149441");
	}

	if (n == 2244)
	{
		CRV.n = to_ZZ("13479973333575319897333507543509819484712548793029295142314934474707");
		// doyouwanttoproceedwiththisone(Y/N)?y
		CRV.a = to_ZZ("-3");
		CRV.b = to_ZZ("7926735752311977820224618556926799352231714653617375950103029803860");
		CRV.p = to_ZZ("26959946667150639794667015087019630673557916260026308143510066298881");
		// R=13479973333575319897333507543509819484712548793029295142314934474707a224bitprime
		CRV.G.x = to_ZZ("24155712282000049631001357595395450813097367499938434330297713128801");
		CRV.G.y = to_ZZ("5882461958218329457915762297197751647381159972043925920846779622521");
		//CRV.G.z = to_ZZ("1");
		CRV.inv = to_ZZ("13479973333575319897333507543509815336778958130013154071755033149441");
	}

	if (n == 256)
	{
		//"{B858801C-582E-4","P-256","p=2^256-2^224+2^N+2^96-1",
		CRV.name = "eccp256";
		CRV.p = to_ZZ("115792089210356248762697446949407573530086143415290314195533631308867097853951");
		CRV.seedE = to_ZZ("1122468115042657169822351801880191947498376363664");
		CRV.r = to_ZZ("57436011470200155964173534038266061871440426244159038175955947309464595790349");
		CRV.a = to_ZZ("-3");
		CRV.b = to_ZZ("41058363725152142129326129780047268409114441015993725554835256314039467401291");
		CRV.G.x = to_ZZ("48439561293906451759052585252797914202762949526041747995844080717082404635286");
		CRV.G.y = to_ZZ("36134250956749795798585127919587881956611106672985015071877198253568414405109");
		//CRV.G.z = to_ZZ("1");
		CRV.n = to_ZZ("115792089210356248762697446949407573529996955224135760342422259061068512044369");
		CRV.h = 1;
		CRV.inv = to_ZZ("57896044605178124381348723474703786765043071707645157097766815654433548926976");
	}

	if (n == 2563)
	{
		CRV.name = "eccp256r2";
		CRV.p = to_ZZ("115792089210356248762697446949407573530086143415290314195533631308867097853951");
		CRV.a = to_ZZ("115792089210356248762697446949407573530086143415290314195533631308867097853948");
		CRV.b = to_ZZ("41058363725152142129326129780047268409114441015993725554835256314039467401291");
		CRV.G.x = to_ZZ("395815829005855038029765540278861637762572903522963440114216832740821793555094");
		CRV.G.y = to_ZZ("30769603638631994634479180612906047718205418376298768584198191172839256571736");
		//CRV.G.z = 1;
		CRV.n = to_ZZ("115792089210356248762697446949407573529996955224135760342422259061068512044369");
	}

	if (n == 2562)
	{

		CRV.a = to_ZZ("20549773689227812627292831297245718700081937423295476612608601227386727850553");
		CRV.b = to_ZZ("47623312769896854212861199545761181316171213330096761101722264837667685936281");
		CRV.n = to_ZZ("101770390931234937383997936042792106547711735705722570460955764669295036711143");
		CRV.p = to_ZZ("101770390931234937383997936042792106548349765143699330079949592058229602107737");
		CRV.G.y = to_ZZ("16845429027015099874700368857878626645060314647744100515127489968869022719070");
		// 84924961904219837509297567184913479903289450495955229564822102089360579388667");
		CRV.G.x = to_ZZ("38996611774847114330982137792567050762097251424678060141486085082902623877233");
		//
		// w=59
		//CRV.G.z = 1;

		/*
		CRV.name="eccp256r1";
		CRV.p=to_ZZ("115792089210356248762697446949407573530086143415290314195533631308867097853951");
		CRV.a=to_ZZ("115792089210356248762697446949407573530086143415290314195533631308867097853948");
		CRV.b=to_ZZ("41058363725152142129326129780047268409114441015993725554835256314039467401291");
		CRV.seedE=to_ZZ("1122468115042657169822351801880191947498376363664");
		CRV.G.x=to_ZZ("395815829005855038029765540278861637762572903522963440114216832740821793555094");
		CRV.G.y=to_ZZ("3600254559212783631020592788373957154744735408555473783510957625074434451321");
		//59240149723730851812311272008153533141816437726696305166080887910572446982458012615605924465554721774228350039774385670824392793454808075284049927019123189");
		CRV.G.z=1;
		CRV.n=to_ZZ("115792089210356248762697446949407573529996955224135760342422259061068512044369");
		CRV.h=1;
		CRV.inv=to_ZZ("57896044605178124381348723474703786765043071707645157097766815654433548926976");
		*/
	}

	if (n == 2564)
	{
		CRV.a = to_ZZ("325379667707489826401064228424852592079356807922648328779606654148301710111");
		CRV.b = to_ZZ("43638953242465233168106495530541200510616011708075374527275181032002171872628");
		CRV.n = to_ZZ("65133050195990359925758679067386948173334237203809005766938102982707303781623");
		CRV.p = to_ZZ("65133050195990359925758679067386948173844660754190413462133164893854956098831");
		// infinity devide1
		CRV.G.x = to_ZZ("18487703388218368811611273467885185415041771995400161457848598846868890320470");
		CRV.G.y = to_ZZ("24687222602040641145870602721247005746364795831134621092877670314906490902760");
		//CRV.G.z = 1;
	}

	if (n == 2565)
	{
		CRV.a = to_ZZ("50615278656685320126791612967404283968036974455733654200179080720363497751491");
		CRV.b = to_ZZ("33743519104456880084527741978269522645357982970489102800119387146908998500994");
		CRV.n = to_ZZ("65133050195990359925758679067386948208553462180126136735397374851843822399097");
		CRV.p = to_ZZ("65133050195990359925758679067386948209063885730507544430592436762991474716443");
		// infinity devide1
		CRV.G.x = to_ZZ("63582577761502127973450815015383738377077716646935254059256424901755563321273");
		CRV.G.y = to_ZZ("22895574053820059180945744963571496712404540419433716833791593749823092772659");
		//CRV.G.z = 1;
	}

	if (n == 2566)
	{

		CRV.a = to_ZZ("58619423044276027832903787065716086236095439918520509851335164825122761913928");
		CRV.b = to_ZZ("5156151719105706093936546029546688635963459581307566382625017622890757039103");
		CRV.n = to_ZZ("101770390931234937383997936042792106563662471655141560935801449392638687313903");
		CRV.p = to_ZZ("101770390931234937383997936042792106564300501093118320554795276781573252710547");
		CRV.G.y = to_ZZ("98399506821057818865862588903161792587668472549669649472059793708337005935795");
		CRV.G.x = to_ZZ("28663290192706932057771677870920156363737430016916930158558409220429370107804");
		//CRV.G.z = 1;
	}

	if (n == 384)
	{
		// Grouporder=
		CRV.n = to_ZZ("5628858028056354173182720014306230543582819895780778095420563755345515056616034903125147135939193286284507539411263");
		CRV.a = -3;
		CRV.b = to_ZZ("7480836070348502192310144960168319822230417316926886013721948420378391982808980728605112950230878688086816085055381");
		CRV.p = to_ZZ("39402006196394479212279040100143613805079739270465446667948293404245721771497210611414187026722401376469034446356971");
		// R=5628858028056354173182720014306230543582819895780778095420563755345515056616034903125147135939193286284507539411263a382bitprime
		CRV.G.x = to_ZZ("13839325240143548436618097820237988853510646421495011597727751118065256776775190573162986203269450299582183630276103");
		CRV.G.y = to_ZZ("33479584122550753431257252642915332781925124384694439200422942083526531640009189112108669026825242814943349686581134");
		//CRV.G.z = 1;
	}

	if (n == 64)
	{
		CRV.a = to_ZZ("3491397999065334299");
		CRV.b = to_ZZ("3864827429762923759");
		CRV.n = to_ZZ("4611686295453070103");
		CRV.p = to_ZZ("4611686291158102679");
		CRV.G.x = to_ZZ("0");
		CRV.G.y = to_ZZ("3694866613525431056");
		//CRV.G.z = to_ZZ("1");
	}
	if (n == 16)
	{
		CRV.a = to_ZZ("2");
		CRV.b = to_ZZ("17");
		CRV.n = to_ZZ("41");
		CRV.p = to_ZZ("31");
		CRV.G.x = to_ZZ("10");
		CRV.G.y = to_ZZ("13");
	}
	if (n == 3)
	{
		CRV.a = to_ZZ("1");
		CRV.b = to_ZZ("8");
		CRV.p = to_ZZ("19");
		CRV.n = to_ZZ("23");
		CRV.G.x = to_ZZ("3");
		CRV.G.y = to_ZZ("0");
		//CRV.G.z = to_ZZ("1");
	}
}
ZZ PLO[2][2];
void niji(ZZ ISR[2][2])
{
	ZZ p, q, tmp;
	// CRV.p=to_ZZ("5");
	p = (CRV.p + ISR[0][0] * ISR[1][1] - ISR[1][0] * ISR[0][1]) % CRV.p;
	p = inv(p, CRV.p);
	cout << p << endl;
	// exit(1);
	tmp = ISR[0][0];
	ISR[0][0] = ISR[1][1];
	ISR[1][1] = tmp;
	ISR[0][1] = (CRV.p - ISR[0][1]) % CRV.p;
	ISR[1][0] = (CRV.p - ISR[1][0]) % CRV.p;
	PLO[0][0] = (ISR[0][0] * p) % CRV.p;
	PLO[0][1] = (ISR[0][1] * p) % CRV.p;
	PLO[1][0] = (ISR[1][0] * p) % CRV.p;
	PLO[1][1] = (ISR[1][1] * p) % CRV.p;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
			cout << PLO[i][j] << endl;
	}
	// cout << PLO[1][1] << endl;
	// exit(1);
}

ZZ isqrt(ZZ x)
{
	ZZ t, s;

	/*
	if(x==0)
	return0;
	*/
	s = 1;
	t = x;
	while (s < t)
	{
		s <<= 1;
		t >>= 1;
	}
	while (s < t)
	{
		t = s;
		s = (x / s + s) >> 1;
	}

	return t;
}

// 有限体の元の逆数
unsigned short
qinv(unsigned short a)
{
	int i;

	if (a == 0)
		return 0;

	for (i = 0; i < 521; i++)
	{
		if (gf[mlt(fg[a], i)] == 1)
			return (unsigned short)i;
	}

	printf("no return \n");
	//  exit (1);
}

// 有限体の元の逆数
unsigned short
oinv(unsigned short a)
{
	int i;

	if (a == 0)
		return 0;

	return ((256 - fg[a]) % (256 - 1)) + 1;
}

// aに何をかけたらbになるか
unsigned short
equ(unsigned short a, unsigned short b)
{
	int i;

	for (i = 0; i < 521; i++)
	{
		if (gf[mlt(fg[a], fg[i])] == b)
			break;
	}
	return i;
}


// 0335196068
po eadd(po A, po B)
{
	po e;
	ZZ x3, y3, q, ll;
	ZZ x1, x2, y1, y2;
	x1 = A.x;
	x2 = B.x;
	y1 = A.y;
	y2 = B.y;
	po T;
	ZZ mod = CRV.p;
	if (y1 * y1 % mod != (x1 * (x1 * x1 + CRV.a) + CRV.b) % mod)
	{
		printf("no point X1\n");
		cout << x1 << "\n";
		cout << y1 << "\n";
		exit(1);
	}

	if (y2 * y2 % mod != (x2 * (x2 * x2 + CRV.a) + CRV.b) % mod)
	{
		printf("no point X2\n");
		exit(1);
	}

	if (x1 == x2)
	{
		printf("infinity devide1\n");
		cout << "Y1=" << y1 << "\n";
		cout << "Y2=" << y2 << "\n";
		if (y1 * y1 % mod == (x1 * (x1 * x1 + CRV.a) + CRV.b) % mod)
		{
			cout << "X1 on Curve\n";
		}
		if (y2 * y2 % mod == (x2 * (x2 * x2 + CRV.a) + CRV.b) % mod)
		{
			cout << "X2 on Curve\n";
		}
		// cout << x1 << "\n";
		// V = 0;
		e.f = 2;
		return e;
		// exit(1);
		//  return 2;
	}

	// lamda
	if (y2 > y1)
	{
		y3 = y2 - y1;
	}
	if (y2 < y1)
	{
		y3 = mod + (y2 - y1);
	}

	if (x2 > x1)
	{
		x3 = x2 - x1;
	}
	if (x2 < x1)
	{
		x3 = mod + (x2 - x1);
	}

	if (y3 == 0)
	{
		printf("Y2=0 is ord=2 !\n");
		cout << "X1=" << x1 << "," << y1 << "\n";
		cout << "X2=" << x2 << "," << y2 << "\n";
	}
	if (x3 == 0)
	{
		printf("X2=0\n");
		cout << x1 << "\n";
		cout << x2 << "\n";
	}

	q = inv(x3, mod);

	// lambda
	ll = y3 * q % mod;
	if (ll < 0)
	{
		cout << "L is - " << ll << "\n";
		exit(1);
	}

	x3 = (ll * ll - x1 - x2) % mod;
	if (x3 > 0)
	{
		x3 = x3 % mod;
	}
	if (x3 < 0)
	{
		while (x3 < 0)
		{
			x3 = x3 + mod;
		}
	}

	if (x1 > x3)
	{
		y3 = (ll * (x1 - x3) - y1) % mod;
	}
	if (x1 < x3)
	{
		y3 = (ll * (mod + x1 - x3) - y1) % mod;
	}
	if (x1 == x3)
	{
		y3 = mod - y1;
	}
	if (y3 < 0)
	{
		printf("Y<0\n");
		y3 = mod + y3 % mod;
	}

	e.x = x3;
	e.y = y3;
	if (y3 * y3 % mod != (x3 * (x3 * x3 + CRV.a) + CRV.b) % mod)
	{
		printf("in eadd no point at\n");
		cout << x1 << "\n";
		cout << x2 << "\n";
		cout << y1 << "\n";
		cout << y2 << "\n";
		exit(1);
	}

	T.x = x3 % mod;
	T.y = y3 % mod;

	return T;
}

po edbl(ZZ x1, ZZ y1)
{
	int ii;
	po de;
	ZZ ll, x, y, x3, y3;
	ZZ mod = CRV.p;
	ZZ a = CRV.a;
	po D;

	if (y1 * y1 % CRV.p != (x1 * (x1 * x1 + CRV.a) + CRV.b) % CRV.p)
	{
		printf("no point at edbl\n");
		exit(1);
	}

	ii = 1;
	x = 3 * x1 * x1 + a;
	y = 2 * y1;
	x3 = inv(y, mod);
	ll = x3 * x % mod;
	x = ll * ll % mod;

	if (x > 2 * x1)
	{
		x3 = x - (2 * x1 % mod);
	}
	if (x < 2 * x1)
	{
		x3 = mod + x - (2 * x1 % mod);
	}
	de.x = x3 % mod;
	x3 = x3 % mod;

	if (x1 > x3)
	{
		y3 = ll * (x1 - x3);
	}
	if (x1 < x3)
	{
		y3 = ll * (mod + x1 - x3);
	}
	if (y3 < 0)
	{
		cout << y3 << "\n";
		exit(1);
	}
	if (y3 > y1)
	{
		y = y3 - y1;
	}
	if (y3 < y1)
	{
		y = y3 - y1 + mod;
	}

	de.y = y % mod;
	if (de.y * de.y % CRV.p != (de.x * (de.x * de.x + CRV.a) + CRV.b) % CRV.p)
	{
		printf("in edbl no point at\n");
		cout << de.x << "\n";
		exit(1);
	}
	if (x3 == 0 && y3 * y3 % mod == (x3 * (x3 * x3 + CRV.a) + CRV.b) % mod)
	{
		cout << "hakken!\n";
		cout << x3 << "\n";
		cout << y3 << "\n";
		exit(1);
	}

	D.x = de.x % CRV.p;
	D.y = de.y % CRV.p;

	return D;
}


ten p2t(po a)
{
	ten b;
	b.x = a.x;
	b.y = a.y;
	b.z = to_ZZ("1");

	return b;
}

po Qmlt(po y, ZZ n)
{
	po ret, tmp1, x;
	int flg = 0, count = 0, first = 0;
	unsigned char bin[640] = {0};
	ZZ nn;
	po Z;

	nn = n;
	while (nn > 0)
	{
		if (nn % 2 == 1)
			flg++;
		// printf("%d", nn % 2);
		count++;
		nn >>= 1;
	}
	// printf("\n");
	nn = n;
	// exit(1);
	x.x = y.x;
	x.y = y.y;

	ret = x;
	x = edbl(x.x, x.y);
	// x = D;

	if (n == 1)
	{
		Z.x = ret.x;
		Z.y = ret.y;
		//Z.z = 1;
		return ret;
	}
	if (n == 2)
	{
		Z.x = x.x;
		Z.y = x.y;
		//Z.z = 1;
		return x;
	}
	// n>>=1;
	while (n > 0 && nn > 1)
	{
		// ret=x;

		n >>= 1;
		if ((n & 1) == 1 && nn > 2)
		{
			ret = eadd(ret, x); // n の最下位bitが 1 ならば x^(2^i) をかける
								// ret = T;
								//  if(n==1)
								//  return ret;
		}
		// break;
		// cout << "n================ " << n << endl;
		// n>>=1;
		if (first == 0 && n % 2 == 1 && nn % 2 == 0)
		{
			ret = x;
			first = 1;
		}

		x = edbl(x.x, x.y);
		// x = D;

		// n >>= 1; // n を1bit 左にずらす
		// cout << n << "," << flg << "," << x.x << "," << x.y << endl;
		if (n == 2 && flg == 1)
			return x;
	}
	Z.x = ret.x;
	Z.y = ret.y;
	//Z.z = 1;
	return ret;
}


////make a calcration table from base point Q
int mktable(ZZ x, ZZ y)
{
	ZZ pp, xx, yy, a;
	int i, j, k;

	cout << "in.mktable\n";

	pp = CRV.p;
	xx = x;
	yy = y;
	a = CRV.a;

	if ((x * x * x + x * a + (CRV.b)) % pp != (yy * yy) % pp)
	{
		printf("x is nor point\n");
		cout << xx << "\n";
		cout << yy << "\n";
		// return -1;
	}

	// enzan table
	le[0].x = xx;
	le[0].y = yy;

	for (i = 1; i < 521; i++)
	{ //} //begin Pub_key at plain
		if (yy == 0)
		{
			cout << "Y ga " << i << "\n";
		}

		if ((xx * (xx * xx + CRV.a) + CRV.b) % CRV.p != (yy * yy) % CRV.p)
		{
			printf("x is n0r point\n");
			cout << i << "\n";
			exit(1);
		}

		le[i] = edbl(xx, yy);
		// le[i].x = D.x;
		// le[i].y = D.y;

		if (le[i].x > pp)
		{
			printf("X > mod\n");
			exit(1);
		}
		if (le[i].y > pp)
		{
			printf("Y > mod\n");
			exit(1);
		}
		xx = le[i].x;
		yy = le[i].y;
	} // end of for

	return 0;
}

// Compute Q=mG
po ellip(ZZ k)
{
	int ki[521];
	ZZ ll;
	po s;
	int j = 0, i;
	int l = 0;
	int tt1 = 0;
	po CNK;

	if (k < 0)
	{
		printf("obsense K\n");
		cout << k << "\n";
		exit(1);
	}

	if (k == 0)
	{
		cout << "mD is 0\n";
		s.x = 0;
		s.y = 0;
		exit(1);
		// return s;
	}

	if (k > CRV.n)
	{
		k = k % CRV.n;
	}
	for (j = 0; j < 521; j++)
	{
		ki[j] = 0;
	}
	ll = k;
	while (ll > 0)
	{
		ll = (ll >> 1);
		l = l + 1;
	}
	ll = k;
	if (j != 0)
	{
		j = 0;
	}
	//  if(((kk^(1<<i))>>i)%2 == 0) //testbit(kk,i) <- shippai
	for (i = 0; i < l; i++)
	{
		if (ll % 2 == 1)
		{
			ki[j] = i;
			j = j + 1;
		}
		ll = (ll >> 1);
	}
	CNK.x = le[ki[0]].x;
	CNK.y = le[ki[0]].y;

	if (k > 0)
	{
		for (i = 1; i < j; i++)
		{
			if (CNK.x != le[ki[i]].x || CNK.y != le[ki[i]].y)
			{
				//      printf("doko1\n");
				if (CNK.y * CNK.y % CRV.p != (CNK.x * CNK.x * CNK.x + CRV.a * CNK.x + CRV.b) % CRV.p)
				{
					printf("in ellip no point at\n");
					cout << i << " eeee\n";
					cout << CNK.x << "\n";
					exit(1);
				}
				if (le[ki[i]].y * le[ki[i]].y % CRV.p != (le[ki[i]].x * le[ki[i]].x * le[ki[i]].x + CRV.a * le[ki[i]].x + CRV.b) % CRV.p)
				{
					cout << ki[i] << "\n";
					printf("no point in ellip\n");
					exit(1);
				}
				if (CNK.y * CNK.y % CRV.p == (CNK.x * CNK.x * CNK.x + CRV.a * CNK.x + CRV.b) % CRV.p)
				{
					//	 printf("doko2\n");
					if (le[ki[i]].y * le[ki[i]].y % CRV.p == (le[ki[i]].x * le[ki[i]].x * le[ki[i]].x + CRV.a * le[ki[i]].x + CRV.b) % CRV.p)
					{
						//	    printf("doko3\n");
						CNK = eadd(CNK, le[ki[i]]);
						/*
						CNK.x = tt1.x;
						CNK.y = tt1.y;
						if (tt1 == 2)
						{
						  printf("doko4 %d\n", tt1);
						  exit(1);
						}
						*/
					}
					if (le[ki[i]].y * le[ki[i]].y % CRV.p != (le[ki[i]].x * le[ki[i]].x * le[ki[i]].x + CRV.a * le[ki[i]].x + CRV.b) % CRV.p)
					{
						printf("no point ellip at\n");
						cout << le[ki[i]].x << "\n";
						exit(1);
					}
				}
			}

			if (CNK.x == le[ki[i]].x && CNK.y == le[ki[i]].y)
			{
				edbl(CNK.x, CNK.y);
				cout << "dbl(" << CNK.x << "," << CNK.y << ")\n";
				exit(1);
			}
			//     cout << "in for loop " << i << " ";
		}

		if (CNK.y * CNK.y % CRV.p != (CNK.x * CNK.x * CNK.x + CRV.a * CNK.x + CRV.b) % CRV.p)
		{
			cout << "bad " << i << "\n";
			printf("j=%d\n", j);
			for (int jj = 0; jj < j + 1; jj++)
			{
				cout << ki[jj] << " ";
			}
			printf("\n");
			exit(1);
		}
		/*
		if (Pub_key.y * Pub_key.y % CRV.p == (Pub_key.x * Pub_key.x * Pub_key.x + CRV.a * Pub_key.x + CRV.b) % CRV.p){
		  //     cout << "Public key = (" << Itoa(Pub_key.x,16) << "," << Itoa(Pub_key.y,16) << ")" << }l;
		  //       print "in ellip of x=", T.x,"\n"
		  //       print "in ellip of y=", T.y,"\n"
		}
		*/
		return CNK;
	}

	// no return --  print "after return\n"
	exit(1);
}



/*=begin
ECDSAsignatureenerationandverification
Tosignameddahem,anentryAwithdomainparametersD=(q,FR,a,b,G,n,h)
andassociatedkeypair(d,Q)doesthefollowing

1.Selectarandomorpseudorandomintegerk,1<=k<=n-1
2.ComputekG=(x1,y1)andr=x1modn.Ifr=0thengotostep1.
3.Computek^{-1}modn.
4.Computee=sha1(m)
5.Computes=k^{-1}(e+dr)modn.Ifs=0thengotostep1.
6.A'ssignatureforthemessagemis(r,s).
=end
*/
po rs;
po ecdsa(ZZ k, ZZ e, ZZ d)
{
	ZZ r, s, L;
	po U, P;
	int i, count = 0, cnp = 0;
	po Q;
	po V;

	cout << "in_ecdsa\n";

	// init_curve(256);
	// mktbl3(CRV.G);
	// mktable(CRV.G.x, CRV.G.y);

	if (k <= 0)
	{
		cout << "kisobsenseinecdsa\n";
		exit(1);
	}
	if (k > 0)
		cout << k << endl;
	//  P=elp3(k);
	V = Qmlt(CRV.G, k);
	// exit(1);

	// cout << Pub_key.x << endl;
	// P=mltn(CRV.G,k);

	// exit(1);

	// U=elp(k,CRV.G);
	// #print"Ex=",e_x,"\n";
	r = V.x % CRV.n;
	// cout << r, " in ecdsa\n";
	k = inv(k, CRV.n);
	// #e=sha(argc,argv);
	s = (k * (e + d * r)) % CRV.n;
	cout << s << endl;
	// #print"k=",k,"\n"
	// #print"d=",d,"\n"
	// #print"e=",e,"\n"
	rs.x = s;
	rs.y = r;
	// cout << s << "," << r;
	// exit(1);

	return rs;
}

/*
=begin
ECDSAsignaturevelification
1.Verifythatrandsareintegersintheinterval[1,n-1]
2.Computee=sha1(m)
3.Computew=s^{-1}modn.
4.Computeu1=ewmodnandu2=rwmodn.
5.ComputeX=u1G+u2Q.IfX=O,thenrejectthesignature.Otherwise,compute
v=x1modnwhereX=(x1,y1)
6.Acceptthesignatureifandonlyifv=r.
=end
*/
void vr_ecdsa(ZZ s, ZZ r, ZZ e)
{
	ZZ w, u1, u2, gx, gy, gz, qx, qy, qz, xx, yy, v, zz, L;
	FILE *fp;
	unsigned char key[32 * 3];
	ZZ I, px, py, pz;
	int i, count = 0, cnp = 0;
	po TT, G1, G2, R, P;
	po A, B;
	po Ga, Gb, X;

	if (r == 0)
	{
		cout << "r=0\n";
		exit(1);
	}
	cout << "in_vr_ecdsa\n";
	fp = fopen("eccpub.key", "rb");
	fread(key, 1, sizeof(key), fp);
	fclose(fp);

	I = 0;
	px = py = pz = 0;
	for (i = 0; i < 32; i++)
	{
		I = key[i];
		px ^= I << (i * 8);
	}
	for (i = 0; i < 32; i++)
	{
		I = key[i + 32];
		py ^= I << (i * 8);
	}
	for (i = 0; i < 32; i++)
	{
		I = key[i + 64];
		pz ^= I << (i * 8);
	}
	TT.x = px;
	TT.y = py;
	//TT.z = pz;

	// #e=sha(argc,argv);
	w = inv(s, CRV.n);
	u1 = (e * w) % CRV.n;
	// #cout<<"u1="<<u1<<endl;
	u2 = (r * w) % CRV.n;

	cout << "Gen_key\n";

	// mktbl3(CRV.G);
	// mktable(CRV.G.x, CRV.G.y);
	if (u1 > 0)
		X = Qmlt(CRV.G, u1);
	//  X = ellip(u1);
	// P = elp3(u1); //#G=
	/*
		L=r;
	  while(L>0){
		if(L%2==0)
		count++;
		cnp++;
		L=(L>>1);
	  }
	  cout << count << " =count , cnp= " << cnp << endl;
	  if(count==cnp-1){
		D=le[count];
		cout << ":" << P.x << endl;
		//exit(1);
	  //return P;
	  }
	*/

	// R=elp(u1,CRV.G);
	// cout << P.x << endl;
	// cout << P.y << endl;
	// P=mltn(CRV.G,u1);
	Ga = X;
	// G1 = P;
	//  cout << u1 << endl;
	cout << P.x << endl;
	cout << P.y << endl;
	// exit(1);

	// mktbl3(TT);
	// mktable(TT.x, TT.y);
	if (u2 > 0)
		X = Qmlt(TT, u2);
	// X = ellip(u2);
	// P = elp3(u2); //#Q=
	/*
	  L=u2;
	  while(L>0){
		if(L%2==0)
		count++;
		cnp++;
		L=(L>>1);
	  }
	  cout << count << " =count , cnp= " << cnp << endl;
	  if(count==cnp-1){
		D=le[count];
		cout << ":" << A.x << endl;
		//exit(1);
	  //return Q;
	  }
	*/
	// cout << Pub_key.x << endl;
	// cout << Pub_key.y << endl;
	// A=mltn(TT,u2);
	cout << X.x << endl;
	cout << X.y << endl;
	// cout << u2 << endl;
	// exit(1);

	// R=elp(u2,T);
	Gb = X;
	// G2 = P;

	if (Ga.x == Gb.x && Ga.y == Gb.y)
	{
		cout << "equalpoint\n";

		// return G1;
		exit(1);
	}

	if (Ga.x != Gb.x)
	{
		xx = eadd(Ga, Gb).x; // #X=
							 // xx = T.x;
							 // yy = T.y;
							 //  zz = P.z;
	}
	v = xx % CRV.n;
	cout << "ris = " << r << "\n";
	cout << "vis = " << v << "\n";

	if (v == r)
		cout << "That'strue!\n";

	if (v != r)
		cout << "baka\n";
}

void genkey(unsigned char str[256], unsigned char t[256])
{
	FILE *fp, *fq;
	unsigned char *filename;
	unsigned char buf[256];
	int i;

	printf("genkey\n");
	/*
	if(fp=fppen("key.bin","rb")!=NULL){
	cout<<"���Ɍ��t�@�C�������݂��܂��B�V�������t�@�C��������͂��Ă��������B\n";
	cin>>filename;
	}
	*/
	// cout<<str<<endl;
	cout << t << endl;

	hasha(t);
	fp = fopen("seed.key", "wb");
	fwrite(c1.dd, 1, 16, fp);
	fwrite(c2.dd, 1, 16, fp);
	hasha(str);
	fwrite(c1.dd, 1, 16, fp);
	fwrite(c2.dd, 1, 16, fp);
	cout << "���L��seed.key����������܂����B" << endl;
	fclose(fp);
}

void ecdh()
{
	ZZ ax, ay, az, b, cx, cy, cz, bf[32], I, key;
	FILE *fp, *fq;
	unsigned char buf[32], buff[4], str[32 * 3], tt[256];
	int i, j, nn, II[32 * 3];
	po X, P;
	po tmp;

	ax = to_ZZ("17861087610823746018273460812734601283746210837");
	cx = to_ZZ("32987203987420893742039874203987402983740293847290387");
	// fp=fopen("key.bin","rb");
	// fread(buf,1,32,fp);
	// fclose(fp);
	init_curve(256);
	// mktbl3(CRV.G);
	/*
	if(fp==NULL){
	  std::cout << "���t�@�C����������܂���B" << std::endl;
	 exit(1);
	}
	*/
	cout << ax << endl;
	tmp = Qmlt(CRV.G, ax);
	Pub_key.x = tmp.x;
	Pub_key.y = tmp.y;
	//Pub_key.z = 1;
	// cout << "Alice's Pubkey " << Pub_key.x << "," << Pub_key.y << "," << Pub_key.z <<endl;
	// elp3(ax);
	cout << "Alice's Pubkey " << Pub_key.x << "," << Pub_key.y << "," << endl;
	// exit(1);
	// mktbl3(Pub_key);
	// elp3(cx);
	// cout << "Bob's Shared key " << Pub_key.x << "," << Pub_key.y << "," << Pub_key.z <<endl;
	tmp = Qmlt(Pub_key, cx);
	Pub_key.x = tmp.x;
	Pub_key.y = tmp.y;
	//Pub_key.z = 1;
	cout << "Bob's Shared key " << Pub_key.x << "," << Pub_key.y << ","  << endl;

	// mktbl3(CRV.G);
	// elp3(cx);
	// cout << "Bob's Pubkey " << Pub_key.x << "," << Pub_key.y << "," << Pub_key.z << endl;
	tmp = Qmlt(Pub_key, cx);
	Pub_key.x = tmp.x;
	Pub_key.y = tmp.y;
	//Pub_key.z = 1;
	cout << "Bob's Pubkey " << Pub_key.x << "," << Pub_key.y << ","  << endl;

	// mktbl3(Pub_key);
	// elp3(ax);
	// cout << "Alice's Shared key " << Pub_key.x << "," <<  Pub_key.y << "," << Pub_key.z <<endl;
	tmp = Qmlt(CRV.G, cx);
	Pub_key.x = tmp.x;
	Pub_key.y = tmp.y;
	//Pub_key.z = 1;
	cout << "Alice's Shared key " << Pub_key.x << "," << Pub_key.y << "," << endl;
	// exit(1);
	/*
	fp = fopen("key.bin", "rb");
	if (fp == NULL)
	{
	  printf("no key\n");
	  exit(1);
	}
	fread(buf, 1, 32, fp);
	fclose(fp);
	init_curve(256);
	mktbl3(CRV.G);
	if (fp == NULL)
	{
	  cout << "no key" << endl;
	  exit(1);
	}
	I = key = 0;
	for (i = 0; i < 32; i++)
	{
	  I = buf[i];
	  key ^= I << (i * 8);
	}
	elp3(key);
	X = P;
	cout << "baka\n";
	cout << X.x << endl;
	cout << X.y << endl;

	for (i = 0; i < 32; i++)
	{
	  bf[i] = X.x % 256;
	  X.x = (X.x >> 8);
	  conv(II[i], bf[i]);
	}
	for (i = 0; i < 32; i++)
	{
	  bf[i] = X.y % 256;
	  X.y = (X.y >> 8);
	  conv(II[i + 32], bf[i]);
	}
	for (i = 0; i < 32; i++)
	{
	  bf[i] = X.z % 256;
	  X.z = (X.z >> 8);
	  conv(II[i + 64], bf[i]);
	}
	for (i = 0; i < 32 * 3; i++)
	{
	  str[i] = II[i];
	  // cout<<to_ZZ(str[i]);
	}
	cout << "�F�؃p�X���[�h��ݒ肵�܂�\n";
	// freopen("con","r",stdin);
	scanf("%s", &tt);
	cout << "in_" << tt << endl;
	hasha(tt);

	// exit(1);

	genkey(str, tt);
  */
	// lk(tt);
	// fclose(fp);
}

void keygen(ZZ r)
{
	// ZZr;
	FILE *fp;
	unsigned char c[32 * 3];
	int i;

	fp = fopen("eccpub.key", "wb");
	init_curve(256);
	//mktbl3(CRV.G);
	// cin>>r;
	Qmlt(CRV.G,r);
	cout << "y=" << Pub_key.y << endl;

	for (i = 0; i < 32; i++)
	{
		c[i] = Pub_key.x % 256;
		Pub_key.x = Pub_key.x >> 8;
	}
	for (i = 0; i < 32; i++)
	{
		c[i + 32] = Pub_key.y % 256;
		Pub_key.y = Pub_key.y >> 8;
	}

	fwrite(c, 1, sizeof(c), fp);
	fclose(fp);
	for (i = 0; i < 32 * 3; i++)
		c[i] = 0;
	for (i = 0; i < 32; i++)
	{
		c[i] = r % 256;
		r = (r >> 8);
	}
	fp = fopen("eccsec.key", "wb");
	fwrite(c, 1, 32, fp);
	fclose(fp);

	exit(1);
}

void enc(char *argv[], int nn)
{
	FILE *fp, *fq, *fr;
	ZZ salt, buf[32], aa, II;
	int i, read_size, ii, c, k, a, j, count = 0, cnt = 0;
	unsigned int I[32];
	unsigned char buff[SIZE], str, *e, m[32], ai[32], key[32 * 3];
	unsigned char *to;
	po X, P;

	init_curve(nn);
	salt = sha2(2, argv);
	// cin>>salt;

	if (sizeof(argv) == 0)
		cout << "usage: inputfile outfile\n";

	fp = fopen(argv[1], "rb");
	fq = fopen(argv[2], "wb");

	// mktbl3(CRV.G);
	// elp3(salt);
	Pub_key = Qmlt(CRV.G,salt);
	cout << "y=" << Pub_key.y << endl;

	for (i = 0; i < 32; i++)
	{
		key[i] = Pub_key.x % 256;
		Pub_key.x = Pub_key.x >> 8;
	}
	for (i = 0; i < 32; i++)
	{
		key[i + 32] = Pub_key.y % 256;
		Pub_key.y = Pub_key.y >> 8;
	}
	/*
	for (i = 0; i < 32; i++)
	{
		key[i + 64] = Pub_key.z % 256;
		Pub_key.z = Pub_key.z >> 8;
	}
	*/
	fwrite(key, 1, sizeof(key), fq);

	fr = fopen("eccpub.key", "rb");
	fread(key, 1, sizeof(key), fr);
	fclose(fr);

	Pub_key.x = 0;
	Pub_key.y = 0;
	//Pub_key.z = 0;
	for (i = 0; i < 32; i++)
	{
		II = key[i];
		Pub_key.x ^= II << (i * 8);
	}
	for (i = 0; i < 32; i++)
	{
		II = key[i + 32];
		Pub_key.y ^= II << (i * 8);
	}

	cout << "py=" << Pub_key.y << endl;
	for (i = 0; i < 32; i++)
	{
		II = key[i + 64];
		//Pub_key.z ^= II << (i * 8);
	}

	//mktbl3(Pub_key);
	//elp3(salt);
	Qmlt(Pub_key,salt);
	//  X=elp(salt,Pub_key);

	X.x = Pub_key.x;
	X.y = Pub_key.y;
	//X.z = Pub_key.z;

	// X=elp(salt,Pub_key);
	cout << "y=" << X.y << endl;

	//  memset(I,0,sizeof(I));
	// memset(buf,0,sizeof(buf));
	// memset(ai,0,sizeof(ai));

	while ((read_size = fread(buff, 1, 32, fp)) > 0)
	{
		for (i = 0; i < 32; i++)
		{
			I[i] = 0;
			buf[i] = 0;
			ai[i] = 0;
		}
		for (i = 0; i < read_size; i++)
			ai[i] = buff[i];

		salt = 0;
		for (i = 0; i < read_size; i++)
		{
			II = to_ZZ(ai[i]);
			salt ^= II << (i * 8);
		}
		//

		cout << salt << endl;
		//jadd(X, CRV.G); // ��Â���
		salt ^= P.y;

		//    exit(1);

		X.x = P.x;
		X.y = P.y;
		//X.z = P.z;

		for (i = 0; i < 32; i++)
		{
			buf[i] = salt % 256;
			conv(I[i], buf[i]);
			salt = (salt >> 8);
		}

		for (i = 0; i < 32; i++)
		{
			m[i] = (unsigned char)I[i];
			if (m[i] != I[i])
			{
				cout << "baka in enc\n";
				exit(1);
			}
		}

		fwrite(m, 1, read_size, fq);
		//        exit(1);
	}

	fclose(fp);
	fclose(fq);
}

void dec(char *argv[], int nn)
{
	ZZ salt, buf[32], a, II, r;
	po R, P;
	unsigned char buff[32], ai[32], key2[32], key[32 * 3];
	FILE *fp, *fq, *fr;
	int read_size, i, j, count = 0, cnt = 0;
	unsigned int I[32];
	unsigned char m[32];

	printf("indec\n");
	init_curve(nn);
	fr = fopen("eccsec.key", "rb");
	fread(key2, 1, 32, fr);
	fclose(fr);

	r = 0;
	for (i = 0; i < 32; i++)
	{
		II = key2[i];
		r ^= (II << (8 * i));
	}
	// cin>>Pub_key.x;
	// cin>>Pub_key.y;
	// cin>>Pub_key.z;

	R.x = 0;
	R.y = 0;
	//R.z = 0;
	fp = fopen(argv[2], "wb");
	fq = fopen(argv[1], "rb");
	fread(key, 1, 32 * 3, fq);
	for (i = 0; i < 32; i++)
	{
		II = key[i];
		R.x ^= II << (i * 8);
	}
	for (i = 0; i < 32; i++)
	{
		II = key[i + 32];
		R.y ^= II << (i * 8);
	}

	Qmlt(R,r);
	//    Pub_key=elp(r,R);

	// cout<<"dcPubkey.x="<<Pub_key.x<<"\n";
	//   cout<<"dcPubkey_y="<<Pub_key.y<<"\n";
	// cout<<"dcPubkey.z="<<Pub_key.z<<"\n";
	// salt=0;

	while (read_size = fread(buff, 1, 32, fq))
	{
		for (i = 0; i < 32; i++)
		{
			buf[i] = 0;
			m[i] = 0;
		}
		// cout<<"r="<<read_size<<endl;

		salt = 0;
		for (i = 0; i < read_size; i++)
		{
			// cout<<to_ZZ(buff[i])<<endl;
			II = to_ZZ(buff[i]);
			salt ^= II << (i * 8);
		}
		// exit(1);
		// salt^=to_ZZ(buff[read_size-1]);

		// padding
		a = salt;
		while (a > 0)
		{
			j++;
			a = (a >> 8);
		}

		P=eadd(Pub_key, CRV.G);
		salt ^= P.y; //%(256*read_size));
		//cout << salt << endl;

		//    exit(1);
		a = salt;
		Pub_key.x = P.x;
		Pub_key.y = P.y;
		//Pub_key.z = P.z;

		for (i = 0; i < 32; i++)
			I[i] = 0;
		for (i = 0; i < read_size; i++)
		{
			m[i] = salt % 256;
			// conv(I[i],buf[i]);
			salt = (salt >> 8);
		}

		//    exit(1);

		fwrite(m, 1, read_size, fp);

		// printf("dec=%d\n",read_size);
	}

	fclose(fp);
	fclose(fq);
}

void nizk()
{
	ZZ r, c, x, b;

	Qmlt(CRV.G,r);
	// elp(r,CRV.G);
	cout << "inputchallenge\n";
	cin >> c;
	b = c - r * x;
}

/*
defotp(x,nn)

init_curve(nn)

r=[0xf874f8444567019dae5d0686dd04f2615204d38875190df1f54af6,0xe814985c80ee06915a0ab4580dd00fe97b37dc2e84110ff5fd6f7a,0xc854f064ce120797a3b76389d309f9711c5cbc51a7140cfbfd2efb,0xe814d03c85dc089feaf00ce85d805f4415c30c467201a0cfcf2f8ef,0xf834d8648cdd0f925ac0fcf84d80bfd190364c59c6e1d06fafc9fed,0xc844a82c9d580f9eca708ef8ada0bfe19547195809f190af9f3d863,0xf04ce82c58030f97da0d1188fdd0cf421035ec231911b03f5f2da40,0xd024f8442d2c009ba497998cd301f1713b1ea021ad1808fcf226e4,0xc87c800463fa0b965aa983c8bd90efc712331a2dc4d1506fcf29a5b,0x9864801c5d0706973a8de1885dd08f671635af27299130af1f26348,0xc82ca0442d2e039ada09b9580db03fb311b35ab2c0d1f0afff8db2e,0x9024f05431370f96dab2caf81d504fe415346894a511b09f1f70d22,0xd01ca824540f08989aad31c80d607f497b2396401c1509f7f4d2af,0x901cb04c4fc5059ccafe75980d20cf961137eb957a91a0ef5f824f5,0xe02cd00c54c6059e7ab4e3885d203f3213301a55590180cfafa2c57,0xa07cb0745d040591ca4dd1c89d109f1390461fa641a100df8f0ece3,0xc034b05c4863039f5a3570b8bdb03ff614b75b24f04110bf1f11075,0x9844f00488df0b96a807ce84d005fc39232ac5dda11b01f2fb3198,0xe82c905c5a0204978a2dd1388d600f5495418fe17c2150cf8f32e3b,0x903cf8442c270497aa694988dda00f5393b20d0db1e1e04f2fe7b0c,0xe05c903c81df0e9e4a10fcb88d208f5412c24bf43cc1102fef7aae2,0x800cb0045a0605909aad11980df0efc513b219a2d291e0cfef0ff6a,0xf034e83caf5d05912af6cf189db05f9313b33958c7e1f09f0f7f425,0xd02c887cc4130d9c7acb16e85db00f1614334fec5b41101f1f74d85,0x9004a86457c20e9aca44e3a8cd90ef0293b3eac8192190bf8f2da66,0xe864d8143b310d96fa629a385d304fc692b0ed578191402f4f89fb0,0x904c800c570a0896bacd81d86db04f4194369c995fb1a04fdfbfb03,0xd85cf0048ade0898fabb51385d904fa395424dc9ac51503f0fec3a0,0xe86ca8544ecd079faace75781da05f2291427be39931b0ffff0d841,0xf87c986c21210892ead9b9482d80df6494376b091c01e02fcfac7aa,0xd83c8014313b0b901aa20a68ed90df1115c3fd919df1b0df4fc193f,0x883c805ca45c04901a969f384dd01ff690c22b845cb1f0df4f8a9ef,0x8864906c48ce02994a3e55f83d807fa793b01957a5e1900f0fefa8a,0xe0149834446706988a1590b89d30dfb710c3cd7adf91108fffc9c17,0xb02cf0744ec40c9bca7e45280de09f26742aae5fa51002fff82aff,0xa864e07c66f703946aa9b328bd904f871033184602e130ef7fefbe5,0xb824c87494f305966abb42680dd0ef0695c6ce234d1506fdfad977,0x8044f02c5c000c9b4a0d41884de07f495416ec7f901301f6f06f4c,0xd87c887ccb8904974a6981b87d009fa313b34cb4e6a160efaf97097,0x885cb8642d210197ba5939086d804fb69035cae252d160bf3f1f067,0xc074880c80d80b99dab0ec48ed107f31437cad8da1701f0f78483,0x986cb82c4d6c0c968a551078fd20df6614b33b44db01302f5f64ba3,0xe87cb0748bd70098da1b01883db0ff571230feed8321e0cf0f1767a,0xd82cd81c4fc80c92ba8e65788d604ff492b0cdcec141e0df5f4cec2,0x9824987c35350897fa523a287d008f3790356905e601502f9f3b3df,0xf064b054252907975ae999582d907f3690b70b286761007fbf0f920,0xa004c06c81dc029dda10fcf8ed905f31532c9eacd71b0ef3f75cc8,0xa06cd84c310c0d9c9a5dc248bd40af3292b569b47bf1802f1fc49a0,0xe834a0245bc6079f9a44d3d83dd09f1316c1fd645691f04f2fe14ba,0xd86cf83c2f260d9cbac949681d705f1712c42f861501701f8f269dd];
c=[0x8014b87450cd0a96fac4b3389d504f6297b1dd23d51101fcf38a80,0x9044a0145c0b099d8a0d7128ad708f02963039499f31a06f6f7fd08,0xa054e86480e00f9e4a4ad478cd909f7396b04ef8e7e160dfcf9fe2d,0xe01c882487da0b948aebe1c87d30bf7390c63d9ea13150dfbf5cfcb,0xc034882c58c90692fa9493085d308fc311316d34fc11600ffffaae1,0x8024d07c3a360c99ac2ca08ed60bf41e1c49f292f01807fdf3f80a,0xa804d02c406a07982a85d0281d401fa514b6cd7a8d61f00f2f555cc,0xb864b07ca9560495a56af08dd20af3510b36f3887d1e08f5fc4af0,0xd064c06c86e70b959abad4481db0af4795c6fb5ed041709f5f69ab2,0x907c90148ce60e95eaea24b80d80ef731631ac925011b0af8f3d627,0xb854c86c390c01904a8d12989d809fa795b4ec18a98140df8ffda6e,0x8054d0244cc50b919abe7558bde05f4394c36ed5da41d07fff8acc7,0xe05c984c446307954a4500c82dd0cf4314b218bd34a1109f8f14bc6,0xa824b03480e20497ca0af418cd704f079436793e3b2140cfef8fc45,0xa004f0448bd90896a60dc785d90cf293b6cd881771b02f6fc47fc,0xa034e8145a050e9a3a3d81a8ed108f649241fc4268c1c0cf4f26771,0xf82c90043935029f2ab21a282da01f2513b15c599641405f3f39ee2,0xa874e81c2528009f9a69e928ad909f5110b31c24c1d1c0ff4f5dba8,0x807c884c80d401935a506ca8fd10ff1792b06a9ab0d1707f1fad1d4,0x883cb83c6ef400968ac9e308bd80ff2914169394e21504fef3a413,0x987cb86c89de069dea5bc158bda00fd590b498f55a91e0df1f721f9,0xd85cc04c340d08999a3d22b81d309f7197b69ec65521a05f0f968d0,0x885cc8744b6900994ae5a0382d80ff2695b45c36717150cf0fce3ea,0xa01cf874c51b0496ba0b26486d800ff690c7db586671a0cf2feb795,0xe84cb00c50010195dabdd1f84d60bfe293468b72de1a05f3fc8086,0xc814a07c81ea049eafa34e8ed50cf4314c4b8fe5101309f7f6cfca,0xe804d02c52020c9f1a9d61882d50af5416316c32d811d0ef8f7637c,0xa07cf07489d708902a50ec685d005fc394b6dfca8001b01f0f0d9d9,0xf034a80caa5f059e1a16ff988d900f069331eb2d1a71807fdf93599,0xc024c044590204942afdf1188d400f592358b6a9d5190df4fa5aca,0xb83ce87c59cf0a9d1ad4e328ad40df341545edb5d9f180bfcf92c52,0xf044c014370f019d9add52580d505f9112361fb44c11203f0fcaeea,0xa014900482e4079afafab4288d408fd295450b52605120df9fdd92d,0xc01cb06c3b340498baf28ac8cd400fa614c66816961e0ef3ff8ee9,0x8844a87ca45d06993a66dff84d40dff515c2a954c77150efbfd872f,0xa834d04c4e6807924a15c0a8adc08fb51240b9986711f05f1f64df6,0xe064f07438390896ea82aa28fd108f41032eb4c3be1909fbf24080,0x8024f82c5703089fa7df118ed702fa792b6f84dca21500fefa3b9c,0x981cf80c88d00e92fa805c884d004fd417c68fe1bc21e00f9fc2338,0x9844e8643c3906988ad24a18fd600f1116c69f0808b1a09f6f4b6cf,0xf814c82447cd0497ca1ed598ed701f7195b28b34de160cfdf4898a,0xe864e86c85d80e917a40fc68ada05fe69230be7e8961805f1fadcb3,0x884cd83cc61907945abb1688fd001fa39345ef190e81c04fdf7a61d,0xf854a01c98f703986acbf2386d305f710361c67b59140ef5f3fe83,0x8034f074af5c06958a66ffb80d109ff596b13cc19c71009f6fd2536,0xe07c8874320f0f967aeda298bd908fe293471f027a71409fcf822ea,0xc824802c5c090c9aaaad91680d90efc596b39a5cda130af2fa858f,0xa07ca82c98550291cac09e58edd07f5591c2b9f307f1a01f9f70b5b,0xc024904cc413079ea4b3658adf0ff7712b03d2bba31609f5fcd069,0x8014c02450080b9ecacd11d84d509fb19042ead38931a09f6f6dd49]
a.x=[50]
a.y=[50]
a.z=[50]
b=[50]

mktbl3(CRV.G.x,CRV.G.y,CRV.G.z)
//ellip(CRV_n)
//exit()

flag=0
elp3(x)
y.x=Pub_key.x
y.y=Pub_key.y
y.z=Pub_key.z
d.x=[50]
d.y=[50]
d.z=[50]
e.x=[50]
e.y=[50]
e.z=[50]

foriin0..49
elp3(r[i])
a.x[i]=Pub_key.x
a.y[i]=Pub_key.y
a.z[i]=Pub_key.z
printr[i],"\n"
end
foriin0..49
b[i]=(r[i]-c[i]*x)%CRV.n
print"b=",b[i],"\n"
end
foriin0..49
elp3(b[i])
d.x[i]=Pub_key.x
d.y[i]=Pub_key.y
d.z[i]=Pub_key.z
print"dx=",d.x[i],"\n"
end
foriin0..49
elp3(c[i]*x)
e.x[i]=Pub_key.x
e.y[i]=Pub_key.y
e.z[i]=Pub_key.z
print"c=",c[i],"\n"
end
//exit()
foriin0..49
jadd(d.x[i],e.x[i],d.y[i],e.y[i],d.z[i],e.z[i],CRV.p)
if(P.x!=a.x[i]||P.y!=a.y[i]||P.z!=a.z[i])
  //print"i=",i,"\n"
flag=1
end
end
if(flag==0)
print"That'sTrue!\n"
end
if(flag==1)
print"baka\n"
end

end
*/

void logcom(int nn, int argc, char *argv[])
{
	int j = 0;
	ZZ plain[21], ai, nha, r;
	po v, c, gr, vm, P;
	combo e;

	init_curve(nn);
	//mktbl3(CRV.G);
	// elp3(CRV_n)
	// printeccp192,"\n"

	// printsecp128r1_p,"\n"

	// schlorbai
	plain[0] = to_ZZ("30166869646807636562727106419");
	plain[1] = to_ZZ("10045117565996920939000919144");
	plain[2] = to_ZZ("30153181069665353679546770802");
	plain[3] = to_ZZ("10033396893489366934389337632");
	plain[4] = to_ZZ("22631414519632726019188813428");
	plain[5] = to_ZZ("10044280489496830406925033588");
	plain[6] = to_ZZ("32308695848121561557187447072");
	plain[7] = to_ZZ("21179691622725275949743743092");
	plain[8] = to_ZZ("32308695848121561556958080800");
	plain[9] = to_ZZ("30765204363694166514334572576");
	plain[10] = to_ZZ("26119354342939208172857421088");
	plain[11] = to_ZZ("30059276595547511335976247399");
	plain[12] = to_ZZ("34493224637425338651624109358");
	plain[13] = to_ZZ("22631636489395346365953810503");
	plain[14] = to_ZZ("34473881935204201160487432814");
	plain[15] = to_ZZ("32629405756452437358033266789");
	plain[16] = to_ZZ("10024939110908884670107837728");
	plain[17] = to_ZZ("36265758736633729355313211168");
	plain[18] = to_ZZ("30151120555452877393537363824");
	plain[19] = to_ZZ("33546635831558563763501754217");
	plain[20] = to_ZZ("34161127499042836141910073459");

	// plaintext

	// encryption
	ai = plain[1];
	// cout<<"r1="<<ai<<"\n";
	Qmlt(CRV.G,ai);
	v.x = Pub_key.x;
	v.y = Pub_key.y;
	//v.z = Pub_key.z;
	// printv_x,"",v_y,"\n"

	e = hash2(argc, argv);
	nha ^= e.e1.dd[0];
	nha = (nha << 64);
	nha ^= e.e1.dd[1];
	nha = (nha << 64);
	nha ^= e.e2.dd[0];
	nha = (nha << 64);
	nha ^= e.e2.dd[1];

	cout << "mha=" << nha << "\n";
	Qmlt(v,nha);
	c.x = Pub_key.x;
	c.y = Pub_key.y;
	//c.z = Pub_key.z;
	// printc_x,"",c_y,"\n"
	cin >> r;
	cout << r << endl;
	Qmlt(CRV.G,r);
	gr.x = Pub_key.x;
	gr.y = Pub_key.y;
	//gr.z = Pub_key.z;
	eadd(gr, c);
	Gen_key.x = P.x;
	Gen_key.y = P.y;
	//Gen_key.z = P.z;
	if ((Gen_key.y * Gen_key.y) % CRV.p != ((Gen_key.x * Gen_key.x * Gen_key.x) + CRV.a * Gen_key.x  + CRV.b) % CRV.p){
	{
		cout << "error!!\n";
		exit(1);
	}

	if ((Gen_key.y * Gen_key.y) % CRV.p == ((Gen_key.x * Gen_key.x * Gen_key.x) + CRV.a * Gen_key.x + CRV.b ) % CRV.p)
	{
		cout << argv[1] << "'sLogCommitmentis"
			 << "\n";
		cout << "x=" << Gen_key.x << "\n";
		cout << "y=" << Gen_key.y << "\n";
		//cout << "z=" << Gen_key.z << endl;
	}
}
}
//
// otp(133,2244)

int ecc(int argc, char *argv[])
{
	unsigned char *str, m[32], s[32], ss[64];
	ZZ x, y, z, salt;
	int i, n, mm[32];
	FILE *fp;
	// cout<<"inputecc256,2562,2563,2564,2565"<<endl;
	// cin>>i;
	n = 256;
	init_curve(n);
	// logcom(256,argc,argv);
	// exit(1);

	fp = fopen("key.bin", "rb");
	if (fp == NULL)
	{
		printf("���t�@�C��������܂���B\n");
		exit(1);
	}
	fread(m, 1, 32, fp);
	salt = 0;
	for (i = 0; i < 32; i++)
		salt = (salt << 8) ^ m[i];

	fclose(fp);
	if (!strcmp(argv[1], "x") && argv[2] == NULL)
	{
		cout << "ecdh Version 0.01���@\n ���s�i���o�[���w�肵�Ă��������B" << endl;
		cout << "�O�F�����̌��J���𐶐�����B" << endl;
		cout << "�P�F���t�@�C����DSA2�����𐶐�����" << endl;
		cout << "�Q�F��M�������̏��������؂���" << endl;
		cout << "�R�F����̌��J������͂��ċ��L���𐶐�����" << endl;
		exit(1);
	}
	if (salt < 1)
	{
		cout << "�����s���ł��B" << endl;
		exit(1);
	}

	if (!strcmp(argv[2], "0"))
	{
		keygen(salt);
		exit(0);
	}
	if (!strcmp(argv[2], "3"))
	{
		ecdh();
		exit(0);
	}
	if (!strcmp(argv[2], "1"))
	{
		dsa2(argc, argv);
		exit(0);
	}
	if (!strcmp(argv[2], "2"))
	{
		dsa2(argc, argv);
		exit(0);
	}
	else if (strcmp(argv[2], "0") && strcmp(argv[2], "1") && strcmp(argv[2], "2") && strcmp(argv[2], "3"))
	{
		cout << "���݂��Ȃ����[�h�ł��B" << endl;
		exit(1);
	}

	// ecdh(to_ZZ("12"),to_ZZ("97"),n);

	return 0;
}

/* Takes as input an odd prime p and n < p and returns r
 * such that r * r = n [mod p]. */
ZZ tonelli_shanks(ZZ n, ZZ p)
{
	ZZ s = to_ZZ("0");
	ZZ q = p - 1;
	while ((q & 1) == 0)
	{
		q /= 2;
		++s;
	}
	if (s == 1)
	{
		ZZ r = pow_mod(n, (p + 1) / 4, p);
		if ((r * r) % p == n)
			return r;
		return to_ZZ("0");
	}
	// Find the first quadratic non-residue z by brute-force search
	ZZ z = to_ZZ("1");
	while (pow_mod(++z, (p - 1) / 2, p) != p - 1)
		;
	ZZ c = pow_mod(z, q, p);
	ZZ r = pow_mod(n, (q + 1) / 2, p);
	ZZ t = pow_mod(n, q, p);
	ZZ m = s;
	while (t != 1)
	{
		ZZ tt = t;
		ZZ i = to_ZZ("0");
		while (tt != 1)
		{
			tt = (tt * tt) % p;
			++i;
			if (i == m)
				return to_ZZ("0");
		}
		ZZ b = pow_mod(c, pow_mod(to_ZZ("2"), m - i - 1, p - 1), p);
		ZZ b2 = (b * b) % p;
		r = (r * b) % p;
		t = (t * b2) % p;
		c = b2;
		m = i;
	}
	if ((r * r) % p == n)
		return r;
	return to_ZZ("0");
}

ZZ root(ZZ a, ZZ p)
{
	ZZ c, b;

	cout << "p mod = " << a << "@" << p % 4 << p << endl;

	if (p % 4 == 3 || p % 8 == 5)
	{
		if (p % 4 == 3)
		{
			b = (p + 1) / 4;
			c = pow_mod(a, b, p);
			{
				// printf("good\n");
				cout << c << endl;
				return c;
			}
		}
		if (p % 8 == 5)
		{
			c = pow_mod(a, (p + 3) / 8, p);
			if ((c * c) % p != a)
			{
				printf("baka2\n");
				c = 2 * a * pow_mod(4 * a, (p - 5) / 8, p);
				if (c * c % p == a)
				{
					printf("good\n");
					return c;
				}
				// return to_ZZ("-1");
			}
			else if (c * c % p == a)
			{
				printf("good\n");
				cout << c << endl;
				return c;
			}
		}
	}
	if (p % 8 == 1 || p % 4 == 1)
	{
		c = tonelli_shanks(a, p);
		if (c * c % p != a)
		{
			printf("fail!\n");
			return to_ZZ("-1");
		}
		else
		{
			return c;
		}
	}
	//  return to_ZZ("0");
	printf("get back\n");
	exit(1);
}

po compress_point(po point)
{
	po z;
	z.x = point.x;
	z.y = point.y % 2;
	//z.z = 1;

	return z;
}

po uncompress_point(po compressed_point)
{
	ZZ x, is_odd;
	po z;
	x = compressed_point.x;
	is_odd = compressed_point.y;
	ZZ y = SqrRootMod(((x * x * x) + CRV.a * x + CRV.b) % CRV.p, CRV.p);
	z.x = x;
	z.y = y;
	//z.z = 1;
	if (is_odd == (y & 1))
		return z;

	z.y = (CRV.p - y);
	return z;
}

// invert of integer
int inv2(int a, int n)
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
kem invs2(kem a)
{
	kem s = {0};
	s.u = 23 - ((((inv2(a.v, 23) * (a.u)))) % 23);
	s.v = inv2(a.v, 23);
	// s.u%=23;

	return s;
}

sem invs(sem a)
{
	sem s;
	ten aa = p2t(a.u);
	s.u = Qmlt(a.u, CRV.n-inv((a.v), CRV.n));
	s.v = inv(a.v, CRV.n);
	// s.u%=23;
	//cout << "sv=" << a.v << endl;

	return s;
}

sem semi(sem a, sem b)
{
	sem n; // = {0};
	n.u = eadd(Qmlt(b.u, a.v), a.u);
	n.v = (a.v * b.v) % CRV.n;
	// n.u = n.to_ZZ("23");

	return n;
}

sem cemi(sem a, sem b)
{
	sem n; // = {0};
	ten m = p2t(a.u);

	n.u = eadd(Qmlt(a.u, b.v), b.u);
	n.v = (a.v * b.v) % CRV.n;
	// n.u = n.to_ZZ("23");

	return n;
}

kem kemi(kem a, kem b)
{
	kem n = {0};
	n.u = ((a.v * b.u) % 23 + a.u);
	n.v = (a.v * b.v) % 23;
	n.u %= 23;

	return n;
}

kem tdp2(kem a, kem b, kem c)
{
	return kemi(a, kemi(b, invs2(c)));
}

sem tdp(sem a, sem b, sem c)
{
	return semi(semi(a,b),invs(c));
}

void psem(sem a)
{
	cout << a.u.x << "," << a.u.y << "," << a.v << endl;
}

int kpk()
{
	kem a, b, c, d, e, f, g, h, a1, a2, a3, b1, b2, b3, c1, c2, c3, d1, d2, d3, e1, f1, g1, h1;
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

	kem x, y;
	int p = 17;
	kem key[4];
	kem tmp1;
	tmp1.u = 7;
	tmp1.v = 8;

	x.u = 19;
	x.v = 20;
	y.u = 21;
	y.v = 22;
	int r1 = 0b00, r2 = 0b11;
	// alice's public key
	a1 = tdp2(a, x, (b));
	a2 = tdp2(b, x, (c));
	a3 = tdp2(c, x, (d));
	b1 = tdp2(a, y, (b));
	b2 = tdp2(b, y, (c));
	b3 = tdp2(c, y, (d));

	// bob's public key
	c1 = tdp2(e, x, (f));
	c2 = tdp2(f, x, (g));
	c3 = tdp2(g, x, (h));
	d1 = tdp2(e, y, (f));
	d2 = tdp2(f, y, (g));
	d3 = tdp2(g, y, (h));
	kem pi, phi;
	phi.u = 12;
	phi.v = 6;
	pi = kemi(d, phi);
	printf("e=%d %d\n", pi.u, pi.v);
	// exit(1);

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

	kem tmp[16];
	tmp[3].u = 12;
	tmp[3].v = 6;
	pi = kemi(invs2(d), a);
	phi = kemi(invs2(h), e);
	printf("%d %d\n", pi.u, pi.v);
	printf("%d %d\n", phi.u, phi.v);
	// exit(1);

	key[0] = kemi(kemi(a1, a2), a3);
	key[1] = kemi(kemi(c1, c2), c3);
	printf("Alice's encrypted key = (%d,%d)\n", key[0].u, key[0].v);
	printf("Bob's encrypted key = (%d,%d)\n", key[1].u, key[1].v);
	tmp[0] = kemi(kemi(invs2(a), key[0]), (d));
	tmp[1] = kemi(kemi(invs2(e), key[1]), (h));
	printf("decrypted key-A = (%d,%d)\n", tmp[0].u, tmp[0].v);
	printf("decrypted key-B = (%d,%d)\n", tmp[1].u, tmp[1].v);
	tmp[5] = kemi(kemi(x, x), x);
	tmp[4] = kemi(kemi((a), key[5]), invs2(d));
	key[1] = kemi(kemi(c1, c2), c3);
	printf("x^3=%d %d\n", tmp[5].u, tmp[5].v);
	// exit(1);
	return 0;
}

sem back(sem a, sem a1, sem b)
{
	return semi(invs(a), semi(a1, b));
}

int ekp()
{
	sem a, b, c, d, e, f, g, h, a1, a2, a3, b1, b2, b3, c1, c2, c3, d1, d2, d3, e1, f1, g1, h1;

	a.u = Qmlt(CRV.G, to_ZZ("6"));
	a.v = to_ZZ("26");
	b.u = Qmlt(CRV.G, to_ZZ("15"));
	b.v = to_ZZ("25");
	c.u =Qmlt(CRV.G, to_ZZ("11"));
	//c.u.y =to_ZZ("21"); //Qmlt(CRV.G, to_ZZ("15"));
	c.v = to_ZZ("2");
	d.u = Qmlt(CRV.G, to_ZZ("12"));
	//d.u.y = to_ZZ("15"); //Qmlt(CRV.G, to_ZZ("15"));
	d.v = to_ZZ("1");
	g1.u=eadd(c.u,d.u);
	cout << g1.u.x << "," << g1.u.y << endl;
	//exit(1);

	e.u = Qmlt(CRV.G, to_ZZ("5"));
	e.v = to_ZZ("4");
	f.u = Qmlt(CRV.G, to_ZZ("17"));
	f.v = to_ZZ("12");
	g.u = Qmlt(CRV.G, to_ZZ("13"));
	g.v = to_ZZ("14");
	h.u = Qmlt(CRV.G, to_ZZ("11"));
	h.v = to_ZZ("16");

	printf("inv6=%d\n", inv2(6, 41));

	
	sem x, y;
	int p = 17;
	sem key[4];

	x.u = Qmlt(CRV.G, to_ZZ("19"));
	x.v = to_ZZ("20");
	y.u = Qmlt(CRV.G, to_ZZ("21"));
	y.v = to_ZZ("22");

	int r1 = 0b00, r2 = 0b11;
	// alice's public key
	
	a1 = tdp(a, x, (b));
	a2 = tdp(b, x, (c));
	a3 = tdp(c, x, (d));
	psem(a1);
	psem(a2);
	psem(a3);
	//exit(1);
	
	
	sem aga;
	aga=semi(a1,semi(a2,a3));
	//aga=tdp(a1,a2,a3); //semi(semi(a,a),invs(a));
	psem(aga);
	aga=semi(semi(invs(a),aga),d);
	psem(aga);
	aga=semi(semi(x,x),x);
	psem(aga);
	
	exit(1);
	
	b1 = tdp(a, y, (b));
	b2 = tdp(b, y, (c));
	// exit(1);
	b3 = tdp(c, y, (d));
	// exit(1);
	//  bob's public key
	
	//c1 = tdp(e, x, (f));
	//c2 = tdp(f, x, (g));
	//c3 = tdp(g, x, (h));
	d1 = tdp(e, y, (f));
	d2 = tdp(f, y, (g));
	d3 = tdp(g, y, (h));
	sem pi, phi;
	pi=invs(b);
	psem(pi);
	//exit(1);
	pi=semi((a),(pi));
	psem(pi);
	exit(1);
	pi=semi(pi,a);
	psem(pi);
	psem(b);
	exit(1);
	phi=tdp(a1,a2,(a3));
	psem(phi);
	exit(1);
	pi=semi((a),semi(pi,invs(d)));
	psem(pi);
	exit(1);
	pi=tdp(x,x,x);
	psem(pi);
	exit(1);
	//pi=semi(a,semi(pi,invs(d))));
	phi=semi(a,semi(pi,invs(d)));
	phi=semi(pi,invs(d));
	psem(pi);
	psem(phi);
	
	psem(cemi(pi,(c)));
	psem(semi(pi,(c)));
	psem(cemi((c),pi));
	psem(semi((c),pi));
	psem(a);
	exit(1);
	//pi=tdp(a1,a2,(a3));
	psem(semi(a,pi));
	pi=cemi(d,phi);
	psem(pi);
	exit(1);

	pi=cemi(pi,d); //semi(pi,d)));
	psem(semi(invs(a),pi));
	//exit(1);
	//psem(back(a,a1,b));
	pi = semi(a, x);
	psem(pi);
	pi = cemi(invs(x), pi);
	psem(pi);
	psem(x);
	exit(1);
	psem(a);
	exit(1);
	// phi.u=12;
	// phi.v=6;
	// pi=semi(d,phi);
	// printf("e=%d %d\n",pi.u,pi.v);
	// exit(1);

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
	printf("Alice's Pubkey1 ="); // %d %d\n", a1.u, a1.v);
	psem(a1);
	printf("Alice's Pubkey1 =");
	psem(a2);
	printf("Alice's Pubkey1 = "); //%d %d\n", a3.u, a3.v);
	psem(a3);
	printf("Alice's Pubkey2 = "); //%d %d\n", b1.u, b1.v);
	psem(b1);
	printf("Alice's Pubkey2 = "); //%d %d\n", b2.u, b2.v);
	psem(b2);
	printf("Alice's Pubkey2 = "); //%d %d\n", b3.u, b3.v);
	psem(b3);
	printf("Bob's Pubkey1 = "); //%d %d\n", c1.u, c1.v);
	psem(c1);
	printf("Bob's Pubkey1 = "); //%d %d\n", c2.u, c2.v);
	psem(c2);
	printf("Bob's Pubkey1 = "); //%d %d\n", c3.u, c3.v);
	psem(c3);
	printf("Bob's Pubkey2 = "); //%d %d\n", d1.u, d1.v);
	psem(d1);
	printf("Bob's Pubkey2 = "); //%d %d\n", d2.u, d2.v);
	psem(d2);
	printf("Bob's Pubkey2 = "); //%d %d\n", d3.u, d3.v);
	psem(d3);

	sem tmp[16];
	// tmp[3].u=12;
	// tmp[3].v=6;
	pi = semi(invs(d), a);
	phi = semi(invs(h), e);
	psem(pi);
	psem(phi);
	// printf("%d %d\n",pi.u,pi.v);
	// printf("%d %d\n",phi.u,phi.v);
	//
	c = cemi(invs(c), (c));
	exit(1);
	key[0] = semi(semi(a1, a2), a3);
	key[1] = semi(semi(c1, c2), c3);
	//  exit(1);
	printf("Alice's encrypted key = "); //(%d,%d)\n", key[0].u, key[0].v);
	psem(key[0]);
	printf("Bob's encrypted key = "); //(%d,%d)\n", key[1].u, key[1].v);
	psem(key[1]);
	tmp[0] = semi(semi(invs(a), key[0]), (d));
	tmp[1] = semi(semi(invs(e), key[1]), (h));
	printf("decrypted key-A = "); //(%d,%d)\n", tmp[0].u, tmp[0].v);
	psem(tmp[0]);
	printf("decrypted key-B = "); //(%d,%d)\n", tmp[1].u, tmp[1].v);
	psem(tmp[1]);
	tmp[5] = semi(semi(x, x), x);
	// tmp[4]=semi(semi((a),key[5]),invs(d));
	// key[1] = semi(semi(c1, c2), c3);
	printf("x^3= "); //%d %d\n",tmp[5].u,tmp[5].v);
	psem(tmp[5]);
	exit(1);
	return 0;
}

int main(int argc, char *argv[])
{
	ZZ z, r, d, I, salt;
	int i, j, k, l;
	char m;
	FILE *fp;
	unsigned char key[32];
	po sig;
	char file[32];
	po T;
	ZZ P;
	ZZ a;

	ellmat J;
	ZZ Za[2][2];
	// mp_init(4096);
	P = to_ZZ("340282366920938463463374607431768211283");
	a = to_ZZ("143864072772599444046778416709082679388");
	ZZ c = to_ZZ("6");
	ZZ b = to_ZZ("22222222222222222222222222222222");
	po A, B;
	po C, M;

	po cho;
	cho.x = to_ZZ("10");
	cho.y = to_ZZ("13");

	init_curve(16);
	cout << inv2(6, 41) << "\n";
	
	//mktbl3(CRV.G);
	//cout << elp3(b).x << endl;
	//mktable(CRV.G.x,CRV.G.y);
	//cout << ellip(to_ZZ("7")).x << endl;
	//cout << ellip(to_ZZ("7")).y << endl;
	//cout << ellip(to_ZZ("41-7")).x << endl;
	//cout << ellip(to_ZZ("41-7")).y << endl;
	//exit(1);
	sem ae[10];
	ae[0].v=to_ZZ("6");
	ae[0].u=Qmlt(CRV.G,to_ZZ("6"));
	ae[1].u=Qmlt(CRV.G,to_ZZ("15"));
	ae[1].v=to_ZZ("15");
	psem(ae[0]);
	psem(ae[1]);
	eadd(ae[0].u,ae[1].u);
	ae[2]=invs(ae[0]);
	psem(ae[2]);
	//exit(1);

	//oe.v=to_ZZ("6");
	//oe.u=cho;
	//uo=semi(oe,oe);
	//psem(uo);
	//uo=cemi(uo,ae);
	//psem(uo);
	/*
	cout << Qmlt(CRV.G,c).x << " --Qx" << endl;
	cout << Qmlt(CRV.G,c).y << " --Qy" << endl;
	cout << eadd(Qmlt(CRV.G,c),cho).x << " --Qx" << endl;
	cout << eadd(Qmlt(CRV.G,c),cho).y << " --Qy" << endl;
	cout << eadd(ae.u,oe.u).x << " --Qx" << endl;
	cout << eadd(ae.u,oe.u).y << " --Qy" << endl;
	*/
	ekp();
	exit(1);

	/*
	cout << ellip(c).x << " --ellip" << endl;
	cout << elp3(c).x << " --elp3" << endl;
	cout << elp(c,CRV.G).x << " -elp" << endl;
	*/
	exit(1);
	ZZ Qi[2][2], Qu[2][2], aa = to_ZZ("0"), Qe[2][2];
	po Q[2][2];
	Q[0][0] = Qmlt(CRV.G, c);
	Q[0][1] = Qmlt(CRV.G, P);
	Q[1][0] = Qmlt(CRV.G, a);
	Q[1][1] = Qmlt(CRV.G, b);
	Qi[0][0] = P;
	Qi[0][1] = a;
	Qi[1][0] = b;
	Qi[1][1] = c;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			cout << Qi[i][j] << endl;
			Qe[i][j] = Qi[i][j];
		}
	}

	// exit(1);
	niji(Qi);
	// exit(1);
	// exit(1);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
			cout << Qe[i][j] << endl;
	}
	// exit(1);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			aa = to_ZZ("0");
			for (int k = 0; k < 2; k++)
				aa += Qe[i][k] * PLO[k][j];
			Qu[i][j] = aa % CRV.p;
			cout << Qu[i][j] << endl;
		}
	}

	// printf("%d\n",i&1);
	exit(1);

	c = SqrRootMod(a, P);
	cout << c << endl;
	cout << a << endl;
	cout << ((c * c) % P) << endl;
	dsa2(argc, argv);

	init_curve(64);
	A = compress_point(CRV.G);
	cout << A.x << " pc1 " << A.y << endl;
	B = uncompress_point(A);
	cout << B.x << " pc2 " << B.y << endl;
	// exit(1);
	cout << Qmlt(CRV.G, CRV.n).f << endl;
	// exit(1);
	// cin >> r;
	//  mktbl3(CRV.G);
	//  A=elp3(r);

	// ten F=jdbl(CRV.G);

	// 20220905 testing
	mktable(CRV.G.x, CRV.G.y);
	// mktbl3(CRV.G);
	po xxx;
	po yyy;
	// xxx.z =to_ZZ("1");
	int bb = 0;
	// C = ellip(r);

	while (bb < 256)
	{
		yyy = Qmlt(CRV.G, to_ZZ(random()));
		xxx.x = yyy.x;
		xxx.y = yyy.y;
		if (compress_point(xxx).y == 0)
		{
			cout << "x" << compress_point(xxx).x << endl;
			bb++;
		}
	}
	exit(1);

	po X[2][2] = {{C, Qmlt(CRV.G, P)}, {Qmlt(CRV.G, to_ZZ("123")), ellip(a)}};
	cout << C.x << " ellip " << C.y << endl;
	M = Qmlt(CRV.G, r);
	cout << M.x << " ml2 " << M.y << endl;
	exit(1);

	ecdh();
	// exit(1);

	for (i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
			J.G[i][j] = CRV.G;
	}
	// exit(1);

	cout << "if argv1=k then key pair generate\n";
	if ((fp = fopen("eccsec.key", "rb")) == NULL) // (strcmp(argv[1], "k") == 0)
	{
		seed();
		argv[1] = "-s";
		argv[2] = password;
		salt = sha2(3, argv);
		keygen(salt);
	}

	cout << "input mode e or d or s\n";
	cin >> m;
	if (strcmp(&m, "e") == 0)
		enc(argv, 256);

	if (strcmp(&m, "h") == 0)
	{
		cout << "usage:e file encryption; d file decryption; s generate signature; \n";
		return 0;
	}
	if (strcmp(&m, "d") == 0)
		dec(argv, 256);

	if (strcmp(&m, "s") == 0)
	{
		I = 0;
		d = 0;
		fp = fopen("eccsec.key", "rb");
		fread(key, 1, 32, fp);
		fclose(fp);
		for (i = 0; i < 32; i++)
		{
			I = key[i];
			d ^= I << (i * 8);
		}
		cin >> r;
		if (r == 0)
			cout << "r is 0" << endl;
		z = sha2(argc, argv);
		sig = ecdsa(r, z, d);
		vr_ecdsa(sig.x, sig.y, z);
	}

	return 0;
}
