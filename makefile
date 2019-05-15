#makefile for assignment5
#Blessed Chitamba


CC=g++       #compiler
LIBS=-lm     #libraries that are going to be refferenced


samp: samp.o
	$(CC) -o samp samp.o -std=c++11

samp.o: samp.cpp Audio.h
	$(CC) -c -o samp.o samp.cpp -std=c++11

run:
	./samp -r 44100 -b 16 -c 1 -o testAdd.raw -add frogs18sec_44100_signed_16bit_mono.raw siren40sec_44100_signed_16bit_mono.raw

clean:
	rm -f -r *.o samp
