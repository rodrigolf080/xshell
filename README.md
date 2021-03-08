# xshell
----
-- Disclaimer:
:: This program is a modified work from multiple scources listed below under the next section
:: Please read the "LICENSE" for more information
----
-- Sources:
:: Blog :: www.geeksforgeeks.org :: https://www.geeksforgeeks.org/making-linux-shell-c/, thank you to Suprotik Dey (LinkedIn :: https://www.linkedin.com/in/suprotik-dey-368706127/)
:: Website :: https://theasciicode.com.ar/extended-ascii-code/box-drawings-single-line-vertical-right-character-ascii-code-195.html
:: Blog :: https://codeforwin.org/2018/03/c-program-to-list-all-files-in-a-directory-recursively.html, thank you to Pankaj (Twitter :: https://twitter.com/pankajprakashh)
----
-- Description:
:: "xshell" or "xsh" is an Open Source Unix Shell built in C by forking a child process to execute the job, using GNU libreadline-dev, execvp system call for non built in commands and allows piping and background processes.
----
-- Dependencies:
:: GNU Readline libreadline-dev package
:: $ sudo apt-get install libreadline-dev
:: $ sudo pacman -S libreadline
----
-- Quick Start:
:: $ chmod +x run
:: $ ./run
----
-- Notes:
:: "run" is a bash script that compiles the program using gcc and a makefile, clears the screen and executes the binary lauching the shell
:: File "dev" holds a running script to catch compilation errors for development
:: Error list
:: :: 0 -- failed to cast execvp on arguments
:: :: 1 -- failed to cast execvp on piped arguments at the write end
:: :: 2 -- failed to cast execvp at the read end
----
-- Commands:
:: exit :: quit the program
:: cd :: C function to change directory chdir()
:: help :: displays the list of commands and functions
:: clear :: prints a string to clear the screen
