CFLAGS := -Wall -Wextra -g -fsanitize=address,undefined -MMD -MP

OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c))

main: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o main

run: main
	ASAN_OPTIONS=detect_leaks=0 ./main

clean:
	rm -f *.o *.d main

-include $(OBJECTS:.o=.d)
