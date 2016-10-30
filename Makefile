
DEBUG = -g
COMPILE = g++
ALL = *.cpp

main.out:main.cpp
	$(COMPILE) $(DEBUG) $(ALL) -lncurses -o main.out -Wall

run:main.out
	./main.out

clean:
	rm *.out
