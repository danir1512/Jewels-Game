#ifndef COMMON_H
#define COMMON_H
#include <SDL2/SDL.h>

constexpr int g_windowWidth{800};

constexpr int g_windowHeight{500};

constexpr int g_boardSquareSize{320};

constexpr int g_boardPosX{420};

constexpr int g_boardPosY{100};

constexpr int g_boardSize{8};

constexpr int g_jewelSquareSize{38};

constexpr int g_jewlesDistance{40};

constexpr int g_FPS{60};

extern bool g_gameOver;

extern SDL_Window* g_window;

extern SDL_Renderer* g_renderer;

#endif
