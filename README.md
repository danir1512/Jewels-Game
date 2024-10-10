# Jewels 

*Jewels* is a simple game in which the player needs to move jewels pieces to create a sequence of 3 jewels or more, horizontally or vertically. If there is a match, the jewels will dissapear and new ones will fall from the top of the board. 

## Dependencies

To compile the code, make sure that you have the SDL libraries install (refer tot he following links to install):

    * SDL: https://github.com/libsdl-org/SDL/releases
    * SDL_image: https://github.com/libsdl-org/SDL_image/releases
    * SDL_ttf: https://github.com/libsdl-org/SDL_ttf/releases

## How to build

The project can be compile using Cmake. To build, you can do the following:
```
cd ${project_folder}
mkdir build
cd build
cmake ..
make
```

This will build the game and crete an executable `game` on the `/build` folder you just created.

To run the game, please copy the executable to the assets folder and run:

```
cd build/
cp ./game ../assets/
cd ../assets/
./game
```

Otherwise, the game will not be able to find the assets to render the images.

Alternavely, I have also include the project as a Xcode project `InterviewExerciseDani.zip`

## App 

And app is also available, which does not requires any setup.

## Game Design Idea

The structure ofthe prject can be broken doen into three main classes: the Game itself, the GameBoard and the Jewels:

* `Game class`: This class has the main function of initializing all the variavels and objects necessary to run the game, as well as control the flow of game loop.

* `GameBoard class`: is responsible for handling the animation and logic behind the game. It is also repoisnble for rendering and updaitng the game windows.

* `Jewels class`: Serves as a representation of the Jewels on the game.

The game loop is as follow:

### Step 1

* Game starts and SDL libraries are initialized.
* Read file with scores.
* Initialize the board (load textures, create initial jewels).
* Lunch the game.

### Step 2

* Runs the game. The user will se the main screen, in which by pressing Enter, it will begin to play the game.
* If the user presses Esc or close the window, the game will end.
  
### Step 3

* After pressing Enter, the game will render the jewels, board, timer and score. 
* Start decreasing the timer.
* Wait for user input. If mouse input if found and in onthe area of the board, go to step 4. Else, update game.
* If timer ends, will signalise gameover. Then, go to step 5.

### Step 4 

* Control passes to the mousehandle function. Detects if the user is selecting a jewel. If the user drags the mouse and relase the button or click on anohter jewel, check is it is a valid swap. 
* If swap is valid, swap jewel and render screen. Then go back to step 3 and repeat.
* If swap is not valid, do nothing, go to step 3 and repeat.

### Step 5 

* Once the time is over, finish removing sequences and render the end screen, showing the current score and the highest score.
* If the current score suprasses the higest score, write to the file.
* User can either press Enter or Esc (or close the window, whihc will end the game.)
* If Enter, play a new game. Go to Step 3 and repeat.
* If ESC, Go back to Step 2 and repeat.

# Notes

The project can benefits from some improvements:

* Creates another class that represents the Engine or Renderer, which handles the rendering, as well as the loading of the Textures. This will make the GameBoard class less big.

* Rethink the way the animation are done, which it may not be optimal. 

* Improve the overall game experience: Adding sound and adding extra game features (e.g. bombs appear once the user have a sequence higher than 3, whihc gives extra points).

* Adding unittest is also very important!



