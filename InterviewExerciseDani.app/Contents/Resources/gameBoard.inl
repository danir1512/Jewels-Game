template <int Rows, int Cols>
GameBoard<Rows,Cols>::GameBoard()
{
   static_assert(Rows == Cols, "The Colummns and Rows size of the board need to be the same!");
}


template <int Rows, int Cols>
void GameBoard<Rows,Cols>::initializeGame()
{
    // Load textures
    loadTextures();

    // Set up the game board
    setUpBoard();
}

template <int Rows, int Cols>
inline int GameBoard<Rows,Cols>::getScore() const { 
    return m_score;
}

template <int Rows, int Cols>
inline void GameBoard<Rows,Cols>::fillBoard(){
    for (int col{0} ; col < Cols; col++) {
        for (int row{0}; row < Rows; row++) {
            if (!m_board[row][col]->getValid()) {
                m_board[row][col] = std::make_unique<Jewels>(generateRandomJewel());
            }
        }
    }
}


template <int Rows, int Cols>
void GameBoard<Rows,Cols>::setUpBoard() {
    for (int row = 0; row < Rows; row++) {
        for (int col = 0; col < Cols; col++) {
            m_board[row][col] = std::make_unique<Jewels>(generateRandomJewel());
        }
    }
    
    while(searchForSequence()) {
        removeMarkedJewels();
        fillBoard();
    }

} 

template <int Rows, int Cols>
void GameBoard<Rows,Cols>::update() {

    updateScore();
    removeMarkedJewels();
    renderJewels();
    renderFrame();
    
    // Refill
    while (!isBoardFull()) {
        moveDownJewelAnimation();
    }
}

template <int Rows, int Cols>
bool GameBoard<Rows,Cols>::isBoardFull() const {
   for(const auto& row : m_board) {
       for(const auto& col : row) {
           if(col->getValid() == false){
               return false;
           }
       }
   }
   return true;
}

template <int Rows, int Cols>
void GameBoard<Rows,Cols>::removeMarkedJewels() const {
    for (int row = 0; row < Rows; row++) {
        for (int col = 0; col < Cols; col++) {
            if (m_marked[row][col]) {
                m_board[row][col]->destroy();
            }
        }
    }
}

template <int Rows, int Cols>
bool GameBoard<Rows,Cols>::searchForSequence() {
    m_marked = Board<bool,Rows,Cols>{false};
    bool hasSequence{false};
    for (int row = 0; row < Rows; row++) {
        for (int col = 0; col < Cols; col++) {
            if (checkSequence(row, col, 1, 0) || checkSequence(row, col, 0, 1)) {
                hasSequence = true;
            }
            // checkSequence(i, j, 1, 0);  // Check horizontally
            // checkSequence(i, j, 0, 1);  // Check vertically
        }
    }
    return hasSequence;
}

template <int Rows, int Cols>
bool GameBoard<Rows,Cols>::checkSequence(const int row, const int col, const int dRow, const int dCol) {
    if(m_board[row][col]->getJewelType() != JewelsType::UNKOWN) { 
        int seqLength = 1;
        while (row + seqLength*dRow < Rows && 
                col + seqLength*dCol < Cols && 
                m_board[row][col]->getJewelType() == 
                m_board[row + seqLength*dRow][col + seqLength*dCol]->getJewelType()) {
            seqLength++;
        }
        if (seqLength >= 3) {
            for (int k = 0; k < seqLength; k++) {
                m_marked[row + k*dRow][col + k*dCol] = true;
            }
            return true;
        }
    }
    return false;
}

template <int Rows, int Cols>
void GameBoard<Rows,Cols>::updateScore() {
    for (const auto& row : m_marked) {
        for (const auto& jewel : row) {
            if(jewel == true) {
                m_score += 1;
            }
        }
    }
}

template <int Rows, int Cols>
inline void GameBoard<Rows,Cols>::resetScore() {
    m_score = 0;
}

template <int Rows, int Cols>
auto GameBoard<Rows,Cols>::generateRandomJewel() -> JewelsType const{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::vector<JewelsType> ALL_COLOURS({JewelsType::BLACK, JewelsType::WHITE, JewelsType::BLUE, JewelsType::RED, JewelsType::PINK});
    std::uniform_real_distribution<> dis(0, ALL_COLOURS.size() - 1);
    JewelsType randomJewel = ALL_COLOURS[dis(gen)];
    return randomJewel;
}

template <int Rows, int Cols>
void GameBoard<Rows,Cols>::loadTextures()
{       
    m_blackJewel = IMG_LoadTexture(g_renderer, "black.png");
    if (m_blackJewel == nullptr) {
        std::cerr << "Error loading black jewel texture" << std::endl;
    }

    m_whiteJewel = IMG_LoadTexture(g_renderer, "white.png");
    if (m_whiteJewel == nullptr) {
        std::cerr << "Error loading white jewel texture" << std::endl;
    }

    m_blueJewel = IMG_LoadTexture(g_renderer, "blue.png");
    if (m_blueJewel == nullptr) {
        std::cerr << "Error loading blue jewel texture" << std::endl;
    }

    m_redJewel = IMG_LoadTexture(g_renderer, "red.png");
    if (m_redJewel == nullptr) {
        std::cerr << "Error loading red jewel texture" << std::endl;
    }

    m_pinkJewel = IMG_LoadTexture(g_renderer, "pink.png");
    if (m_pinkJewel == nullptr) {
        std::cerr << "Error loading pink jewel texture" << std::endl;
    }

    m_background = IMG_LoadTexture(g_renderer, "background.jpg");
    if (m_background == nullptr) {
        std::cerr << "Error loading background texture" << std::endl;
    }
    
    m_background = IMG_LoadTexture(g_renderer, "background.jpg");
    if (m_background == nullptr) {
        std::cerr << "Error loading background texture" << std::endl;
    }

    m_font = TTF_OpenFont("Roboto-Black.ttf", 100);
    if (m_font == nullptr) {
        std::cerr << "Error loading fonts" << std::endl;
    }
}

/** -------- Animation ---------*/
template <int Rows, int Cols>
void GameBoard<Rows,Cols>::moveDownJewelAnimation() {
    int posCount{0};
    
    constexpr int posOffset{2};
    constexpr int boardOffSetX{g_boardPosX + posOffset};
    constexpr int boardOffSetY{g_boardPosY + posOffset};
    
    std::vector<SDL_Rect> rectToRender;
    std::vector<JewelsType> jewelsType;
    SDL_Rect jewelPos{0,0,g_jewelSquareSize,g_jewelSquareSize};
    
    for (int row = Rows - 1; row >= 0; row--) {
        for (int col{0} ; col < Cols; col++) {
            if (!m_board[row][col]->getValid()) {
                if(row > 0) {
                    if (!m_board[row-1][col]->getValid()) {
                        continue;
                    }
                    for (int k = row - 1 ; k >= 0; k--) {
                        m_toAnimate[k][col] = true;
                        // Is the same
                        jewelPos.x = (col * g_jewlesDistance) + boardOffSetX;
                        
                        jewelPos.y = (k * g_jewlesDistance) + boardOffSetY;
                        
                        rectToRender.push_back(jewelPos);
                        jewelsType.push_back(m_board[k][col]->getJewelType());
                    }
                    
                    // Don't render the jewel that is being swap
                    m_toAnimate[row][col] = true;
                    std::swap(m_board[row][col], m_board[row-1][col]);
                } else {
                    m_board[0][col] = std::make_unique<Jewels>(generateRandomJewel());
                    
                    m_toAnimate[0][col] = true;
                    // Is the same
                    jewelPos.x = (col * g_jewlesDistance) + boardOffSetX ;
                    // Move down
                    jewelPos.y = boardOffSetY  - g_jewelSquareSize;
                    
                    rectToRender.push_back(jewelPos);
                    jewelsType.push_back(m_board[0][col]->getJewelType());
    
                    continue;
                }
            }
        }
    }
    
    while (posCount != g_jewelSquareSize)
    {
        renderJewels();
        
        for (int i{0}; i < rectToRender.size(); i++) {
            renderTest(&rectToRender[i], jewelsType[i]);
            rectToRender[i].y += 2;
        }
        
        renderFrame();
        posCount+= 2;
    }
    
    // Reset control variavels
    jewelsType.clear();
    rectToRender.clear();
    m_toAnimate = Board<bool,Rows,Cols>{false};
}

template <int Rows, int Cols>
void GameBoard<Rows,Cols>::swapAnimation(const int originRow, const int originCol, const int destRow, const int destCol) {
    bool animationFinished{false};
    const float frame{2.0F};
    constexpr int posOffset{2};
    constexpr int boardOffSetX{g_boardPosX + posOffset};
    constexpr int boardOffSetY{g_boardPosY + posOffset};
    
    SDL_Rect jewelPosOrigin{
        (originCol * g_jewlesDistance) + boardOffSetX,
        (originRow * g_jewlesDistance) + boardOffSetY,
        g_jewelSquareSize,
        g_jewelSquareSize};
    SDL_Rect jewelPosDest{
        (destCol * g_jewlesDistance) + boardOffSetX,
        (destRow * g_jewlesDistance) + boardOffSetY,
        g_jewelSquareSize,
        g_jewelSquareSize};
    
    m_toAnimate[originRow][originCol] = true;
    m_toAnimate[destRow][destCol] = true;
    
    while(!animationFinished) {
        if ( originRow > destRow) {
            jewelPosOrigin.y -= frame ;
            jewelPosDest.y += frame ;
        } else if (originRow < destRow){
            jewelPosOrigin.y += frame ;
            jewelPosDest.y -= frame ;
        }
        
        if ( originCol > destCol) {
            jewelPosOrigin.x -= frame ;
            jewelPosDest.x += frame ;
        } else if ( originCol < destCol){
            jewelPosOrigin.x += frame ;
            jewelPosDest.x -= frame ;
        }
        
        // Check if the piece is already at the destination
        if (jewelPosOrigin.x == (destCol * g_jewlesDistance) + boardOffSetX && jewelPosOrigin.y == (destRow * g_jewlesDistance) + boardOffSetY) {
            animationFinished = true;
        }

        renderJewels();
        
        renderTest(&jewelPosOrigin, m_board[originRow][originCol]->getJewelType());
        renderTest(&jewelPosDest, m_board[destRow][destCol]->getJewelType());
        
        renderFrame();
    }
    std::swap(m_board[destRow][destCol], m_board[originRow][originCol]);
    m_toAnimate = Board<bool,Rows,Cols>{false};
}

/** ------------------------------  Render Functions ---------------------------------**/
template <int Rows, int Cols>
void GameBoard<Rows,Cols>::renderJewelTexture(const int row, const int col, JewelsType type) const {
    constexpr int posOffset{2};
    const SDL_Rect jewelPos{(col * g_jewlesDistance) +  (g_boardPosX + posOffset),
                            (row * g_jewlesDistance) + (g_boardPosY + posOffset),
                            g_jewelSquareSize,
                            g_jewelSquareSize};
    
    switch (type) {
        case JewelsType::BLACK:
            SDL_RenderCopy(g_renderer, m_blackJewel, NULL, &jewelPos);
            break;
        case JewelsType::WHITE:
            SDL_RenderCopy(g_renderer, m_whiteJewel, NULL, &jewelPos);
            break;
        case JewelsType::BLUE:
            SDL_RenderCopy(g_renderer, m_blueJewel, NULL, &jewelPos);
            break;
        case JewelsType::RED:
            SDL_RenderCopy(g_renderer, m_redJewel, NULL, &jewelPos);
            break;
        case JewelsType::PINK:
            SDL_RenderCopy(g_renderer, m_pinkJewel, NULL, &jewelPos);
            break;
        default:
            SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 0);  // Black
            SDL_RenderFillRect(g_renderer, &jewelPos);
            SDL_RenderCopy(g_renderer,NULL, NULL, &jewelPos);
            break;
    }
}

template <int Rows, int Cols>
void GameBoard<Rows,Cols>::renderTest(SDL_Rect * rect, JewelsType type) const {
    switch (type) {
        case JewelsType::BLACK:
            SDL_RenderCopy(g_renderer, m_blackJewel, NULL, rect);
            break;
        case JewelsType::WHITE:
            SDL_RenderCopy(g_renderer, m_whiteJewel, NULL, rect);
            break;
        case JewelsType::BLUE:
            SDL_RenderCopy(g_renderer, m_blueJewel, NULL, rect);
            break;
        case JewelsType::RED:
            SDL_RenderCopy(g_renderer, m_redJewel, NULL, rect);
            break;
        case JewelsType::PINK:
            SDL_RenderCopy(g_renderer, m_pinkJewel, NULL, rect);
            break;
        default:
            // Do nothing if jewel is unknown
            break;
    }
}

template <int Rows, int Cols>
void GameBoard<Rows,Cols>::renderJewels() {
    renderBackground();
    renderTime();
    renderScore();
    renderBoard();

    for (int row = 0; row < Rows; row++) {
        for (int col = 0; col < Cols; col++) {
            if(!m_toAnimate[row][col]){
                renderJewelTexture(row, col, m_board[row][col]->getJewelType());
            }
        }
    }
}


template <int Rows, int Cols>
void GameBoard<Rows,Cols>::renderTime() {
    if(g_gameOver) {
        m_clock.stop();
    } else if (!m_clock.ticktock(m_time * 1000)) {
        g_gameOver = true;
    }

    std::string minutes = std::to_string(m_clock.m_currentTime / 60);
    std::string seconds = std::to_string(m_clock.m_currentTime % 60);

    if(std::stoi(minutes) < 10) {
        minutes = "0" + minutes;
    }
    
    if(std::stoi(seconds) < 10) {
        seconds = "0" + seconds;
    }
    
    renderText(&m_timeText, "Time");

    const std::string stringTime = minutes + ":" + seconds;
    renderText(&m_timeRect, stringTime);
}

template <int Rows, int Cols>
void GameBoard<Rows,Cols>::renderScore() {
    renderText(&m_scoreText, "Score");
    renderText(&m_scoreRect, std::to_string(m_score));
}


template <int Rows, int Cols>
void GameBoard<Rows,Cols>::renderText(SDL_Rect* rect, const std::string& text) {
    const SDL_Color textColor = {0, 0, 0};
    SDL_Surface* textSurface = TTF_RenderText_Blended(m_font, text.c_str(), textColor);
    m_text = SDL_CreateTextureFromSurface(g_renderer, textSurface);
    SDL_RenderCopy(g_renderer, m_text, NULL, rect);
    SDL_FreeSurface(textSurface);
}


template <int Rows, int Cols>
inline void  GameBoard<Rows,Cols>::renderFrame() const {
    SDL_RenderPresent(g_renderer);
    SDL_Delay(1000 / g_FPS);
}


template <int Rows, int Cols>
void GameBoard<Rows,Cols>::renderSelect(const int& orgRow, const int& orgCol,
                                        const int& destRow, const int& destCol,
                                        const bool selected, const bool drag)
{
    renderJewels();
    if (selected) {
        m_boardSquares.x = g_boardPosX + orgCol * m_boardSquares.w;
        m_boardSquares.y = g_boardPosY + orgRow * m_boardSquares.h;
        SDL_SetRenderDrawColor(g_renderer, 226, 255, 191, 100);
        SDL_RenderFillRect(g_renderer, &m_boardSquares);
    }
    if (drag) {
        m_boardSquares.x = g_boardPosX + destCol * m_boardSquares.w;
        m_boardSquares.y = g_boardPosY + destRow * m_boardSquares.h;
        SDL_SetRenderDrawColor(g_renderer, 191, 234, 255, 100);
        SDL_RenderFillRect(g_renderer, &m_boardSquares);
    }
    renderFrame();
}

template <int Rows, int Cols>
void GameBoard<Rows,Cols>::renderBoard() {
    for (int i = 0; i < g_boardSize; i++) {
        for (int j = 0; j < g_boardSize; j++) {
            m_boardSquares.x = g_boardPosX + j * m_boardSquares.w;
            m_boardSquares.y = g_boardPosY + i * m_boardSquares.h;

            if ((i + j) % 2 == 0) {
                SDL_SetRenderDrawColor(g_renderer, 229, 255, 204, 150);
            } else {
                SDL_SetRenderDrawColor(g_renderer, 229, 204, 255, 150);
            }

            SDL_RenderFillRect(g_renderer, &m_boardSquares);
        }
    }
}

template <int Rows, int Cols>
inline void GameBoard<Rows,Cols>::renderBackground() const {
    SDL_RenderCopy(g_renderer, m_background, NULL, &m_windowRect);
}
