#include <openssl/aes.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

using namespace std;


unsigned int char2ui(unsigned char c) {
	if((int)c>=(int)'0' && (int)c<=(int)'9') {
		return (int)c-(int)'0';
	} else if((int)c>=(int)'a' && (int)c<=(int)'z') {
		return 10+(int)c-(int)'a';
	} else if((int)c>=(int)'A' && (int)c<=(int)'Z') {
		return 10+(int)c-(int)'A';
	} else {
//		printf("illegle char %02X\n",c);
		assert(0);
	}
}

unsigned char ui2char(unsigned int ui) {
	if(ui>=0 && ui<=9) {
		return (unsigned char)((int)'0'+ui);
	} else if(ui>=10 && ui<=15) {
		return (unsigned char)((unsigned int)'a'+ui-10);
	} else {
//		printf("illegle ui %x\n",ui);
		assert(-1);
	}
}

void chars2uis(unsigned char * p1, unsigned char * pres,long len) {
	for(long i=0;i<len;i=i+2) {
//		printf("high %02X %d\n",*(p1+i),i);
//		printf("low  %02X %d\n",*(p1+i+1),i);
		unsigned int high=char2ui(*(p1+i));
		unsigned int low =char2ui(*(p1+i+1));
		unsigned int res=high*16+low;
		pres[i]=res;
	}
}

void uis2chars(unsigned char * p1, unsigned char * pres, long len) {
	for(long i=0;i<len;i++) {
		printf("uis2chars %02X\n",p1[i]);
		sprintf((char*)(pres+2*i),"%02X",p1[i]);
	}
}
void strxor (unsigned char * p1,unsigned char * p2,unsigned char * pres, long len) {
	for(long i=0;i<len;i++) {
		pres[i]=p1[i]^p2[i];
	}
}

unsigned int KEYSIZE=16;
unsigned int KEYSIZECHAR=16*2;
unsigned int MAXSIZE=2014;

void cbc_aes(unsigned char * ct, unsigned char* key) {
	printf("ct len %d \n",(int)strlen((char*)ct) );
	printf("key len %d \n",(int)strlen((char*)key) );
	assert(KEYSIZECHAR==(int)strlen((char*)key));
	assert((int)strlen((char*)ct) % KEYSIZECHAR ==0);


	//converting ct and key to uis
	unsigned char ctui[MAXSIZE];
	printf("ct change\n");
	chars2uis(ct,ctui,strlen((char*)ct));
	unsigned char keyui[MAXSIZE];
	printf("key change\n");
	chars2uis(key,keyui,strlen((char*)key));
	// key bits so need to *8
	AES_KEY aeskey;
	AES_set_decrypt_key(keyui,KEYSIZE*8,&aeskey);

	//although each key and frame is 16byte, each byte need two char to represented, so we need to move at 32 byte
	unsigned char finalres[MAXSIZE] ;
	for(long i=KEYSIZE;i<(int)strlen((char*)ct)/2;i=i+KEYSIZE) {
		unsigned char dres[MAXSIZE];
		AES_decrypt(ctui+i,dres,&aeskey);
		unsigned char xorres[MAXSIZE];
		strxor(ctui+i-KEYSIZE,dres,xorres,KEYSIZE);
		uis2chars(xorres,finalres+i-KEYSIZE,KEYSIZE);
		printf("hha\n");
	}

	finalres[strlen((char*)ct)-KEYSIZECHAR-1]=(unsigned char)0;
	printf("finalres is : %s\n",finalres);
}

int main(int argc, char *argv[]) {
	unsigned char ct[] = "4ca00ff4c898d61e1edbf1800618fb2828a226d160dad07883d04e008a7897ee2e4b7465d5290d0c0e6c6822236e1daafb94ffe0c5da05d9476be028ad7c1d81";
	unsigned char key[] = "140b41b22a29beb4061bda66b6747e14";
	cbc_aes(ct,key);
}


