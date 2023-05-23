CC=g++
CFLAGS=-Wall -pedantic
DEPS = struct_date.h is_date_valid.h is_day_out_of_high_bound.h is_end_later.h is_it_sunday.h is_year_leap.h
OBJ = hodiny_na_vezi.o is_date_valid.o is_it_sunday.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

a.out: $(OBJ)
	$(CC) -o $@ $^

.PHONY: clean

clean:
	rm a.out *.o
