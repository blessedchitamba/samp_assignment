#makefile for assignment5
#Blessed Chitamba


CC=g++       #compiler
LIBS=-lm     #libraries that are going to be refferenced


samp: samp.o
	$(CC) -o samp samp.o -std=c++11

samp.o: samp.cpp Audio.h
	$(CC) -c -o samp.o samp.cpp -std=c++11

run:
	#test add with mono and stereo
	echo("---------Testing add with mono and stereo:-----------")
	echo("")
	./samp -r 44100 -b 16 -c 1 -o testAddMono.raw -add frogs18sec_44100_signed_16bit_mono.raw siren40sec_44100_signed_16bit_mono.raw
	./samp -r 44100 -b 16 -c 2 -o testAddStereo.raw -add frogs18sec_44100_signed_16bit_stereo.raw siren40sec_44100_signed_16bit_stereo.raw
	echo("")

	#test cat with mono and stereo
	echo("---------Testing cat with mono and stereo:-----------")
	echo("")
	./samp -r 44100 -b 16 -c 1 -o testConcatMono.raw -cat frogs18sec_44100_signed_16bit_mono.raw siren40sec_44100_signed_16bit_mono.raw
	./samp -r 44100 -b 16 -c 2 -o testConcatStereo.raw -cat frogs18sec_44100_signed_16bit_stereo.raw siren40sec_44100_signed_16bit_stereo.raw
	echo("")

	#test cut with mono and stereo
	echo("---------Testing cut with mono and stereo:-----------")
	echo("")
	./samp -r 44100 -b 16 -c 1 -o testCutMono.raw -cut 396900 790000 frogs18sec_44100_signed_16bit_mono.raw
	./samp -r 44100 -b 16 -c 2 -o testCutStereo.raw -cut 396900 790000 frogs18sec_44100_signed_16bit_stereo.raw
	echo("")
clean:
	rm -f -r *.o samp
