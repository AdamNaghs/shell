CC = gcc
CFILES = main.c IO.c string.c shell.c signal.c
CFLAGS = -Wall -Werror -g 
LFLAGS = 
SRCFOLDER = src\\
OBJFOLDER = obj\\
OUTNAME = main
OBJS = $(CFILES:%.c=$(OBJFOLDER)%.o)
RM = del


default: $(OUTNAME)

$(OUTNAME): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) $(CFLAGS) -o $(OUTNAME)

$(OBJFOLDER)%.o: $(SRCFOLDER)%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(CFLAGS)

clean:
	$(RM) $(OBJS)
	$(RM) $(OUTNAME).exe
	$(RM) $(OUTNAME)