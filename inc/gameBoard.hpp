#ifndef GAMEBOARD_HPP_INCLUDED
#define GAMEBOARD_HPP_INCLUDED

#include "jewels.hpp"
#include "common.hpp"
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <memory>
#include <array>
#include <vector>

template <typename T, int Rows, int Cols>
using Board = std::array<std::array<T, Rows>, Cols>;

/// Class representing the game board.
/// 
/// This class manages the game board and provides various functions for manipulating and rendering the jewels on the board.
template <int Rows, int Cols>
class GameBoard {
public:
    
    /// Constructor
    GameBoard();
    
    /// Get the current game score
    ///
    /// @return game score
    inline int getScore() const;
    
    /// Reset the score of the game
    ///
    /// @brief Sets `m_score` to 0.
    inline void resetScore();
    
    /// Update the score based on the current state of the game board.
    ///
    /// @brief Update the score of the game based on how many sequences where found on the current move.
    void updateScore();

    /// Initialize the game.
    ///
    /// @brief The function is call to initilize the game assets and fill the board with jewels,
    ///        calling @ref loadTexture() and @ref setUpBoard() respectavley.
    void initializeGame();
    
    /// Set up the initial state of the game board.
    ///
    /// @brief Creates a board wit no possible moves for the beginning of the game. Fills `m_board` with Jewels.
    /// Then, checks for any sequences and it removes it. It repeats this process until the board has no sequences.
    void setUpBoard();

    /// Search for sequences of matching jewels on the game board.
    ///
    /// @return true if a sequence is found, false otherwise.
    bool searchForSequence();

    /// Move down the jewels on the game board by one row.
    ///
    /// @brief Starts searching for "empty" positions (an empty position is a Jewel mark as UNKNOWN) from the bottom of the board.
    ///        After finding one, move all jewels above on the colum by one position. If the Jewel from the top of the board are empty,
    ///        create a new random jewel.
    void moveDownJewelAnimation();

    /// Remove the jewels that are marked for destruction.
    ///
    void removeMarkedJewels() const ;

    /// Check if the game board is full.
    /// 
    /// @return true if the game board is full, false otherwise.
    bool isBoardFull() const;
    
    /// Update the the game states
    ///
    /// @brief It calls @ref renderJewels, which copy all the element to the renderer.
    ///        Then it proceeds to call @ref renderFrame, which update the window with the current renderer.
    void update();
    
    /// Render the contents of the renderer
    ///
    /// @brief Calls`SDL_RenderPresent()` to display changes on the window. Calls `SDL_Delay` to emulate FPS.
    inline void renderFrame() const;
    
    /// Render the jewels on the game board.
    ///
    /// @brief Renders all the element of the game.
    void renderJewels();

    /// Render the game board.
    ///
    /// @brief Renders the backgound image.
    inline void renderBackground() const;
    
    /// Render text.
    ///
    ///@brief Render the string given by @p text, and display into on the @p rect
    ///
    ///@param rect SDL_Rect destination to render the text.
    ///@param text string containg the text to render
    void renderText(SDL_Rect* rect, const std::string& text);
    
    /// Render highligh Jewel
    ///
    ///@brief This function renders a selection indicator at the specified destination
    ///       position based on given parameters such as origin position, selection state,
    ///       and drag state.
    ///
    ///@param destRow The row index of the destination position where the selection indicator will be rendered.
    ///@param destCol The column index of the destination position where the selection indicator will be rendered.
    ///@param orgRow The row index of the origin position from where the selection started.
    ///@param orgCol The column index of the origin position from where the selection started.
    ///@param selected A boolean flag indicating whether the selection is active (true) or not (false).
    ///@param drag A boolean flag indicating whether the selection is being dragged (true) or not (false).
    void renderSelect(const int& destRow, const int& destCol, 
                                        const int& orgRow, const int& orgCol,
                                        const bool selected, const bool drag);
    
    void swapAnimation(int originRow, int originCol, int destRow, int destCol);
private:
    
    ///Fill the board with Jewels
    inline void fillBoard();

    /// Generate a random type of jewel.
    /// 
    /// @return The type of jewel generated.
    auto generateRandomJewel() -> JewelsType const;

    /// Render the texture of the jewel at the specified position.
    /// 
    /// @param row The row index of the jewel.
    /// @param col The column index of the jewel.
    /// @param type The type of the jewel.
    void renderJewelTexture(const int row, const int col, JewelsType type) const;
    
    void renderTest(SDL_Rect * rect, JewelsType) const;

    /// Load the textures for the jewels.
    ///
    /// @brief Load all necessary texture and fonts.
    void loadTextures();
    
    /// Render board
    void renderBoard();

    /// Renders the remaing time
    ///
    /// @brief Calculates the reaming time and renders it.
    ///        If the time is over, it sets the flag gameover to true.
    void renderTime();
    
    /// Check if there is a sequence of matching jewels starting from the specified position.
    /// 
    /// @param row The row index of the starting position.
    /// @param col The column index of the starting position.
    /// @param dRow The row direction to check for sequence.
    /// @param dCol The column direction to check for sequence.
    /// @return true if a sequence is found, false otherwise.
    bool checkSequence(const int row, const int col, const int dRow, const int dCol);

    /// Renders the current score.
    void renderScore();

    
    // Timer
    Clock m_clock;
    const int m_time{120};

    // Renderer
    SDL_Renderer* m_renderer{nullptr};

    // Boards state
    Board<std::shared_ptr<Jewels>, Rows, Cols> m_board {nullptr};
    Board<bool, Rows, Cols> m_marked{false};
    Board<bool, Rows, Cols> m_toAnimate{false};

    int m_score{0};

    // Rects
    SDL_Rect m_boardRect { g_boardPosX , g_boardPosY , g_boardSquareSize, g_boardSquareSize };
    SDL_Rect m_windowRect {0, 0, g_windowWidth, g_windowHeight};
    SDL_Rect m_timeText {100, 150, 70, 70};
    SDL_Rect m_timeRect {190, 152, 70, 70};
    SDL_Rect m_scoreText {100, 250, 70, 70};
    SDL_Rect m_scoreRect {220, 252, 70, 70};
    SDL_Rect m_boardSquares {0, 0, g_boardSquareSize / Rows, g_boardSquareSize / Cols};

    // Textures
    SDL_Texture * m_background{nullptr};
    SDL_Texture * m_boardBackground{nullptr};
    SDL_Texture * m_blackJewel{nullptr};
    SDL_Texture * m_whiteJewel{nullptr};
    SDL_Texture * m_blueJewel{nullptr};
    SDL_Texture * m_redJewel{nullptr};
    SDL_Texture * m_pinkJewel{nullptr};

    // Fonts
    TTF_Font* m_font;
    SDL_Texture* m_text;
    
};


#include "gameBoard.inl"

#endif
