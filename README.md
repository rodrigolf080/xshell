# myshell
----
-- Sources:
:: Website :: https://theasciicode.com.ar/extended-ascii-code/box-drawings-single-line-vertical-right-character-ascii-code-195.html
:: GitHub Page :: https://siongui.github.io/2013/01/09/c-remove-string-trailing-newline-carriage-return/
:: Website :: codeforwin.org :: Articles and Resources
:: Website :: www.geeksforgeeks.org :: Articles and Resources
----
-- Description:
:: myshell is a Unix Sihell built in C by forking a child process to execute the job, using GNU libreadline-dev, execvp system call for non built in commands and allows piping and background processes.
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
:: Background processing should be achievied by allowing the user to interact with the shell stdin and stdout (and stderr) while the child process is executing and when its done bringing the  stdout and stderr to the foreground
:: IO redirection should be achieved by parsing user input and duplicating the file descriptors for stdin and/or stdin accordingly to the files following the read write append (<, >, >>) 
:: Error list for exec:
:: :: 0 -- failed to cast execvp on arguments
:: :: 1 -- failed to cast execvp on piped arguments at the write end
:: :: 2 -- failed to cast execvp at the read end
----
-- Commands:
:: exit :: quit the program
:: cd :: C function to change directory chdir()
:: help :: displays the list of commands and functions
:: clear :: prints a string to clear the screen
:: dir :: print files and directories
:: print :: print a comment
