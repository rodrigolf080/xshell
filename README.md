# xshell
"xshell" or "xsh" is an Open Source Unix Shell built in C by forking a child process to execute the job, using GNU libreadline-dev, execvp system call for non built in commands and allows piping and background processes.

Dependencies:
GNU Readline libreadline-dev package
$ sudo apt-get install libreadline-dev

Quick Start:
$ chmod +x run
$ ./run

note: "run" is a bash script that compiles the program using gcc and a makefile, clears the screen and executes the binary lauching the shell

Commands:
-- exit :: quit the program
-- cd :: C function to change directory chdir()
-- help :: displays the list of commands and functions
-- clear :: prints a string to clear the screen
