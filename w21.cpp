#include <openssl/aes.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

using namespace std;
typedef unsigned char uchar;
typedef unsigned int  uint;

unsigned int char2ui(uchar c) {
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

uchar ui2char(unsigned int ui) {
	if(ui>=0 && ui<=9) {
		return (uchar)((int)'0'+ui);
	} else if(ui>=10 && ui<=15) {
		return (uchar)((unsigned int)'a'+ui-10);
	} else {
//		printf("illegle ui %x\n",ui);
		assert(-1);
	}
}

void chars2uis(uchar * p1, uchar * pres,long len) {
	for(long i=0;i<len;i=i+2) {
//		printf("high %02X %d\n",*(p1+i),i);
//		printf("low  %02X %d\n",*(p1+i+1),i);
		unsigned int high=char2ui(*(p1+i));
		unsigned int low =char2ui(*(p1+i+1));
		unsigned int res=high*16+low;
		assert(i%2==0);
		pres[i/2]=res;
	}
}

void uis2chars(uchar * p1, uchar * pres, long len) {
	for(long i=0;i<len;i++) {
		//printf("uis2chars %02X\n",p1[i]);
		sprintf((char*)(pres+2*i),"%02X",p1[i]);
	}
}
void strxor (uchar * p1,uchar * p2,uchar * pres, long len) {
	for(long i=0;i<len;i++) {
		pres[i]=p1[i]^p2[i];
	}
}
void dispuis(uchar * p,long len) {
	for(long i=0;i<len;i++) {
		printf("%02X",p[i]);
	}
	printf("\n");
}

unsigned int KEYSIZE=16;
unsigned int KEYSIZECHAR=16*2;
unsigned int MAXSIZE=2014;

void cbc_aes(uchar * ct, uchar* key) {
	printf("ct len %d \n",(int)strlen((char*)ct) );
	printf("key len %d \n",(int)strlen((char*)key) );
	assert(KEYSIZECHAR==(int)strlen((char*)key));
	assert((int)strlen((char*)ct) % KEYSIZECHAR ==0);

	//converting ct and key to uis
	uchar ctui[MAXSIZE];
	//printf("ct change\n");
	chars2uis(ct,ctui,strlen((char*)ct));
	uchar keyui[MAXSIZE];
	//printf("key change\n");
	chars2uis(key,keyui,strlen((char*)key));
	// key bits so need to *8
	AES_KEY aeskey;
	AES_set_decrypt_key(keyui,KEYSIZE*8,&aeskey);

	//although each key and frame is 16byte, each byte need two char to represented, so we need to move at 32 byte
	uchar finalres[MAXSIZE] ;
	for(long i=KEYSIZE;i<(int)strlen((char*)ct)/2;i=i+KEYSIZE) {
		uchar dres[MAXSIZE];
		AES_decrypt(ctui+i,dres,&aeskey);
		strxor(ctui+i-KEYSIZE,dres,finalres+i-KEYSIZE,KEYSIZE);
	}
	//cut off the padding
	unsigned int paddinglen=(unsigned int)finalres[strlen((char*)ct)/2-KEYSIZE-1]%256;
	assert(paddinglen>=0 && paddinglen<=255);
	for(long i=strlen((char*)ct)/2-KEYSIZE-1;i>=strlen((char*)ct)/2-KEYSIZE-paddinglen;i--) {
		//printf("i %d\n",i);
		assert(((unsigned int)finalres[i])==paddinglen);
	}
	uchar finalres1[MAXSIZE];
	uis2chars(finalres,finalres1,strlen((char*)ct)/2-KEYSIZE-paddinglen);
	finalres1[strlen((char*)ct)-KEYSIZECHAR-paddinglen*2]=(uchar)0;
	//printf("zero position %d\n",strlen((char*)ct)-KEYSIZECHAR-paddinglen*2);
	printf("finalres len %d\n is : %s\n",strlen((char *)finalres),finalres);
}

void stradd1(uchar * pin,uchar * pout,long len) {
	for(long i=len-1;i>=0;i--) {
		*(pout+i)=(uchar)((unsigned int)(*(pin+i))+1);
		if(*(pout+i)!=(uchar)0) return;
	}
}

void ctr_aes(uchar * ct, uchar* key) {
	printf("ct len %d \n",(int)strlen((char*)ct) );
	printf("key len %d \n",(int)strlen((char*)key) );
	assert(KEYSIZECHAR==(int)strlen((char*)key));
	//we may not need to check the size, because ctr is not aliged to KEYSIZE
	//assert((int)strlen((char*)ct) % KEYSIZECHAR ==0);

	//converting ct and key to uis
	uchar ctui[MAXSIZE];
	//printf("ct change\n");
	chars2uis(ct,ctui,strlen((char*)ct));
	uchar keyui[MAXSIZE];
	//printf("key change\n");
	chars2uis(key,keyui,strlen((char*)key));
	// key bits so need to *8
	AES_KEY aeskey;
	AES_set_decrypt_key(keyui,KEYSIZE*8,&aeskey);
	
	uchar ivi[MAXSIZE];
	strncpy((char*)ivi,(char*)ctui,KEYSIZE);

	//generating the F(k,IV)...
	uchar fui[MAXSIZE];
	//although each key and frame is 16byte, each byte need two char to represented, so we need to move at 32 byte
	for(long i=0;i<(int)strlen((char*)ct)/2-KEYSIZE;i=i+KEYSIZE) {
		AES_decrypt(ivi,fui+i,&aeskey);
		printf("input\n");
		dispuis(ivi,KEYSIZE);
		stradd1(ivi,ivi,KEYSIZE);
		printf("output\n");
		dispuis(ivi,KEYSIZE);
	}
	uchar finalres[MAXSIZE] ;
	strxor(ctui+KEYSIZE,fui,finalres,(int)strlen((char*)ct)/2-KEYSIZE);
	uchar finalres1[MAXSIZE] ;
	uis2chars(finalres,finalres1,(int)strlen((char*)ct)/2-KEYSIZE);
	finalres1[strlen((char*)ct)-KEYSIZECHAR]=(uchar)0;
	printf("finalres len %d\n is : %s\n",strlen((char*)finalres),finalres);
}

int main(int argc, char *argv[]) {
	uchar ct1[] = "4ca00ff4c898d61e1edbf1800618fb2828a226d160dad07883d04e008a7897ee2e4b7465d5290d0c0e6c6822236e1daafb94ffe0c5da05d9476be028ad7c1d81";
	uchar key1[] = "140b41b22a29beb4061bda66b6747e14";
	cbc_aes(ct1,key1);

	uchar ct2[]= "5b68629feb8606f9a6667670b75b38a5b4832d0f26e1ab7da33249de7d4afc48e713ac646ace36e872ad5fb8a512428a6e21364b0c374df45503473c5242a253";
	uchar key2[] = "140b41b22a29beb4061bda66b6747e14" ;
	cbc_aes(ct2,key2);

	uchar ct3[] = "69dda8455c7dd4254bf353b773304eec0ec7702330098ce7f7520d1cbbb20fc388d1b0adb5054dbd7370849dbf0b88d393f252e764f1f5f7ad97ef79d59ce29f5f51eeca32eabedd9afa9329" ;
	uchar key3[]= "36f18357be4dbd77f050515c73fcf9f2";
	ctr_aes(ct3,key3);

	uchar ct4[]="770b80259ec33beb2561358a9f2dc617e46218c0a53cbeca695ae45faa8952aa0e311bde9d4e01726d3184c34451";
	uchar key4[]="36f18357be4dbd77f050515c73fcf9f2";
	ctr_aes(ct4,key4);
}


