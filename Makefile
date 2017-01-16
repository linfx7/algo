TARGET 		:= run
CC 			:= g++
OBJS 		:= main.o tss.o
CXXFLAGS 	:= 

all: $(TARGET)

rebuild: clean all

clean:
	rm -fr *.o
	rm -fr $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $(OBJS)
