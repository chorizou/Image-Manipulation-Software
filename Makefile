#gwang46, czou9
#Makefile


# Flags
CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra

# Links together files needed for executable
project: project.o imageManip.o ppm_io.o
	$(CC) -o project project.o imageManip.o ppm_io.o -lm

# Compiles project.c to create project.o
project.o: project.c imageManip.h ppm_io.h
	$(CC) $(CFLAGS) -c project.c -g -o project.o

# Compiles imageManip.c to create imangeManip.o
imageManip.o: imageManip.c imageManip.h
	$(CC) $(CFLAGS) -c imageManip.c -g -lm

# Compiles ppm_io.c to create ppm_io.o
ppm_io.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm_io.c -g -o ppm_io.o

# Removes all object files and executable
clean:
	rm -f *.o project





