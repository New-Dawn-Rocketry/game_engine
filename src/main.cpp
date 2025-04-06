#include "SDL3/SDL.h"

SDL_Semaphore* frame_update_ready = nullptr;

Uint32 frame_update_trigger(void *userdata, SDL_TimerID timerID, Uint32 interval)
{
    // Timer has elapsed, time to render another frame
    SDL_SignalSemaphore(frame_update_ready);
    return interval;
} 

int renderer_update(SDL_Renderer*& renderer, SDL_FRect rectangle)
{
    bool e = false; 
    static SDL_Time prev_frame_update = 0;
    SDL_Time current_frame_update = 0; 
    e |= SDL_GetCurrentTime(&current_frame_update);
    int frame_dt_ns = current_frame_update - prev_frame_update;
    float frame_rate =  1.0 / SDL_NS_TO_SECONDS((float)frame_dt_ns);
    
    e |= SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    e |= SDL_RenderClear(renderer);
    e |= SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    e |= SDL_RenderFillRect(renderer, &rectangle);
    e |= SDL_RenderDebugTextFormat(renderer, 0.0, 0.0, "FPS: %0.0f", frame_rate);
    e |= SDL_RenderPresent(renderer);
    prev_frame_update = current_frame_update;
    return e;
}

void object_update(SDL_FRect* rect)
{
    rect->x++;
    rect->y++;
    return;
}

int main () 
{
    frame_update_ready = SDL_CreateSemaphore(1);
    SDL_Thread* renderer_thread = nullptr;
    SDL_Log("Hello World!\n");
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_CreateWindowAndRenderer("My Game", 800, 480, SDL_WINDOW_ALWAYS_ON_TOP, &window, &renderer);
    // SDL_CreateThread(renderer_update, "renderer_update", &renderer);
    Uint32 period_ms = SDL_roundf((1.0 / 60.0) * 1000.0);
    SDL_Log("Period in ms is %u\n", period_ms);
    SDL_AddTimer(period_ms, frame_update_trigger, nullptr);
    SDL_Event event;
    SDL_FRect rectangle = {200, 0, 200, 200};
    do
    {
        SDL_WaitSemaphore(frame_update_ready);
        object_update(&rectangle);
        renderer_update(renderer, rectangle);
        SDL_PollEvent(&event);
    }while(event.type != SDL_EVENT_QUIT);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    return 0;
}