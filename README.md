# xshell
----
-- Disclaimer:
:: This program is a modified version of the one made by Suprotik Dey (LinkedIn :: https://www.linkedin.com/in/suprotik-dey-368706127/)
:: The source is available at the website www.geeksforgeeks.org :: https://www.geeksforgeeks.org/making-linux-shell-c/
:: Please read the "LICENSE" for more information
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
-- Note:
:: "run" is a bash script that compiles the program using gcc and a makefile, clears the screen and executes the binary lauching the shell
----
-- Commands:
:: exit :: quit the program
:: cd :: C function to change directory chdir()
:: help :: displays the list of commands and functions
:: clear :: prints a string to clear the screen
