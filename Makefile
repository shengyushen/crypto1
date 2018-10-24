LINKTARGET = w3.exe w21.exe

all : $(LINKTARGET)

clean :
	rm -f *.o *.exe *.stackdump

%.exe : %.o
	g++ -o $@ $^ -lcrypto 

%.o : %.cpp
	g++ -o $@ -c $<



