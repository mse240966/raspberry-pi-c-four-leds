all: main.c
	gcc -Wall -O2 -std=c99 -std=gnu99 -std=gnu11 -c GPIO/gpio.c -o gpio.o
	gcc -Wall -O2 -std=c99 -std=gnu99 -std=gnu11 -c main.c -o main.o
	g++ -o fourLeds gpio.o main.o -s
	rm gpio.o main.o
