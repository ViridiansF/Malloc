CFLAGS=-O0 -g -Wall -Wno-unused-parameter -Wno-Wdeprecated-declarations -Wextra -Iinclude
CPPFLAGS=-MMD
LDFLAGS=
LDLIBS=
 
OBJS=tests/test_my_allocator.o src/my_allocator.o
#OBJS=tests/test_allocator.o src/my_allocator.o

DEPS=$(OBJS:.o=.d)
 
.PHONY: all clean
 
all: test_allocator
 
-include $(DEPS)
 
%.o: %.c
	gcc -c $(CFLAGS) $(CPPFLAGS) $< -o $@
 
test_allocator: $(OBJS)
	gcc $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
 
clean:
	rm -f $(OBJS) $(DEPS) test_allocator