
// all project files must be included using the external file include syntax, with angle brackets
// #include <foobar.hpp>


#include <iostream>

#include <graphics/graphic_manager.hpp>


int main() {

    std::cout << "rimboar" << std::endl;
    GraphicManager graphical_manager{boarglib::Vector2i32{1280,720}};

    while(!graphical_manager.shall_quit)
    {
        graphical_manager.process_input();
        graphical_manager.render();
    }
}
