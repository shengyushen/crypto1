OBJS = \
w3.o 

LINKTARGET = w3.exe

all : $(LINKTARGET)

clean :
	rm -f *.o *.exe *.stackdump

$(LINKTARGET) : $(OBJS)
	g++ -o $@ $^ -lcrypto 

%.o : %cpp
	g++ -o $@ -c $<



