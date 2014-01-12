OBJS := knight_tour.o

.PHONY: all
all: knight_tour

knight_tour: $(OBJS)
	gcc -g -o $@ $(OBJS)

%.o:%.c
	gcc -g -c $< -o $@

clean:
	rm $(OBJS) knight_tour
