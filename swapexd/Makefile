CC=gcc
OUTPUT=swapexd
MODULES=main.o swapexd.o
FLAGS=-DFREEBSD #-DDEBUG

all: $(OUTPUT)

clean:
	rm -f *.o
	rm -f $(OUTPUT)
	rm -f Test

main.o: main.c swapexd.h
	$(CC) -c main.c $(FLAGS)

swapexd.o: swapexd.c swapexd.h
	$(CC) -c swapexd.c $(FLAGS)

$(OUTPUT): $(MODULES)
	$(CC) $(MODULES) -o $(OUTPUT) $(FLAGS)

testswap: test.c
	$(CC) -o Test test.c

install: $(OUTPUT)
	mkdir -p /usr/local/sbin
	cp $(OUTPUT) /usr/local/sbin
	cp -n swapexd.conf /etc

deinstall:
	rm /etc/swapexd.conf
	rm /usr/local/sbin/swapexd

