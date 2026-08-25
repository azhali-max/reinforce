#include "Screen.hpp"
#include <iostream>
#include <cmath>
#include <filesystem>

int main(){
    
    Screen screen(800, 800, "Cobaan", 2000.0f, 2000.0f);

    if(!screen.createScreen()){
        std::cerr << "inisialisasi screen gagal" << std::endl;
        return -1;
    }

    if(!screen.createObject()){
        std::cerr << "gagal inisialisasi object" << std::endl;
        return -1;
    }

    while(!screen.shouldClose()){

        screen.calculateDeltaTime();
        screen.roket->physic.setDeltaTime(screen.getDeltaTime());

        screen.roket->saveSafePosition();

        screen.processMove();
        screen.roket->physic.procesGravity(screen.land);
        screen.roket->physic.procesCollision(screen.land);

        glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        screen.setView();

        screen.drawModel();
        screen.updateScreen();
    }

    screen.terminateObject();

    return 0;

}
