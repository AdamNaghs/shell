CC = gcc
CFILES = main.c IO.c string.c shell.c signal.c
CFLAGS = -Wall -Werror -g
LFLAGS = 
OUTNAME = main
default:
	$(CC) $(CFILES) $(CFLAGS) $(LFLAGS) -o $(OUTNAME)