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
	} else 
		assert(0);
}

unsigned char ui2char(unsigned int ui) {
	if(ui>=0 && ui<=9) {
		return (unsigned char)((int)'0'+ui);
	} else if(ui>=10 && ui<=15) {
		return (unsigned char)((unsigned int)'a'+ui-10);
	} else
		assert(0);
}

struct  UnsignedShortArray{
	long lLength;
	unsigned char* pArray;
};

typedef struct UnsignedShortArray UnsignedShortArray_t;
//converting two char to an unsigned int in one char position
unsigned char TwoHex2UnsignedShort(unsigned char * p) {
	//convert two char "1e" to 0x 1e
	unsigned int high = char2ui(p[0]);
	unsigned int low  = char2ui(p[1]);
	return (unsigned char)(high*16+low);
}

UnsignedShortArray_t * HexString2ShortArray(unsigned char * p) {
	long lLengthp=strlen((char*)p);
	assert(lLengthp%2==0);
	unsigned char *x = (unsigned char *)malloc(lLengthp/2);
	for(long i=0;i<lLengthp/2;i++) {
		x[i]=TwoHex2UnsignedShort(p+2*i);
	}
	struct  UnsignedShortArray * pus= (struct UnsignedShortArray *)malloc(sizeof(struct UnsignedShortArray));
	pus->lLength=lLengthp/2;
	pus->pArray = x;
	return pus;
}

void printShortArray(UnsignedShortArray_t * pus) {
	printf("len=%d ",pus->lLength);
	for(long i=0;i<pus->lLength;i++) {
		printf("%02X",(unsigned int)(pus->pArray[i])%256);
	}
	printf("\n");
}

void freeShortArray(UnsignedShortArray_t * pus)  {
	free(pus->pArray);
	free(pus);
}

int main(int argc, char *argv[]) {
	unsigned char ct[] = "4ca00ff4c898d61e1edbf1800618fb2828a226d160dad07883d04e008a7897ee2e4b7465d5290d0c0e6c6822236e1daafb94ffe0c5da05d9476be028ad7c1d81";
	unsigned char key[] = "140b41b22a29beb4061bda66b6747e14";
	
	struct  UnsignedShortArray * pkeyus  = HexString2ShortArray(key);
	printf("key in hex : \n");
	printShortArray(pkeyus);

	struct  UnsignedShortArray * pctus  = HexString2ShortArray(ct);
	printf("ct in hex : \n");
	printShortArray(pctus);

	
}


