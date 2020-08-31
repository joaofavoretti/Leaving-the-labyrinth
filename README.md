# Recursive-C-Algorithm
C program that finds the way out of a labyrinth

Given a labyrinth at the format:

M N
X Y
###....#
#.#...##
#...#.##
#.#.####
#.#.....
########

M and N are the amount of lines and rows, X and Y are the (x, y) coordinates of the cursor.

At the end, the program gives you the path from the begginning to the end of the labyrinth, following (top, right, bottom, left) preference.

### Compile with: 
```sh
  gcc -c main.o main .c && gcc -o main main.o
 ```
