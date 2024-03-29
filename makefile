CC = gcc
CFILES = main.c IO.c string.c shell.c signal.c credentials.c cmd.c builtins.c utils.c var.c string_array.c tokenize.c
CFLAGS = -Wall  -g -std=c99 -pedantic 
LFLAGS = 
OUTNAME = asn
OBJS = $(CFILES:%.c=$(OBJFOLDER)%.o)


OS := $(shell uname -s)

ifeq ($(OS),Linux)
    # Commands for Linux
	SRCFOLDER = src/
	OBJFOLDER = obj/
	RM = rm
	OUT_TYPE = 
else
    # Commands for Windows or others
	SRCFOLDER = src\\
	OBJFOLDER = obj\\
	RM = del
	OUT_TYPE = .exe
endif


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
	$(RM) a.out

fuzz:
	afl-clang-fast -g -fsanitize=address,undefined fuzz_test.c -std=c99 $(OBJS)
	