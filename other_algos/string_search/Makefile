OBJS := queue.o string_search.o

.PHONY: all
all: string_search

string_search: $(OBJS)
	gcc -g -o $@ $(OBJS)

%.o:%.c
	gcc -g -c $< -o $@

clean:
	rm $(OBJS) string_search
