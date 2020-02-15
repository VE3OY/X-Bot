#
# Makefile for conversd "X-Bot" channel robot
#   written by YO2LOJ, VE3OY [FEB2020]
#   based on original work of
#   Carl Makin and Adam Roach
#

#
# What to use for compiler/linker. Someone might prefer egcs.
CC = gcc

#COPTS = -DWANT_LOG
# on MACOSX us this:
#COPTS = -DWANT_LOG
COPTS = -DWANT_LOG -DFORKPTY

#
# Optimisation: -O2 for optimised slightly faster and smaller "production"
# code, -g if you want to run gdb on it or it's cores
#COPTIM = -g
#LDFLAGS = 
#CFLAGS =  -O
COPTIM = -O2 -s

#
# Warnings: If compiling using something than gcc, must usually drop these    
# away.
CWARNS = -Wall -Wstrict-prototypes

# comment the following two lines out if you don't have / want libreadline
# you do not have it on MACOSX
# if readline.h is in /usr/include/, use -DHAVE_READLINE_H,
# if readline.h is in /usr/include/readline, use -DHAVE_READLINE_READLINE_H
#READLINE_INCLUDES = -DHAVE_LIBREADLINE -DHAVE_READLINE_READLINE_H
#READLINE_LIBS = -lreadline -lncurses

install:
	@echo "X-Bot process will now bw killed"
	killall x-bot
	@echo "Deleting x-bot executable from /usr/local/bin"
	rm -f /usr/local/bin/x-bot
	@echo "Copying new x-bot executable to /usr/local/bin"
	cp -f x-bot /usr/local/bin/x-bot
	@echo "Starting new X-Bot process"
	/usr/local/bin/x-bot
	@echo "Done."

clean:
	@echo "Removing any previous build files"
	rm -rf *.o *~ */*~

#.c.o:
#	$(CC) $(CFLAGS) -c $<

:
x-bot: x-bot.o
	@echo "Compiling ..."
	${CC} ${CFLAGS} x-bot.o -o x-bot
	strip x-bot
	@echo "Done."

