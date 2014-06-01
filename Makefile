
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

HEADERS= gen.H measure-sort.H sort.H defs.H

SRC= rand-perm.C rand-perm-op.C semi-perm.C semi-perm-op.C testmqi.C testqi.C testmi.C testq.C testm.C testsqm.C testmc.C sample.C

EXE=rand-perm rand-perm-op semi-perm semi-perm-op testmqi testqi testmi testq testm testmqi-op testqi-op testmi-op testq-op testm-op testsqm testsqm-op testmc testmc-op sample sample-op

all: $(EXE)

clean:
	rm -f $(EXE)

sample: sample.C $(HEADERS)
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)

sample-op: sample
	$(CXX) $(FLAGS) $(INCLUDE) sample.C -o $@ $(LIBS)

testmqi: testmqi.C $(HEADERS)
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)

testmi: testmi.C $(HEADERS)
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)

testmc: testm.C $(HEADERS)
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)

testmc-op: testmc
	$(CXX) $(FLAGS) $(INCLUDE) testmc.C -o $@ $(LIBS)

testqi: testqi.C $(HEADERS)
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)

testq: testq.C $(HEADERS)
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)

testm: testm.C $(HEADERS)
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)

testsqm: testsqm.C $(HEADERS)
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)

testmqi-op: testmqi
	$(CXX) $(OPT) $(INCLUDE) testmqi.C -o $@ $(LIBS)

testmi-op: testmi
	$(CXX) $(OPT) $(INCLUDE) testmi.C -o $@ $(LIBS)

testqi-op: testqi
	$(CXX) $(OPT) $(INCLUDE) testqi.C -o $@ $(LIBS)

testq-op: testq
	$(CXX) $(OPT) $(INCLUDE) testq.C -o $@ $(LIBS)

testm-op: testm
	$(CXX) $(OPT) $(INCLUDE) testm.C -o $@ $(LIBS)

testsqm-op: testsqm
	$(CXX) $(OPT) $(INCLUDE) testsqm.C -o $@ $(LIBS)

rand-perm: rand-perm.C $(HEADERS)
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)

rand-perm-op: rand-perm
	$(CXX) $(OPT) $(INCLUDE) rand-perm.C -o $@ $(LIBS)

semi-perm: semi-perm.C $(HEADERS)
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS)

semi-perm-op: semi-perm
	$(CXX) $(OPT) $(INCLUDE) semi-perm.C -o $@ $(LIBS)