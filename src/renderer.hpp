#pragma once
#include "SDL3/SDL.h"

class renderer
{
public:
    // renderer();
    // ~renderer();
    
    renderer& operator=(const SDL_Renderer* other)
    {
        _renderer = (SDL_Renderer*)other;
        return *this;
    }

    void render_next_frame();
private:
    SDL_Renderer* _renderer = nullptr;
}inline renderer;