CC ?= gcc
CFLAGS_common ?= -Wall -std=gnu99 -DNDEBUG
CFLAGS_orig = -O0
CFLAGS_opt1  = -O0 -DOPT1="1"
CFLAGS_opt2  = -O0 -DOPT2="1" -DTABLE_SIZE=10000

EXEC = phonebook_orig phonebook_opt1 phonebook_opt2
all: $(EXEC)

SRCS_common = main.c

phonebook_orig: $(SRCS_common) phonebook_orig.c phonebook_orig.h
	$(CC) $(CFLAGS_common) $(CFLAGS_orig) \
		-DIMPL="\"$@.h\"" -o $@ \
		$(SRCS_common) $@.c

phonebook_opt1: $(SRCS_common) phonebook_opt1.c phonebook_opt1.h
	$(CC) $(CFLAGS_common) $(CFLAGS_opt1) \
		-DIMPL="\"$@.h\""  -o $@ \
		$(SRCS_common) $@.c

phonebook_opt2: $(SRCS_common) phonebook_opt2.c phonebook_opt2.h
	$(CC) $(CFLAGS_common) $(CFLAGS_opt2) \
		-DIMPL="\"$@.h\""  -o $@ \
		$(SRCS_common) $@.c

run: $(EXEC)
	echo 3 | sudo tee /proc/sys/vm/drop_caches
	watch -d -t "./phonebook_orig && echo 3 | sudo tee /proc/sys/vm/drop_caches"

run_opt: $(EXEC)
	echo 3 | sudo tee /proc/sys/vm/drop_caches
	watch -d -t "./phonebook_opt1 && echo 3 | sudo tee /proc/sys/vm/drop_caches"

cache-test: $(EXEC)
	perf stat --repeat 100 \
		-e cache-misses,cache-references,instructions,cycles \
		./phonebook_orig 
	perf stat --repeat 100 \
		-e cache-misses,cache-references,instructions,cycles \
		./phonebook_opt1
	perf stat --repeat 100 \
		-e cache-misses,cache-references,instructions,cycles \
		./phonebook_opt2
output.txt: cache-test calculate
	./calculate 
	@echo "calculate"

plot: output.txt
	gnuplot scripts/runtime.gp
	eog runtime.png

calculate: calculate.c
	$(CC) $(CFLAGS_common) $^ -o $@

.PHONY: clean
clean:
	$(RM) $(EXEC) *.o perf.* \
	      	calculate orig.txt opt1.txt opt2.txt  output.txt runtime.png
