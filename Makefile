# THIS IS A MAKEFILE 3
# Save the file with name "Makefile" #

## DEFINE COMMAND ARGUMENTS

# C language compiler
CC= clang
CFLAGS= -ggdb3 -gdwarf-4 -O0 -std=c11

# Warning Error Flags
WFLAGS= -Wall -Werror -Wextra -Wno-gnu-folding-constant -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow

# Flags to include Math.h library
MATHFLAG= -lm

# Flags to include CS50.h library
CS50FLAG= -lcs50

# filename = execute Make file

# execute the command every time without deleting a execute filename
.PHONY: contact clean


#***** METHOD 1 *****#

# main:
# 		$(CC) $(CFLAGS) $(WFLAGS) main.c $(CS50FLAG) $(MATHFLAG) -o main

# clean:
# 		rm -f *.~ main


#****** METHOD 2 ******#
# -c means only execute one if there's no change in a file

contact: main.o contact.o
		$(CC) $(CFLAGS) $(WFLAGS) main.o contact.o $(CS50FLAG) $(MATHFLAG) -o contact

main.o: main.c contact.h 
		$(CC) $(CFLAGS) $(WFLAGS) -c main.c

contact.o: contact.c contact.h 
		$(CC) $(CFLAGS) $(WFLAGS) -c contact.c

clean:
		rm -f *.o contact