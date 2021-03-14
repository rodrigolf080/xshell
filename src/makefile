.PHONY: all
xsh: xsh.c utils.c
	gcc xsh.c utils.c -lreadline -o xsh

.PHONY: clean
clean:
	rm -f xsh

.PHONY: debug
debug:
	gcc -g -Wall -Wextra xsh.c utils.c -lreadline -o xsh 
