OPTIMIZE = 2
WERROR = -Werror

CFLAGS = -std=c99 -Iinclude -O$(OPTIMIZE)
CFLAGS += -Wall -Wextra $(WERROR) -Wpedantic -Wconversion -Wnonnull -Wstrict-prototypes

TEST_BINARIES = bin/c/test_deque bin/c/test_stack bin/c/test_queue
TEST_BINARIES += bin/c/test_array bin/c/test_str bin/c/test_list
TEST_BINARIES += bin/c/test_avltree bin/c/test_set bin/c/test_map
TEST_BINARIES += bin/c/test_unordered_set bin/c/test_unordered_map

BENCHMARK_BINARIES = bin/c/benchmark_c_ds bin/cpp/benchmark_cpp_ds

#.SECONDARY: $(OBJS)

all: $(TEST_BINARIES) $(BENCHMARK_BINARIES)

debug: OPTIMIZE = 0
debug: CFLAGS += -g -DDEBUG
debug: $(TEST_BINARIES) $(BENCHMARK_BINARIES)

scan: OPTIMIZE = 2
scan: WERROR = 
scan: CFLAGS += -g -fanalyzer -DDEBUG
scan: $(TEST_BINARIES)

test: $(TEST_BINARIES)
	@bash bin/run_tests.sh

benchmark: $(BENCHMARK_BINARIES)
	@python3 bin/run_benchmarks.py

bin/c/test_unordered_%: tests/test_unordered_%.c include/unordered_%.h
	gcc $(CFLAGS) -o $@ $< src/hash.c

bin/c/test_str: tests/test_str.c include/str.h src/str.c
	gcc $(CFLAGS) -o $@ $< src/str.c

bin/c/test_%: tests/test_%.c include/%.h
	gcc $(CFLAGS) -o $@ $<

bin/c/benchmark_%: tests/benchmark_%.c include/array.h include/list.h
	gcc $(CFLAGS) -o $@ $<

bin/cpp/%: tests/%.cpp
	g++ -std=c++11 -O2 -Wall -Wextra -Werror -o $@ $<


clean:
	rm -f $(TEST_BINARIES) $(BENCHMARK_BINARIES)
