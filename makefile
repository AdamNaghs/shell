CC = clang
CFILES = main.c IO.c string.c shell.c signal.c credentials.c cmd.c builtins.c
CFLAGS = -Wall -g -fsanitize=address -std=c99 -pedantic
LFLAGS = 
SRCFOLDER = src/
OBJFOLDER = obj/
OUTNAME = main
OBJS = $(CFILES:%.c=$(OBJFOLDER)%.o)
RM = rm
OUT_TYPE = 

default: $(OUTNAME)

$(OUTNAME): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) $(CFLAGS) -o $(OUTNAME)$(OUT_TYPE)

$(OBJFOLDER)%.o: $(SRCFOLDER)%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	$(RM) $(OUTNAME).exe
	$(RM) $(OUTNAME)
	$(RM) $(OUTNAME).exp
	$(RM) $(OUTNAME).lib
	$(RM) $(OUTNAME).pdb