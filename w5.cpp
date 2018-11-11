#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>
#include<unordered_map>
#include<iostream>

using namespace std;

int main(int argc, char *argv[]) {

	mpz_t p,g,h;
	mpz_init (p);
	mpz_init (g);
	mpz_init (h);

	mpz_set_str(p,"13407807929942597099574024998205846127479365820592393377723561443721764030073546976801874298166903427690031858186486050853753882811946569946433649006084171",10);
	mpz_set_str(g,"11717829880366207009516117596335367088558084999998952205599979459063929499736583746670572176471460312928594829675428279466566527115212748467589894601965568",10);
	mpz_set_str(h,"3239475104050450443565264378728065788649097520952449527834792452971981976143292558073856937958553180532878928001494706097394108577585732452307673444020333",10);


	gmp_printf("p is %Zd\n",p);

	char * pstr=(char*)malloc(1024);
	std::unordered_map<std::string,long> map;

	mpz_t hg;
	mpz_init(hg);
	mpz_set(hg,h);

	mpz_t subg;
	mpz_init(subg);
	mpz_invert(subg,g,p);

	//generating the ahsh table
	for(long i=0;i<1024*1024;i++) {
		//generating string for hg
		mpz_get_str(pstr,10,hg);
		if((i%100)==0) {
			gmp_printf(" %Zd \n",hg); 
		}
		std::string hgs(pstr);

		//setting x1s maped to hg
		map.insert(std::make_pair(hgs,i));

		//generating new hg
		mpz_t res;
		mpz_init(res);
		mpz_mul(res,hg,subg);
		mpz_mod(hg,res,p);
		
		mpz_clear(res);
	}

	//geenrating gB
	mpz_t gB;
	mpz_init(gB);
	mpz_set(gB,g);
	long i=1;
	while(i!=1024*1024) {
		mpz_t res;
		mpz_init(res);
		mpz_mul(res,gB,gB);
		mpz_mod(gB,res,p);
		i=i*2;
	}

	//checking the hash table
	mpz_t gBx0;
	mpz_init(gBx0);
	mpz_set_ui(gBx0,1);
	for(long i=0;i<1024*1024;i++) {
		mpz_get_str(pstr,10,gBx0);
		std::string gBx0str(pstr);
		std::unordered_map<std::string,long>::iterator pp;
		if((pp = map.find(gBx0str)) != map.end()) {
			//got it
			cout<<"x1 "<<(pp->second)<<endl;
			cout<<"x0 "<<i<<endl;
			cout<<"result is "<<(pp->second)+1024*1024*i<<endl;
		}
		//geneating the gBx0
		mpz_t res;
		mpz_init(res);
		mpz_mul(res,gBx0,gB);
		mpz_mod(gBx0,res,p);
	}


	mpz_clear(hg);
	mpz_clear(subg);
	mpz_clear(p);
	mpz_clear(g);
	mpz_clear(h);
}

