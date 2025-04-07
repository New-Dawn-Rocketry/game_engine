#pragma once
#include "SDL3/SDL.h"
#include <vector>

using std::vector;

// inline class game game;

class object_base
{
public:
    object_base() : rect(SDL_FRect()) {}
    virtual void init(int w, int h, int x, int y) = 0;
    virtual void update() = 0;
    virtual const SDL_FRect& get_rect() { return rect; }
protected:
    SDL_FRect rect;
    // SDL_Texture texture;
    bool invalidated = false;
};

class tile : public object_base
{
public:
    tile(SDL_Color color) : object_base(), color(color) {} 
    void init(int w, int h, int x, int y) override { rect.w = w; rect.h = h; rect.x = x; rect.y = y; }
    void update() override {rect.x++; rect.y++; }
    const SDL_Color& get_color() { return color; }
    const SDL_FRect& get_rect() { return rect; }
protected:
SDL_Color color;
};

/// This encapsulates the functionality that determines which parts of the level and which objects are in view
/// Anything else will only be updated in the back ground and not sent to the renderer
class camera;

class game
{
public:
    // game();
    void init() 
    {
        SDL_Color color = { 255, 255, 255, 255 }; 
        object_base* rect = new tile(color); 
        objects.push_back(rect);
    }
    const vector<object_base*>& get_objects() { return objects; }
private:
    vector<object_base*> objects;
}game;