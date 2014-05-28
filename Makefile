
ALEPH = ~/Dropbox/aleph-w

CLANGPATH=/home/lrleon/LLVM-3.4/bin
CXX = $(CLANGPATH)/clang++

OPT=-D_GLIBCXX__PTHREADS -O3 -DWITHOUT_NANA -std=c++11 $(WARN)

WARN = -Wall -Wextra -Wcast-align -Wno-sign-compare -Wno-write-strings \
       -Wno-parentheses 

FLAGS = -DDEBUG -D_GLIBCXX__PTHREADS -g0 -O0 -std=c++11 $(WARN)

INCLUDE = -I. -I $(ALEPH) 

LIBS = -L $(ALEPH) \
       -lAleph -lnana -lc -lm -lgsl -lgslcblas -lgmp -lmpfr -lasprintf -lpthread

all: rand-perf rand-perf-op

clean:
	rm -f rand-perf rand-perf-op

rand-perf: rand-perf.C
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)

rand-perf-op: rand-perf.C
	$(CXX) $(OPT) $(INCLUDE) rand-perf.C -o $@ $(LIBS)