OBJS = \
usingsha256.o 

LINKTARGET = usingsha256.exe

all : $(LINKTARGET)

clean :
	rm -f *.o *.exe

$(LINKTARGET) : $(OBJS)
	g++ -o $@ $^ libcryptopp.a

%.o : %cpp
	g++ -o $@ -c $<



