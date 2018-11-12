#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>
#include<assert.h>


using namespace std;

void w61(void) {
	mpf_t N;
	mpf_init2(N,1000);
	mpf_set_str (N, "17976931348623159077293051907890247336179769789423065727343008115 \
												77326758055056206869853794492129829595855013875371640157101398586 \
												47833778606925583497541085196591615128057575940752635007475935288 \
												71082364994994077189561705436114947486504671101510156394068052754 \
												0071584560878577663743040086340742855278549092581e0"
									,10);

	mpf_t Nsqrt;
	mpf_init2 (Nsqrt,1000);
	mpf_sqrt(Nsqrt,N);

	//this is also A	
	mpf_t Nsqrt_ceil;
	mpf_init2 (Nsqrt_ceil,1000);
	mpf_ceil(Nsqrt_ceil,Nsqrt);

	mpf_t A2;
	mpf_init2 (A2,1000);
	mpf_mul(A2,Nsqrt_ceil,Nsqrt_ceil);

	mpf_t A2N;
	mpf_init2(A2N,1000);
	mpf_sub(A2N,A2,N);

	mpf_t A2N_sqrt;
	mpf_init2(A2N_sqrt,1000);
	mpf_sqrt(A2N_sqrt,A2N);

	mpf_t p;
	mpf_init2(p,1000);
	mpf_sub(p,Nsqrt_ceil,A2N_sqrt);

//	gmp_printf("N is %Ff\n",N);
//	gmp_printf("Nsqrt is %Ff\n",Nsqrt);
//	gmp_printf("Nsqrt_ceil is %Ff\n",Nsqrt_ceil);
//	gmp_printf("A2 is %Ff\n",A2);
//	gmp_printf("A2N is %Ff\n",A2N);
//	gmp_printf("x is %Ff\n",A2N_sqrt);
	gmp_printf("answer 1 is %Ff\n",p);

}

void w62(void) {
	mpf_t N;
	mpf_init2(N,1000);
	mpf_set_str(N,"6484558428080716696628242653467722787263437207069762630604390703787 \
								9730861808111646271401527606141756919558732184025452065542490671989 \
								2428844841839353281972988531310511738648965962582821502504990264452 \
								1008852816733037111422964210278402893076574586452336833570778346897 \
								15838646088239640236866252211790085787877e0"
				,10);
	
	mpf_t Nsqrt;
	mpf_init2 (Nsqrt,1000);
	mpf_sqrt(Nsqrt,N);

	//this is also A	
	mpf_t Nsqrt_ceil;
	mpf_init2 (Nsqrt_ceil,1000);
	mpf_ceil(Nsqrt_ceil,Nsqrt);

	for(long i=0;i<=1024*1024;i++) {
//		if(i%100 ==0) {
//			gmp_printf("current p is  %Ff\n",Nsqrt_ceil);
//			gmp_printf("current i is  %d\n",i);
//		}

		//now Nsqrt_ceil is actually used as A
		mpf_t A2;
		mpf_init2 (A2,1000);
		mpf_mul(A2,Nsqrt_ceil,Nsqrt_ceil);

		mpf_t A2N;
		mpf_init2(A2N,1000);
		mpf_sub(A2N,A2,N);

		//A2N_sqrt is x
		mpf_t A2N_sqrt;
		mpf_init2(A2N_sqrt,1000);
		mpf_sqrt(A2N_sqrt,A2N);
		
		mpf_t x_floor;
		mpf_init2(x_floor,1000);
		mpf_floor(x_floor,A2N_sqrt);

		if(mpf_cmp(A2N_sqrt,x_floor) == 0) {
			mpf_t p;
			mpf_init2(p,1000);
			mpf_sub(p,Nsqrt_ceil,x_floor);
			gmp_printf("answer 2 is %Ff\n",p);
			return;
		}

		mpf_t tmp;
		mpf_init2 (tmp,1000);
		mpf_add_ui(tmp,Nsqrt_ceil,1);
		mpf_set(Nsqrt_ceil,tmp);
	}

	assert(0);
	
}


void w63(void) {
	mpf_t N;
	mpf_init2 (N,1000);
	mpf_set_str(N,"72006226374735042527956443552558373833808445147399984182665305798191 \
								63556901883377904234086641876639384851752649940178970835240791356868 \
								77441155132015188279331812309091996246361896836573643119174094961348 \
								52463970788523879939683923036467667022162701835329944324119217381272 \
								9276147530748597302192751375739387929e0"
						,10);
//	gmp_printf("N is %Ff\n",N);
	mpf_t N6;
	mpf_init2 (N6,1000);	
	mpf_mul_ui(N6,N,6);
//	gmp_printf("N6 is %Ff\n",N6);

	mpf_t N6_sqrt;
	mpf_init2(N6_sqrt,1000);
	mpf_sqrt(N6_sqrt,N6);
//	gmp_printf("N6_sqrt is %Ff\n",N6_sqrt);

	mpf_t N6_sqrt_mul2;
	mpf_init2(N6_sqrt_mul2,1000);
	mpf_mul_ui(N6_sqrt_mul2,N6_sqrt,2);

	mpf_t N6_sqrt_mul2_ceil;
	mpf_init2(N6_sqrt_mul2_ceil,1000);
	mpf_ceil(N6_sqrt_mul2_ceil,N6_sqrt_mul2);

	//this is actually p3q2div2 and A
	mpf_t N6_sqrt_ceil;
	mpf_init2(N6_sqrt_ceil,1000);
	mpf_div_ui(N6_sqrt_ceil,N6_sqrt_mul2_ceil,2);

	mpf_t p3q2div2_sqt;
	mpf_init2(p3q2div2_sqt,1000);
	mpf_mul(p3q2div2_sqt,N6_sqrt_ceil,N6_sqrt_ceil);

	mpf_t p3q2div2_sqt_sub_N6;
	mpf_init2(p3q2div2_sqt_sub_N6,1000);
	mpf_sub(p3q2div2_sqt_sub_N6,p3q2div2_sqt,N6);

	mpf_t x;
	mpf_init2(x,1000);
	mpf_sqrt(x,p3q2div2_sqt_sub_N6);
//	gmp_printf("x is %Ff\n",x);

	mpf_t low,high;
	mpf_init2(low,1000);
	mpf_init2(high,1000);
	mpf_sub(low,N6_sqrt_ceil,x);
	mpf_add(high,N6_sqrt_ceil,x);
//	gmp_printf("low is %Ff\n",low);
//	gmp_printf("high is %Ff\n",high);

	mpf_t lowdiv2,lowdiv3,highdiv2,highdiv3;
	mpf_init2(lowdiv2,1000);
	mpf_init2(lowdiv3,1000);
	mpf_init2(highdiv2,1000);
	mpf_init2(highdiv3,1000);
	mpf_div_ui(lowdiv2,low,2);
	mpf_div_ui(lowdiv3,low,3);
	mpf_div_ui(highdiv2,high,2);
	mpf_div_ui(highdiv3,high,3);

	mpf_t lowdiv2_floor,lowdiv3_floor,highdiv2_floor,highdiv3_floor;
	mpf_init2(lowdiv2_floor,1000);
	mpf_init2(lowdiv3_floor,1000);
	mpf_init2(highdiv2_floor,1000);
	mpf_init2(highdiv3_floor,1000);
	mpf_floor(lowdiv2_floor,lowdiv2);
	mpf_floor(lowdiv3_floor,lowdiv3);
	mpf_floor(highdiv2_floor,highdiv2);
	mpf_floor(highdiv3_floor,highdiv3);

	if(mpf_cmp(lowdiv2_floor,lowdiv2) == 0 && mpf_cmp(highdiv3_floor,highdiv3) == 0) {
		if(mpf_cmp(lowdiv2,highdiv3) >0 ) {
			gmp_printf("answer 3 is %Ff\n",highdiv3);
		} else {
			gmp_printf("answer 3 is %Ff\n",lowdiv2);
		}
		return;
	} else if(mpf_cmp(lowdiv3_floor,lowdiv3) ==0 && mpf_cmp(highdiv2_floor,highdiv2) ==0) {
		if(mpf_cmp(lowdiv3,highdiv2) >0 ) {
			gmp_printf("answer 3 is %Ff\n",highdiv2);
		} else {
			gmp_printf("answer 3 is %Ff\n",lowdiv3);
		}
		return;
	}
	assert (0);
}

int getint(char c) {
	if(((unsigned int)c)>=(unsigned int)'0' && ((unsigned int)c)<=(unsigned int)'9')
		return ((unsigned int)c)-(unsigned int)'0';
	else if(((unsigned int)c)>=(unsigned int)'a' &&((unsigned int)c)<='f')
		return ((unsigned int)c)-(unsigned int)'a';
	else if(((unsigned int)c)>=(unsigned int)'A' &&((unsigned int)c)<='F')
		return ((unsigned int)c)-(unsigned int)'A';
	else
		assert(0);
}

void w64(void) {
	mpf_t p;
	mpf_init2(p,1000);
	mpf_set_str(p,"13407807929942597099574024998205846127479365820592393377723561443721764030073662768891111614362326998675040546094339320838419523375986027530441562135724301e0",10);
//	gmp_printf("p is %Ff\n",p);
	

	mpf_t N;
	mpf_init2(N,1000);
	mpf_set_str (N, "17976931348623159077293051907890247336179769789423065727343008115 \
												77326758055056206869853794492129829595855013875371640157101398586 \
												47833778606925583497541085196591615128057575940752635007475935288 \
												71082364994994077189561705436114947486504671101510156394068052754 \
												0071584560878577663743040086340742855278549092581e0"
									,10);
	mpf_t q;
	mpf_init2(q,1000);
	mpf_div(q,N,p);
//	gmp_printf("q is %Ff\n",q);

	mpf_t psub1,qsub1;
	mpf_init2(psub1,1000);
	mpf_init2(qsub1,1000);
	mpf_sub_ui(psub1,p,1);
	mpf_sub_ui(qsub1,q,1);

	mpf_t phi;
	mpf_init2(phi,1000);
	mpf_mul(phi,psub1,qsub1);
//	gmp_printf("phi is %Ff\n",phi);

	mpz_t pz,qz,phiz;
	mpz_init(pz);
	mpz_init(qz);
	mpz_init(phiz);
	mpz_set_str(pz,"13407807929942597099574024998205846127479365820592393377723561443721764030073662768891111614362326998675040546094339320838419523375986027530441562135724301",10);
	mpz_set_str(qz,"13407807929942597099574024998205846127479365820592393377723561443721764030073778560980348930557750569660049234002192590823085163940025485114449475265364281",10);
	mpz_set_str(phiz,"179769313486231590772930519078902473361797697894230657273430081157732675805505620686985379449212982959585501387537164015710139858647833778606925583497541058380975755242863376792702638595783680329979182465163185324772729610833089327423716839554556643863112192450291488028966916159055724074828097964241148004000",10);

	mpz_t pqz;
	mpz_init(pqz);
	mpz_mul(pqz,pz,qz);
//	gmp_printf("pqz is %Zd\n",pqz);

	mpz_t ez;
	mpz_init(ez);
	mpz_set_ui(ez,65537);
	
	//computing d
	mpz_t dz;
	mpz_init(dz);
	int res=mpz_invert(dz,ez,phiz);
	assert(res!=0);
//	gmp_printf("d is %Zd\n",dz);
	
	mpz_t dez;
	mpz_init(dez);
	mpz_mul(dez,dz,ez);

	mpz_t dezmod;
	mpz_init(dezmod);
	mpz_mod(dezmod,dez,phiz);
//	gmp_printf("dezmod is %Zd\n",dezmod);


	mpz_t ct;
	mpz_init(ct);
	mpz_set_str(ct,"22096451867410381776306561134883418017410069787892831071731839143676135600120538004282329650473509424343946219751512256465839967942889460764542040581564748988013734864120452325229320176487916666402997509188729971690526083222067771600019329260870009579993724077458967773697817571267229951148662959627934791540",10);
//	gmp_printf("ct is %Zd\n",ct);

	mpz_t Nz;
	mpz_init(Nz);
	mpz_set_str (Nz, "17976931348623159077293051907890247336179769789423065727343008115 \
												77326758055056206869853794492129829595855013875371640157101398586 \
												47833778606925583497541085196591615128057575940752635007475935288 \
												71082364994994077189561705436114947486504671101510156394068052754 \
												0071584560878577663743040086340742855278549092581"
									,10);
//179769313486231590772930519078902473361797697894230657273430081157732675805505620686985379449212982959585501387537164015710139858647833778606925583497541085196591615128057575940752635007475935288710823649949940771895617054361149474865046711015101563940680527540071584560878577663743040086340742855278549092581
	mpz_t ptencoded;
	mpz_init(ptencoded);
	mpz_powm(ptencoded,ct,dz,Nz);
//	gmp_printf("ptencoded is %#Zx\n",ptencoded);

//0x20805907610b524330594e51d5dbbf643f09603731e9817111392d0c64e2739959a092d4daf979d387520ea7e577af9eb50a29f736925e810ab2fb4640e091a0f73252cb669d5b62b26764190ed188239fe71e1a7cb9e935d2db55c98b024e1dae46d00466163746f72696e67206c65747320757320627265616b205253412e	


	char str[4096];
	mpz_get_str(str,16,ptencoded);
//	printf("str in :%s\n",str);
	assert(str[0]=='2' || (str[0]=='0' && str[1]=='2')) ;
	long i;
	for(i=0;i<4096-1;i++) {
		if(str[i]=='0' && str[i+1]=='0') break;
	}
	assert(i<4096-1);

	char * pstr=str+i+2;
//	printf("pstr in :%s\n",pstr);
	printf("answer 4 is\n");
	for(long i=0;i<4096 && pstr[i]!=(char)0 && pstr[i+1]!=(char)0;i=i+2) {
		int high=getint(pstr[i]);
		assert(high>=0 && high<=15);
		int low=getint(pstr[i+1]);
		assert(low>=0 && low<=15);
		int res=high*16+low;
		printf("%c",(char)res);
	}
	printf("\n");

//	mpz_t ctz;
//	mpz_init(ctz);
//	mpz_powm(ctz,ptencoded,ez,Nz);
//	gmp_printf("ctz is %Zd\n",ctz);

}

int main(int argc, char *argv[]) {
	w61();
	w62();
	w63();
	w64();
}
