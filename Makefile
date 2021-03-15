FLAGS = -Wunused -mfpmath=sse -fstack-protector-all -W -Wall -Wextra -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format -O3

all: a.out

a.out: functions.o main.o
	g++ functions.o main.o -lpthread -o a.out

functions.o: functions.cpp functions.h class_args.h
	g++ -c $(FLAGS) functions.cpp

main.o: main.cpp functions.h class_args.h
	g++ -c $(FLAGS) main.cpp

clean:
	rm -rf *.0 a.out
