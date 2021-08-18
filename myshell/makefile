# Rodrigo Ferreira, M00736217 - Operating Systems CW01
# myshell - command line interactive shell built in C
CXX = gcc
CXXFLAGS = # -g  -Wall -Wextra
RM = rm -rf 

.PHONY : all
all : myshell

myshell: myshell.c
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY : clean
clean:
	$(RM) myshell
