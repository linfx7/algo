objs := tss.o main.o

default: $(objs)

tss.o:
	gcc -c tss.c

main.o:
	gcc -c main.c

default:
	gcc $(objs) -o run

clean:
	rm tss.o main.o run
