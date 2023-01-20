
//local
#include "./graphics/graphic_manager.hpp"

class GameApplication
{
private:
    bool shall_quit = false;

public:
    GraphicManager graphic_manager{boarglib::Vector2i32{1280,720}};

    void run();

private:
    void handle_input();

};