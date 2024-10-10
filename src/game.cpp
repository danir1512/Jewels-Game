#include "game.hpp"

Game::Game() {

    // Initialize SDL.
    initSDL();
    
    // Read the highscore saved.
    //readScore();
    
    // Initialize the gameboard
    m_board.initializeGame();

    runGame();
}

void Game::initSDL() {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }
   
    if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
    }

    if(TTF_Init() != 0) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }

    g_window = SDL_CreateWindow("Jewels", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, g_windowWidth, g_windowHeight, 0);
    if (g_window == NULL) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }

    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (g_renderer == NULL) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }
    SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
}

void Game::exit() {
    //m_scoreRecord.close();
    SDL_DestroyWindow(g_window);
    SDL_DestroyRenderer(g_renderer); 
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}

void Game::runGame() {
    renderStartingScreen();
    while(SDL_WaitEvent(&m_event)) {
        if(m_event.type == SDL_KEYDOWN) {
            if(m_event.key.keysym.sym == SDLK_ESCAPE) {
                exit();
                break;
            }
            else if(m_event.key.keysym.sym == SDLK_RETURN) {
                g_gameOver = false;
                m_timerId = SDL_AddTimer(1000, callback, NULL);
                m_board.resetScore();
                gameStart();
            } 
        } else if (m_event.type == SDL_QUIT) {
            exit();
            break;
        }
    }
}

void Game::gameStart() {
    while(SDL_WaitEvent(&m_event) && !g_gameOver) {
        if(m_event.type == SDL_QUIT) {
            SDL_RemoveTimer(m_timerId);
            exit();
            return;
        } else if (m_event.type == SDL_MOUSEBUTTONDOWN ||
                    m_event.type == SDL_MOUSEMOTION ||
                    m_event.type == SDL_MOUSEBUTTONUP ){
                if (IsWithinBounds(m_event.button.x, m_event.button.y)) {
                    
                    if(!m_selected) {
                        getJewelsPos(m_originRow, m_originCol);
                    } else {
                        getJewelsPos(m_destRow,m_destCol);
                    }
                    
                    mouseInputHandle();
                    
                    m_board.renderSelect(m_originRow, m_originCol,m_destRow,m_destCol,m_selected, m_isMouseMoving);
                    
                    while(m_board.searchForSequence() && !m_selected) {
                        m_board.update();
                    }
                }
        } else if (m_event.type != SDL_KEYDOWN || m_event.type != SDL_KEYUP){
                    m_board.renderSelect(m_originRow, m_originCol,m_destRow,m_destCol,m_selected, m_isMouseMoving);
            }
    }
    endGame();
    SDL_RemoveTimer(m_timerId);
}

void Game::mouseInputHandle() {
    switch(m_event.type) {
        case SDL_MOUSEBUTTONDOWN:
            if (!m_selected) {
                m_click = true;
                m_selected = true;
            } else if (!m_isMouseMoving && m_selected) {
                if(isMoveValid()) {
                    makeMove();
                }
                m_selected = false;
            }
            break;

        case SDL_MOUSEMOTION:
            if(m_click && isMoveValid()) {
                m_isMouseMoving = true;
            } else {
                m_isMouseMoving = false;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (m_isMouseMoving && m_selected) {
                if(isMoveValid()) {
                    makeMove();
                }
                m_selected = false;
            }
            m_isMouseMoving = false;
            m_click = false;
            break;
    }
}

void Game::endGame(){
    renderEndGame();
    while(SDL_WaitEvent(&m_event)){
        if(m_event.type == SDL_KEYDOWN) {
            if(m_event.key.keysym.sym == SDLK_ESCAPE) {
                runGame();
                break;
            }
            else if(m_event.key.keysym.sym == SDLK_RETURN) {
                m_board.setUpBoard();
                m_board.resetScore();
                m_timerId = SDL_AddTimer(1000, callback, NULL);
                g_gameOver = false;
                gameStart();
                break;
            }
        } else if (m_event.type == SDL_QUIT) {
                exit();
                break;
            }
    }
}

void Game::renderStartingScreen(){
    // Render background
    m_board.renderBackground();
    m_board.renderText(&m_titleRect, "Jewels");
    SDL_SetRenderDrawColor(g_renderer,  255, 255, 255, 0);  // Black
    SDL_RenderFillRect(g_renderer, &m_currentScoreRect);
    m_board.renderText(&m_currentScoreRect, "Press Enter to start a new game.");

    m_board.renderFrame();
}

std::string Game::getHighScore() {
    // Read from file
    //m_scoreRecord.seekg(0);
    //std::string stringHighScore;
    //m_scoreRecord >> stringHighScore;
    
    //int highScore = std::stoi(stringHighScore);
    const int currentScore = m_board.getScore();
    
    if (currentScore > m_highScore) {
        //m_scoreRecord.seekp(0);
        //m_scoreRecord << currentScore;
        m_highScore = currentScore;
    }
    
    return std::to_string(m_highScore);
}

void Game::renderEndGame(){
    // Render background
    m_board.renderBackground();
    
    m_board.renderText(&m_titleRect, "GAME OVER");
    
    const std::string stringHighScore = "High Score: " + getHighScore();
    m_board.renderText(&m_highScoreRect, stringHighScore);
    
    const std::string score = "Score: " + std::to_string(m_board.getScore());
    m_board.renderText(&m_currentScoreRect, score);
    
    m_board.renderText(&m_promt, "Pres Enter to play a new game or Esc to go back to the menu.");
    
    m_board.renderFrame();
}

bool Game::IsWithinBounds(const int x, const int y) {
    // Too far left
    if (x < g_boardPosX) return false;

    // Too far right
    if (x > g_boardPosX + g_boardSquareSize) return false;

    // Too high
    if (y < g_boardPosY) return false;

    // Too low
    if (y > g_boardPosY + g_boardSquareSize) return false;

    // Inside rectangle
    return true;
}

inline bool Game::isMoveValid() {
    if ((m_destCol == m_originCol + 1 && m_destRow == m_originRow) ||
        (m_destCol == m_originCol && m_destRow == m_originRow + 1) ||
        (m_destCol == m_originCol - 1 && m_destRow == m_originRow) ||
        (m_destCol == m_originCol  && m_destRow == m_originRow - 1) ) {
        return true;
    }
    return false;
}

void Game::getJewelsPos(int& jewel_row, int& jewel_col) {
    SDL_GetMouseState(&m_mousePos.x, &m_mousePos.y);
    jewel_row = (m_mousePos.y - g_boardPosY) / g_jewelSquareSize;
    jewel_col = (m_mousePos.x - g_boardPosX) / g_jewelSquareSize;
}

void Game::makeMove() {
    m_board.swapAnimation(m_originRow, m_originCol, m_destRow, m_destCol);
    if(!m_board.searchForSequence()) {
        m_board.swapAnimation(m_originRow, m_originCol, m_destRow, m_destCol);
    }
}

//void Game::readScore() {
//    m_scoreRecord.open("scores.txt", std::ios::in | std::ios::out);
//    if (!m_scoreRecord) {
//        std::cerr << "Error loading scores files" << std::endl;
//    }
//}

Uint32 Game::callback(Uint32 interval, void *param)
{
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = 0;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return(interval);
}
