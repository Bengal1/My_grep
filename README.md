# My grep
This reposatory is part of a small project I did in my B.sc in Elecrical engineering at Tel-Aviv University. The project is part of advanced computer laboratory about C programming. 

Grep searches for PATTERNS in each FILE. PATTERNS is one or more patterns separated by newline characters, and grep prints each line that matches a pattern. Typically PATTERNS should be quoted when grep is used in a shell command. For more information on [*grep*](https://man7.org/linux/man-pages/man1/grep.1.html).

## My grep
This program is a vriation of the Linux utility grep. The program gets a an expression, file path and control keys (switches), it goes through the file, finds the wanted expression and prints out the line with the expression. The output and the expression can be modified according to the control keys. If there is no file path in the comman, the program will get input from stdin. 

### Control Keys (Switches)
`-A NUM` - The program will output the wanted line and NUM lines after it. meaning the program will print NUM + 1 continuous lines were the first line is the line of the expression.

`-b` - Before every line the number of bytes from the beginning until this line will be print out.

`-c` - Instead of printing out the lines, the program will print the line number.

`-i` - The program will not be case sensative when looking up the wanted expression.

`-n` - The program will print the line number before every printed line. If -A also implementedת there will be an hyphen between the line number and the line in all the line that printed after the wanted line. 

`-v` - The program will print the lines that don't contain the wanted expression.

`-x` - The program will print lines that contain only the expression and nothing else.
#### `-E` Control key:
This control key is written right before the expression and it indicates that [regular expressions](https://en.wikipedia.org/wiki/Regular_expression) are used at the input expression.
- The special keys **()[]{}|\\** will be escaped with '\\'.
- **(str1|str2)** - When using it the program will accept str1 or str2 in its respective place when lookin for the wanted expression.
- **[x-y]** - When using it the program will accept characters with ASCII values between x and y in its respective place when lookin for the wanted expression.
- **.** (dot) - When using dot (.) the program will accept every character in its respective place when lookin for the wanted expression.


### *Title!!!!:*

add text here!!!

`my_grep -n`

`my_grep -n`

`my_grep -n`

`my_grep -n`

`my_grep -n`

add printouts after every command!!!


