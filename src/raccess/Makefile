BIG_INLINE_LIMIT = 20000
CFLAGS1 = 

CC = g++
# CC = icpc

SRC_DIR   = ../

INCLUDES = -I$(SRC_DIR)

 # CFLAGS = -O3 -Wall -finline-limit=$(BIG_INLINE_LIMIT) $(CFLAGS1)
CFLAGS = -O3 -g -Wall -std=c++03 $(CFLAGS1)
# CFLAGS = -O2 -Wall $(CFLAGS1)

LIBS     = -lz
CFLAGS_D = -O0 -g3 -pg -Wall -std=c++03 -DDEBUG              \
	   -fkeep-inline-functions -fkeep-static-consts      \
	   -finline-limit=0 -fno-default-inline $(CFLAGS1)
LIBS_D = -lz

.PHONY: clean depend sync all util all_mpi util_mpi
.SUFFIXES: .cpp .hpp .o .o_D .c
all: all_

SRCS0    = 
EXES_ALL =
SRCS_ALL =
include run_raccess.mk
include run_raccess_contrafold.mk

ifneq ($(strip $(SCBDSERV)),)
MAGI=true
endif

ifeq ($(strip $(MAGI)),)
 MPICC = mpicxx
else
 MPICC = mpic++
 LIBS += -lpthread
endif
EXES_ALL_MPI =
SRCS_ALL_MPI =

all_: $(EXES_ALL)

all_mpi: util_mpi $(EXES_ALL_MPI)

util:
	(cd $(UTIL_DIR); make)
util_mpi:
	(cd $(UTIL_DIR); make all_mpi)
.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<
.c.o_D:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<
.cpp.o_D:
	$(CC) $(CFLAGS_D) $(INCLUDES) -o $@ -c $<
clean:
	rm -rf core {.,*}/{*.o,*.o_*,*.da,*.bb,*.bbg,*.gcda,*.gcno,*.gcov,*.stackdump}
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


