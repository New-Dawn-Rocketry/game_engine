// #include "renderer.hpp"
// #include "game.hpp"
// #include <queue>

#include "world.hpp"
#include  "math.h"

// using std::queue;

// SDL_Semaphore* frame_update_ready = nullptr;
// renderer renderer_c;

// Uint32 frame_update_trigger(void *userdata, SDL_TimerID timerID, Uint32 interval)
// {
//     // Timer has elapsed, time to render another frame
//     SDL_SignalSemaphore(frame_update_ready);
//     return interval;
// }

struct transform
{
    int x;
    int y;
};

struct motion
{
    int v_x;
    int v_y;
    int a_x;
    int a_y;
};


class motion_system : public system_base
{

};

int main () 
{
    world my_world = world();
    my_world.register_component<transform>();
    my_world.register_component<motion>();

    entity player = my_world.create_entity();


    // frame_update_ready = SDL_CreateSemaphore(1);

    // SDL_Window* window;
    // SDL_Renderer* render;
    // SDL_CreateWindowAndRenderer("Lost ", 800, 480, SDL_WINDOW_ALWAYS_ON_TOP, &window, &render);
    // renderer_c = render;

    // game.init();

    // Uint32 period_ms = SDL_roundf((1.0 / 60.0) * 1000.0);
    // // SDL_Log("Period in ms is %u\n", period_ms);

    // SDL_AddTimer(period_ms, frame_update_trigger, nullptr);
    
    // SDL_Event event;
    
    // do
    // {
    //     SDL_WaitSemaphore(frame_update_ready);
    //     for (auto& itr : game.get_objects())
    //     {
    //         itr->update();
    //     }
    //     renderer_c.render_next_frame();
    //     SDL_PollEvent(&event);
    // }while(event.type != SDL_EVENT_QUIT);
    // SDL_DestroyWindow(window);
    // SDL_DestroyRenderer(render);
    return 0;
}