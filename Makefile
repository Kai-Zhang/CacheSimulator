CC	= g++
LD	= ld
CFLAGS	= -MD -Wall -Werror -fno-strict-aliasing -I./include -O2

CFILES	= $(shell find src/ -name "*.cpp")
HEADERS	= $(shell find include/ -name "*.h")
OBJS	= $(CFILES:.cpp=.o)

%.o: %.cpp $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

cache: $(OBJS)
	$(CC) -o cache $(OBJS) $(CFLAGS)

-include $(OBJS:.o=.d)

.PHONY: clean

clean:
	-rm -f cache $(OBJS) $(OBJS:.o=.d) 2> /dev/null
