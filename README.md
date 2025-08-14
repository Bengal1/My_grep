# My grep
This reposatory is part of a small project I did in my B.sc in Elecrical Engineering. The project is part of advanced computer laboratory about C programming. 

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
- **[x-y]** - When using it the program will accept characters with [ASCII](https://en.wikipedia.org/wiki/ASCII) values between x and y in its respective place when lookin for the wanted expression.
- **.** (dot) - When using dot (.) the program will accept every character in its respective place when lookin for the wanted expression.


## *Demonstration:*
The nine lines in the qoute below are from the Wikipedia page - grep. We will use them to demonstrait the use of the program functionality. Let us refer to them as a file 'grep.wiki'.

"""

grep is a command-line utility for searching plain-text data sets for lines that match a regular expression.

Its name comes from the ed command grep (globally search for a regular expression and print matching lines), which has the same effect.

grep was originally developed for the Unix operating system, but later available for all Unix-like systems and some others such as OS-9.

Before it was named, grep was a private utility written by Ken Thompson to search files for certain patterns. 

Doug McIlroy, unaware of its existence, asked Thompson to write such a program. 

Responding that he would think about such a utility overnight, 

Thompson actually corrected bugs and made improvements for about an hour on his own program called s (short for "search"). 

The next day he presented the program to McIlroy, who said it was exactly what he wanted. 

Thompson's account may explain the belief that grep was written overnight.*

"""

The following part demonstrait valid command and there output. As you can see the name of the program is `my_grep` and the name of the "file" in the demonstraition is `grep.wiki`.

```terminal
my_grep -n OS grep.wiki
```

```
3:  grep was originally developed for the Unix operating system, but later available for all Unix-like systems and some others such as OS-9.
```
* `my_grep -b OS grep.wiki`
```
244:  grep was originally developed for the Unix operating system, but later available for all Unix-like systems and some others such as OS-9.
```
* `my_grep -n -A 2 OS grep.wiki`

```
3:  grep was originally developed for the Unix operating system, but later available for all Unix-like systems and some others such as OS-9.
4-  Before it was named, grep was a private utility written by Ken Thompson to search files for certain patterns. 
5-  Doug McIlroy, unaware of its existence, asked Thompson to write such a program. 
```

* `my_grep -i -E ("w"|"h")as grep.wiki`
```
Its name comes from the ed command grep (globally search for a regular expression and print matching lines), which has the same effect. 

grep was originally developed for the Unix operating system, but later available for all Unix-like systems and some others such as OS-9.

Before it was named, grep was a private utility written by Ken Thompson to search files for certain patterns.

The next day he presented the program to McIlroy, who said it was exactly what he wanted. 

Thompson's account may explain the belief that grep was written overnight.
```

* `my_grep -n -E it[t-z] grep.wiki`
```
1:  grep is a command-line utility for searching plain-text data sets for lines that match a regular expression.
4:  Before it was named, grep was a private utility written by Ken Thompson to search files for certain patterns.
6:  Responding that he would think about such a utility overnight,
9:  Thompson's account may explain the belief that grep was written overnight.
```
* `my_grep -c -E it[t-z] grep.wiki`
```
1
4
6
9
```
* `my_grep -n -i -E be. grep.wiki`
```
3:  grep was originally developed for the Unix operating system, but later available for all Unix-like systems and some others such as OS-9.
7:  Thompson actually corrected bugs and made improvements for about an hour on his own program called s (short for "search").
```
* `my_grep -n -v -E it. grep.wiki`
```
4:  Before it was named, grep was a private utility written by Ken Thompson to search files for certain patterns. 
9:  Thompson's account may explain the belief that grep was written overnight.
```
