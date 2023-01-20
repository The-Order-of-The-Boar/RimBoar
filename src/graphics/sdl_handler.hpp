
//builtin
#include <functional>

//local
#include "../data_structures/vector.hpp"

struct SDL_Window;
struct SDL_Renderer;

class SDLHandler
{
public:
    boarglib::Vector2i32 window_size;
    SDL_Window* window;
    SDL_Renderer* renderer;

public:
    SDLHandler(const boarglib::Vector2i32 window_size);
    ~SDLHandler();

    void render(std::function<void(SDL_Renderer*)> render_func);

};

