all:
	gcc -Wall -c Hamming.c
	g++ testing.cpp Hamming.o -lgtest -lgtest_main -pthread -o testing

clean:
	rm -f *.o testing