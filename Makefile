LINKTARGET = w3.exe w21.exe w5.exe w61.exe

all : $(LINKTARGET)

clean :
	rm -f *.o *.exe *.stackdump

%.exe : %.o
	g++ -std=c++11 -o $@ $^ -lcrypto -lgmpxx -lgmp

%.o : %.cpp
	g++ -std=c++11 -o $@ -c $<



