#include "renderer.hpp"

void renderer::render_next_frame()
{
    static SDL_FRect rect = {200, 0, 200, 200};
    static SDL_Time prev_frame_update = 0;
    SDL_Time current_frame_update = 0; 
    SDL_GetCurrentTime(&current_frame_update);
    int frame_dt_ns = current_frame_update - prev_frame_update;
    float frame_rate =  1.0 / SDL_NS_TO_SECONDS((float)frame_dt_ns);
    
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);

    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
    SDL_RenderDebugTextFormat(_renderer, 0.0, 0.0, "FPS: %0.0f", frame_rate);

    rect.x++;
    rect.y++;

    SDL_RenderFillRect(_renderer, &rect);

    SDL_RenderPresent(_renderer);
    prev_frame_update = current_frame_update;
    return;
}