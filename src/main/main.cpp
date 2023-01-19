
// all project files must be included using the external file include syntax, with angle brackets
// #include <foobar.hpp>


#include <iostream>

#include <SDL.h>

#include <graphics/graphic_manager.hpp>


int main() {

    std::cout << "rimboar" << std::endl;
    GraphicManager graphical_manager{boarglib::Vector2i32{1280,720}};

    SDL_Event event;
    bool shall_quit = false;
    while(!shall_quit)
    {

        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                shall_quit = true;
            else if(event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        shall_quit = true;
                        continue;
                    
                    default:
                        continue;
                }

            }
        }
        
        graphical_manager.render();
    }
}
