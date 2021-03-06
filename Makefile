TESTS = test_cpy test_ref

TEST_DATA = s Tai

CFLAGS = -O0 -Wall -Werror -g

# Control the build verbosity                                                   
ifeq ("$(VERBOSE)","1")
    Q :=
    VECHO = @true
else
    Q := @
    VECHO = @printf
endif

GIT_HOOKS := .git/hooks/applied

.PHONY: all clean

all: $(GIT_HOOKS) $(TESTS)

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

OBJS_LIB = \
    tst.o bloom.o 

OBJS := \
    $(OBJS_LIB) \
    test_cpy.o \
    test_ref.o \

deps := $(OBJS:%.o=.%.o.d)

test_%: test_%.o $(OBJS_LIB)
	$(VECHO) "  LD\t$@\n"
	$(Q)$(CC) $(LDFLAGS)  -o $@ $^ -lm

%.o: %.c
	$(VECHO) "  CC\t$@\n"
	$(Q)$(CC) -o $@ $(CFLAGS) -c -MMD -MF .$@.d $<

test:  $(TESTS)
	echo 3 | sudo tee /proc/sys/vm/drop_caches;
	perf stat --repeat 100 \
                -e cache-misses,cache-references,instructions,cycles \
                ./test_cpy --bench $(TEST_DATA)
	perf stat --repeat 100 \
                -e cache-misses,cache-references,instructions,cycles \
				./test_ref --bench $(TEST_DATA)

bench: $(TESTS)
	@for test in $(TESTS); do\
		./$$test --bench $(TEST_DATA); \
	done

plot: $(TESTS) res/case_*.txt
	python gen_test.py
	./test_cpy --bench
	./test_ref --bench
	gnuplot scripts/bloom_err_rate.gp
	gnuplot scripts/runtime3.gp
	eog plot/runtime3.png
	eog plot/bloom_err_rate.png

perf: $(TESTS)
	./test_cpy --bench
	./test_ref --bench
	perf record -o cpy_perf.data -e cpu-cycles ./test_cpy --bench	
	if [ -f res/cpy_perf.txt  ] ; \
		then \
		rm res/cpy_perf.txt ; \
	fi;
	perf report -F +period,overhead -i cpy_perf.data | sed '/^#/ d' | sed '/^$$/d' >> res/cpy_perf.txt 
	perf record -o ref_perf.data -e cpu-cycles ./test_ref --bench	
	if [ -f ref_res/perf.txt  ] ; \
		then \
		rm ref_res/perf.txt ; \
	fi;
	perf report -F +period,overhead -i ref_perf.data | sed '/^#/ d' | sed '/^$$/d' >> res/ref_perf.txt 

clean:
	$(RM) $(TESTS) $(OBJS)
	$(RM) $(deps)
	rm -f  res/*

-include $(deps)
