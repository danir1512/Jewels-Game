#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include <string.h>
#include "clock.hpp"
#include "gameBoard.hpp"

class Game {
public:
    
    /// Constructor
    Game();

    /// Exist the game
    ///
    /// @brief Calls SDL lib `SDL_Quit` (as well as its respectives function for `SDL_Image` and `SDL_TTF`) to clean up all initialized subsystems.
    ///        It also closes the file containg the high score.
    void exit();

    /// Initialize the SDL libraries
    ///
    /// @brief Initialize SDL libraies and also create the window and render obejcts to be used in the prorgam.
    void initSDL();
    
    /// Read fiel containg high Score
    ///
    /// @brief Reads file containing the highest score attained by the player.
    //void readScore();

    /// Launches  the game
    ///
    /// @brief This function launches the game and display the main menu (title screen). If the user press the Enter Keybord, the games will start.
    void runGame();
    
    /// Starts the game loop
    ///
    /// @brief The game loop consiste of detecting the playes input. If the user click on 1 of the jewels, the input is conider as valid an the control is passed to the function `mouseInputHandle()` which handles the input.
    ///        Additonaly, state flags are place to detect if the user is cliking into jewels to chnage their position, or draging then to another possition.
    ///        A timer is set to render the screen while animation operation are in place, namely to continuely render the time information to the player.
    void gameStart();

    /// End game handle
    ///
    /// @brief Hanldes the end of the game. Renders the end game screen, showing he current score of the player and the highest score the player has obtained.
    void endGame();

    /// Renders the end game screen
    void renderEndGame();

    /// Renders the title screen
    void renderStartingScreen();
    
    /// Handles the mouse input
    ///
    /// @brief Handle the mouse input of the user. This handles check if the user has either pressed the mouse down, lift the button or mouve the mouse.
    void mouseInputHandle();

    /// Checks if the given coordinates are within the bounds of the game board.
    ///
    /// @param x The x-coordinate.
    /// @param y The y-coordinate.
    /// @return true if the coordinates are within bounds, false otherwise.
    bool IsWithinBounds(int x, int y);

    /// Checks if we can swap the jewels
    ///
    /// @brief It swaps the jewels from board if the destination coordinates are valid return true, Othersie, false
    inline bool isMoveValid();

    /// Get the coordinates of the jewels at the specified position.
    ///
    /// @param jewel_x Reference to the variable to store the x-coordinate of the jewel.
    /// @param jewel_y Reference to the variable to store the y-coordinate of the jewel.
    void getJewelsPos(int& jewel_x, int& jewel_y);

    /// Make the move of swapping Jewels
    void makeMove();
    
    std::string getHighScore();
    
private:

    //SDL variavels
    SDL_Event m_event;
    SDL_Point m_mousePos;
    SDL_TimerID m_timerId;

    // Rect
    SDL_Rect m_highScoreRect {200, 220, 400, 70};
    SDL_Rect m_currentScoreRect {200, 310, 400, 70};
    SDL_Rect m_promt {130, 400, 600, 70};
    SDL_Rect m_titleRect {g_windowWidth / 4, g_windowHeight / 4, 400, 100};

    /// Board
    GameBoard<g_boardSize,g_boardSize> m_board{};

    static Uint32 callback(Uint32 interval, void *param);

    // Flow control variavels
    bool m_selected{false};
    bool m_isMouseMoving{false};
    bool m_click{false};
    int m_destCol{0};
    int m_destRow{0};
    int m_originCol{0};
    int m_originRow{0};

    // Score info
    //std::fstream m_scoreRecord;
    int m_highScore{0};

};

#endif


