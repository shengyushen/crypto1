#include<iostream>
#include<fstream>
#include<assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include"../cryptopp700/sha.h"


using namespace std;
using namespace  CryptoPP;


long GetFileSize(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

void ComputeSHA256At1KSize(word32* pDigest,word32 * pAddr,long n64B) {
  word32 s[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
	for(long i=0;i<n64B;i=i+1) {
		SHA256::Transform(s,pAddr+16*i);
	}
	for(long i=0;i<8;i++) {
		pDigest[i]=s[i];
	}
}

int main(int argc, char *argv[]) {
	assert(argc==2);
	char * filename = argv[1];

	cout<<"file to be checked is "<<filename<<endl;

	ifstream myfile(filename,ios::in|ios::binary);
	std::vector<char> ssy;
	char x;
	while(!myfile.eof()) {
		myfile >> x;
		ssy.push_back(x);
	}
	long lFileSize=ssy.size();
	cout<<"ssy size is "<<lFileSize<<endl;
	long lNumber1K = lFileSize/1024 ;
	long lBufBound1K = lNumber1K*1024;

	long lNumber64B = lFileSize/64 ;
	long lBufBound64BFull = (lNumber64B+1)*64;
	//padding with 0 to reach 64 byte 
	long i;
	for(i=0;i<lBufBound64BFull-lFileSize;i++)
		ssy.push_back(0);
	cout<<"expanding ssy to 64 boundary i="<<i<<endl;

	word32 * pssy32=(word32 *)&ssy[0];
	std::vector<word32> buffer(lBufBound64BFull/4,(word32)0);
	for(i = 0; i<(ssy.size())/4;i++) {
		buffer[i]=pssy32[i];
	}
	cout<<"copying to buffer i="<<i<<endl;
	SHA256 * pSHA256=new SHA256();
	cout<<pSHA256->StaticAlgorithmName()<<endl;
	cout<<"block size "<<SHA256::BLOCKSIZE<<endl;

	
	//first compute the has value for the last block less than 1K
	word32 pDigest[8];
	ComputeSHA256At1KSize(pDigest,&(buffer[0])+lBufBound1K/4,(lBufBound64BFull-lBufBound1K)/64);
	for(long startAddr=lBufBound1K/4;startAddr>=0;startAddr=startAddr-1024/4) {
		cout<<"working on "<<startAddr<<endl;
		//extract the current 1KB
		vector<word32>::const_iterator first=buffer.begin()+startAddr;
		vector<word32>::const_iterator last =buffer.begin()+startAddr+1024/4;
		vector<word32>  newV(first,last);
		//append the last s
		for(long i=0;i<8;i++) {
			newV.insert(newV.end(),pDigest[i]);
		}
		ComputeSHA256At1KSize(pDigest,&(newV[0]),16);
	}
	cout<<"final res is "<<endl;
	for(long i=0;i<8;i++) {
		cout<<pDigest[i]<<endl;
	}
	return 1;
}
