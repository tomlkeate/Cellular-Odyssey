# Cellular-Odyssey
## A conways game of life simulator written in C using curses library. Using the curses library it is a Terminal based version of the game with many features. Including color support, reading and writing games to files, a lexicon and custom placement of cells. 

![Picture of conways game of life.](/assets/image.png).

## Making
### I have only ever compiled this on linux but it might work in WSL on windows.
### To get started 
### just clone repository and run 
```bash
./cellular-odyssey
```
or
```
make run
```

## Help
### Playing
* Press p to switch between playing mode and edit mode
* Press q or esc to exit.
* Ability to be resized.

### Movement
* k or "up-arrow" to move up.
* l or "right-arrow" to move right.
* j or "down-arrow" to move down.
* h or "left-arrow" to move left.

### Editing Board
* n, then type how many, to place n number of cells randomly.
* i to place down cell under cursor.
* x to remove cell under cursor.

### Writing and Reading from File
* w to write to file.
  * Type file name
  * Type title
  * Will overwrite any file of same name
* r to read from file.
  * Type file name to read from (without file extension)

### Using Lexicon
1. Go to life_lexicon/lexicon.txt
2. Copy design you want.
3. Create a file in the folder with cellular-odyssey called {file name}.conways
4. Run cellular-odyssey
5. Press r to read file and type in {file name}

## Configuration Options
### Editing config.h
* Change background color (assuming color support). 
* Change title color.
* Change field background color.
* Change field color.
* Change cell look.
* Change exit message.
* Change game update speed.
* Change bell sound.
* **Remember to remake**

## Contribution
### I built this project for fun allows contribution with MIT license.
