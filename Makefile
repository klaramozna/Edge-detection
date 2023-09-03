# compiling normally, all warnings, debug
all:
	g++ -g -Wall -Wextra -o result main.cpp

# removing executables
clean:
	$(RM) result