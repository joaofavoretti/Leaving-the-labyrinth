# Leaving the Labyrinth
(University project for Introduction on Computer Science II)

Given a labyrinth at the format:
```sh
M N
X Y
###....#
#.#...##
#...#.##
#.#.####
#.#.....
########
```

The dots are the free way and the # are the walls.

M and N are the amount of lines and rows, X and Y are the (x, y) coordinates of the cursor.

At the end, the program gives you the path from the begginning to the end of the labyrinth, following (top, right, bottom, left) preference.

### Compile with: 
```sh
  gcc -c main.o main .c && gcc -o main main.o
 ```
