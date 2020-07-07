CFLAGS1      = 
CC           = g++
# CC = icpc

SRC_DIR      = ./
SUB_DIRS     = util raccess

INCLUDES     = -I$(SRC_DIR)
CFLAGS       = -O3 -g3 -pg -Wall -std=c++03 $(CFLAGS1)
LIBS         = # -lz
CFLAGS_D     = -O0 -g3 -pg -Wall -std=c++03 -DDEBUG -fkeep-inline-functions -fkeep-static-consts  \
	       -finline-limit=0 -fno-default-inline $(CFLAGS1)
LIBS_D       = # -lz

.PHONY: clean depend all all_mpi depend_all util util_mpi raccess

.SUFFIXES: .cpp .hpp .o .o_D
all: all_

SRCS0    = 
EXES_ALL =
SRCS_ALL =

MPICC = mpic++
LIBS += -lpthread
EXES_ALL_MPI =
SRCS_ALL_MPI =

all_:  $(EXES_ALL) util raccess
all_mpi: util_mpi $(EXES_ALL_MPI)
util:
	(cd util/; make)
raccess:
	(cd raccess/; make)

util_mpi:
	(cd util/; make all_mpi)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<
.cpp.o_D:
	$(CC) $(CFLAGS_D) $(INCLUDES) -o $@ -c $<
clean:
	rm -rf core {.,*,*/*}/{*.exe,*.o,*.o_*,*.da,*.bb,*.bbg,*.gcda,*.gcno,*.gcov,*.stackdump,*.exe}

depend_all:
	for dir in $(SUB_DIRS); \
	do (cd $$dir; touch make_deps_o.mk make_deps_o_D.mk; make depend) done;
	touch make_deps_o.mk make_deps_o_D.mk; make depend

SUFLIST = o o_D 
depend:
	for opt in $(SUFLIST);                      \
	do touch make_deps_$$opt.mk;                \
	   makedepend -o.$$opt -fmake_deps_$$opt.mk \
	   -Y -- $(CFLAGS_D) $(INCLUDES) --         \
	   $(SRCS_ALL) $(SRCS_ALL_MPI);             \
	done
include make_deps_o.mk
include make_deps_o_D.mk


