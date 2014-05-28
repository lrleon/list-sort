
ALEPH = ~/Dropbox/aleph-w

CLANGPATH=/home/lrleon/LLVM-3.4/bin
CXX = $(CLANGPATH)/clang++

OPT=-D_GLIBCXX__PTHREADS -O3 -DWITHOUT_NANA -std=c++11 $(WARN)

WARN = -Wall -Wextra -Wcast-align -Wno-sign-compare -Wno-write-strings \
       -Wno-parentheses 

FLAGS = -DDEBUG -D_GLIBCXX__PTHREADS -g -O0 -std=c++11 $(WARN)

INCLUDE = -I. -I $(ALEPH) 

LIBS = -L $(ALEPH) \
       -lAleph -lnana -lc -lm -lgsl -lgslcblas -lgmp -lmpfr -lasprintf -lpthread

all: rand-perm rand-perm-op semi-perm semi-perm-op

clean:
	rm -f rand-perm rand-perm-op

rand-perm: rand-perm.C gen.H measure-sort.H
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)

rand-perm-op: rand-perm
	$(CXX) $(OPT) $(INCLUDE) rand-perm.C -o $@ $(LIBS)

semi-perm: semi-perm.C gen.H measure-sort.H
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)

semi-perm-op: semi-perm
	$(CXX) $(OPT) $(INCLUDE) rand-perm.C -o $@ $(LIBS)