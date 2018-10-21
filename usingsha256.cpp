#include<iostream>
#include<iomanip>
#include<fstream>
#include<assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include <openssl/sha.h>

using namespace std;

unsigned int char2ui(char c) {
	unsigned int uc= (unsigned int)c;
	return uc-uc/256*256;
}
void sha256(const std::vector<char> * pData,std::vector<char> * pDigest)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, &((*pData)[0]), pData->size());
    SHA256_Final(hash, &sha256);
		pDigest->clear();
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        pDigest->push_back(hash[i]);
    }
		assert(pDigest->size()==32);
}


int main(int argc, char *argv[]) {
	assert(argc==2);
	char * filename = argv[1];

	cout<<"file to be checked is "<<filename<<endl;

	//reading file into ssy
	ifstream myfile;
	myfile.open(filename,ios::binary);
	std::vector<char> ssy;
	char x;
	while(myfile.read((char*)&x,1)) {
		ssy.push_back(x);
	}

	//first compute the has value for the last block less than 1K
	std::vector<char> tmp1;
	std::vector<char> digest;
	cout<<dec<<"ssy size "<<ssy.size()<<endl;
	cout<<dec<<"last 1K start addr "<<ssy.size()/1024*1024<<endl;
	for(long i = ssy.size()/1024*1024 ; i < ssy.size() ;i++) {tmp1.push_back(ssy[i]);}
	sha256(&tmp1,&digest);

	// then backword computing to the first block
	for(long startAddr=ssy.size()/1024*1024-1024;startAddr>=0;startAddr=startAddr-1024) {
		std::vector<char> tmp;
		for(long i = startAddr ; i < startAddr + 1024 ;i++) {tmp.push_back(ssy[i]);}
		for(long i = 0 ; i < digest.size() ; i++) {tmp.push_back(digest[i]);}
		sha256(&tmp,&digest);
	}

	cout<<"final res is "<<endl;
	for(long i=0;i<32;i++) {
		cout<<hex<<setw(2)<<setfill('0')<<char2ui(digest[i]);
		cout<<dec;
	}
	cout<<dec<<endl;

/*	std::vector<char>tmp11;
	char x1[]="1234567890_1";	
	for(long i=0;i<sizeof(x1);i++) {tmp11.push_back(x1[i]);}
	sha256(&tmp11,&digest) ;
	cout<<"final res is "<<endl;
	for(long i=0;i<32;i++) {
		cout<<hex<<setw(2)<<setfill('0')<<char2ui(digest[i])<<endl;
		cout<<dec;
	}
	cout<<dec;
*/
	return 1;
}

