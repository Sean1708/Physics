PROFILE_FLAGS= -O0 -pg -fprofile-arcs -ftest-coverage 
OPT_FLAGS = -O3 -march=native
NO_OPT_FLAGS = -O0
WARN_FLAGS = -Wall -Wextra -pedantic
C_STD_FLAGS = -ansi
F_STD_FLAGS = -std=f95

simple_md: simple_md.f90
	gfortran -o simple_md $(NO_OPT_FLAGS) simple_md.f90

simple_md_pg: simple_md.f90
	gfortran -o simple_md_pg $(PROFILE_FLAGS) $(WARN_FLAGS) $(F_STD_FLAGS) simple_md.f90

simple_md_opt: simple_md.f90
	gfortran -o simple_md_opt $(OPT_FLAGS) simple_md.f90

simple_md_C: simple_md.c md_stuff.c
	gcc -o simple_md_C.o $(NO_OPT_FLAGS) -c simple_md.c
	gcc -o md_stuff_C.o  $(NO_OPT_FLAGS) -c md_stuff.c
	gcc -o simple_md_C   $(NO_OPT_FLAGS) simple_md_C.o md_stuff_C.o -lm

simple_md_C_pg: simple_md.c md_stuff.c
	gcc -o simple_md_C_pg.o $(PROFILE_FLAGS) $(WARN_FLAGS) $(C_STD_FLAGS) -c simple_md.c
	gcc -o md_stuff_C_pg.o  $(PROFILE_FLAGS) $(WARN_FLAGS) $(C_STD_FLAGS) -c md_stuff.c
	gcc -o simple_md_C_pg   $(PROFILE_FLAGS) $(WARN_FLAGS) $(C_STD_FLAGS) simple_md_C_pg.o md_stuff_C_pg.o -lm

simple_md_C_opt: simple_md.c md_stuff.c
	gcc -o simple_md_C_opt.o $(OPT_FLAGS) -c simple_md.c
	gcc -o md_stuff_C_opt.o  $(OPT_FLAGS) -c md_stuff.c
	gcc -o simple_md_C_opt   $(OPT_FLAGS) simple_md_C_opt.o md_stuff_C_opt.o -lm

tar:
	tar -cvf simple_md.tar *.f90 *.c *.h Makefile

clean:
	rm -f simple_md simple_md_pg simple_md_opt simple_md_C simple_md_C_pg simple_md_C_opt 
	rm -f *.o *.mod
	rm -f gmon.out *.gcno *.gcov *.gcda
