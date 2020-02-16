#
# Makefile for conversd "X-Bot" channel robot
#   written by YO2LOJ, VE3OY [FEB-2020]
#   based on original work of
#   Carl Makin and Adam Roach
#

#
# What to use for compiler/linker.
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

install:
	@echo "Any running X-Bot process will now be killed"
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

:
x-bot: x-bot.o
	@echo "Compiling ..."
	${CC} ${CFLAGS} x-bot.o -o x-bot
	strip x-bot
	@echo "Done."

