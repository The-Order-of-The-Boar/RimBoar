# Scene System

RimBoar has a Scene system where each scene has 3 functions:

* update: This function is executed directly on GameApplication and has access to no graphical context. It is designed to update the state of the scene before rendering.

* update_hud: This function is executed inside ImGuiHandler and has acess to it's context. It is designed to render the Scene HUD, and update the state based on user interactions on HUD.

* render: This function is executed inside SDLHandler and has access to it's renderer and graphical context. It is designed to render the Game Objects and World inside a Scene, with no changes to the state. 

