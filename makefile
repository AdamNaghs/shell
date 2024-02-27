CC = gcc
CFILES = main.c IO.c string.c shell.c signal.c
CFLAGS = -Wall -Werror -g
LFLAGS = 
SRCFOLDER = src\\
OBJFOLDER = obj\\
OUTNAME = main
OBJS = $(CFILES:%.c=$(OBJFOLDER)%.o)
DIR = C:\\Users\\adamn\\Dropbox\\src\\c code\\shell


default: $(OUTNAME)

$(OUTNAME): $(OBJS)
	$(CC) $(LFLAGS) -o $(OUTNAME) $(OBJS)

$(OBJFOLDER)%.o: $(SRCFOLDER)%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del $(OBJS)