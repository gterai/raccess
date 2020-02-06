BIG_INLINE_LIMIT = 20000
CFLAGS1 = 

CC = g++
# CC = icpc

SRC_DIR  = ../
UTIL_DIR = $(SRC_DIR)/util/

INCLUDES  = -I$(SRC_DIR) -I$(UTIL_DIR)

CFLAGS = -O3 -g3 -pg -Wall $(INCLUDES) $(CFLAGS1)
LIBS = -lz
CFLAGS_D = -O0 -g3 -pg -Wall -DDEBUG                         \
	   -fkeep-inline-functions -fkeep-static-consts      \
	   -finline-limit=0 -fno-default-inline $(CFLAGS1)
LIBS_D = -lz

.PHONY: clean depend all all_mpi
.SUFFIXES: .cpp .hpp .o .o_D
all: all_

SRCS0    = 
EXES_ALL =
SRCS_ALL =

MPICC = mpic++
EXES_ALL_MPI =
SRCS_ALL_MPI =

all_: $(EXES_ALL)

all_mpi: util_mpi $(EXES_ALL_MPI)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<
.cpp.o_D:
	$(CC) $(CFLAGS_D) $(INCLUDES) -o $@ -c $<
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<
.c.o_D:
	$(CC) $(CFLAGS_D) $(INCLUDES) -o $@ -c $<
clean:
	rm -rf core {.,*}/{*.o,*.o_*,*.da,*.bb,*.bbg,*.gcda,*.gcno,*.gcov,*.stackdump,*.exe}
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


