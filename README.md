# ASN Shell

This is a simple shell that I made for educational purposes and for fun.
I've made a simpler version of a shell before so this time I challenged myself
by supporting Linux & Windows and programs in the system PATH instead of just
builtin commands.

I also intend on eventually adding a simple shell scripting language.

# Crossplatform

I don't have much experience with the windows api so at the moment I am focussing on implemeting the windows version of some functions before the linux verison.

I am also developing this on windows and while im not sure how useful this shell will be I think it would be more useful to me if it ran on my native platform first.

# Building
Create a folder named 'obj' in the same folder as the 'src' and 'include' folder.


### Linux
1. Remove '.exe' from OUT_TYPE.
2. replace '\\' with '/'
3. Set RM to 'rm'
### Windows
1. Set OUT_TYPE to '.exe'
2. replace '/' with '\\'
3. Set RM to 'del'

I'm using a makefile to build and clean.

When building, call 'make' while in the same directory as the makefile.

After changing the code, call 'make clean' then 'make' to rebuild the objs and the exe.

I'm currently only using the address sanitizer and -g to debug. Please let me know what other flags I should add while compiling or what other programs I can use to debug my code (aside from gdb).


# Notes
Not sure whether or not I should store the external programs found in the path or if I should just call osys whenever the shell doesn't recognize a command.

I think another project I may do is a terminal written in a graphics library and then I'll try integrating my shell.
# Commands

So far ASN supports a few simple builtin commands

    help - Prints this message to stdout.
    exit - Exits program.
    echo - Prints message.
    osys - Outer system/shell call.
    clear - Wipes terminal.
    cd - Change directory.
    ls - List files in current directory.
    pwd - Print working directory.
    mkdir  - Creates new direction with provided path.
    rm - Removes files or directories.
    touch - Creates files.


Some commands like 'rm' and 'clear' just use the system function to call 'del' or 'rm' depending on if you're using windows or not. I need to fix this, but for now its just a simple solution.

I also recently added support for programs in the system path.

The commands and programms are all internally stored in an array.

The internal commands call a command from the builtins.c file.
The external commands call a fascade command from cmd.c which is basically equavilent to using osys which captures the output of the command you called outside of the shell and returns it for viewing.

# Strings

In my code I've tried to treat strings as immutable, but thats made a lot of clutter and eventually I broke that personal rule anyway and made a str_append function.

For the most part, all strings need to be freed except for some places in my code where I explicitely made a String on the stack.

I've always hated how char* strings in C need you to parse them to determine their length so my idea in implementing my own string was so lessen some of that headache. However it is not a complete string library and I lean on the std <string.h> header a lot.

The way I've implemented and used Strings is also probably the source of a lot of memory leaks.

# Deprecated functions
At some places in my code I am using deprecated functions. I intend to go back in and replace these but please feel free to still create issue's about them

I also would like to make my code compile to ansi C so I am not sure how to get around using some of the deprecated functions.

I realize that my code does not currently compile to ansi C, but I just need to restructure the for loops for the most part and maybe take out some newer functions.

# Login Security
I've never created a login before so I might save this for last.

My biggest worry is finding a place to store the encrypted login information that would be out of reach from attackers but still in reach for my program.

I don't want to have any dependancies besides the libc and my own headers so I will likely implement my own encyption methods when the time comes.

# Colored output
The colors don't work in some terminals. I am not yet sure how I can check if a terminal has them enabled yet to handle replacing all the colors to something else.

# Known Issues

1.  osys and external commands do not mix stdin, stdout, and stderr while running a process.
2.  osys and external commands sometimes stop, or appear to, perhaps becuase of the lack of file stream mixing.
# Todo
### All Versions
    1. Get login working (credentials.h/c)
    2. Get mixed input and output from caputure_system_call working. (cmd.c)
    3. getenv is deprecated (cmd.c)
    4. Find out if _dupenv_s is unix compatible
    5. Find ansi C and crossplatform way to replace getenv
### Unix
    1. Implement load_external_from_folder function (cmd.c)
    2. Debug and Fix b_ls
### Windows
    1. 

# Debug Macros
Comment out any of these macros and rebuild to disable these features.
    LOAD_EXTENALS_DEBUG - If enabled, it will cause all of the programs in the path to be printed when they are added.
    
    COLOR_ENABLED - Enabled colors.
