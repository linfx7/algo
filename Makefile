TARGET 		:= run
CC 			:= g++
OBJS 		:= main.o tss.o fc.o rbfc.o
CXXFLAGS 	:= 

all: $(TARGET)

rebuild: clean all

clean:
	rm -fr *.o

depclean:
	rm -fr *.o
	rm -fr $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $(OBJS)
