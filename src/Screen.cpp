#include <iostream>
#include <glad/glad.h>
#include "Screen.hpp"
#include <functional>

Screen::Screen(int w, int h, std::string judul, float mapW, float mapH) : 
                width(w), 
                height(h), 
                judul(judul), 
                mapWidth(mapW),
                mapHeight(mapH),
                window(nullptr){}

bool Screen::createScreen(){
    if(!glfwInit()){
        std::cout << "Gagal insialisasi GLFW" << std::endl; 
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(this->width, this->height, this->judul.c_str(), NULL, NULL);

    
    if(window == NULL){
        std::cout << "Gagal membuat window" << std::endl;
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(window);

    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, frameBufferCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Gagal menginisialisasi GLAD" << std::endl;
        return false;
    }

    projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

    std::cout << this << std::endl;

    return true;
}

bool Screen::createObject(){
    if(!initObject(roket, roketProp, "roket", 400.0f, 400.0f))return false;
    if(!initObject(triangle, triangleProp, "triangle", 0.0f, 0.0f))return false;
    if(!initObject(ground, groundProp, "ground", 1000.0f, 700.0f))return false;

    land.push_back(ground);
    land.push_back(triangle);
    return true;
}

void Screen::drawObject(Triangle* shape){
    setVPModel(shape);
    shape->drawTriangle();
}

void Screen::drawModel(){
    drawObject(roket);
    drawObject(triangle);
    drawObject(ground);
}

void Screen::terminateObject(){
    roket->terminateTriangle();
    triangle->terminateTriangle();
    ground->terminateTriangle();

    delete(roket);
    delete(triangle);
    delete(ground);
}

void Screen::setView(){
    view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(cameraX, cameraY, 0.0f));
}

void Screen::setVPModel(Triangle* shape){
    shape->triangelShader.useShader();
    shape->triangelShader.setMat4("view", view);
    shape->triangelShader.setMat4("projection", projection);
}

bool Screen::shouldClose(){
    return glfwWindowShouldClose(window);
}

void Screen::updateScreen(){
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Screen::calculateDeltaTime(){
    float currentTime = static_cast<float>(glfwGetTime());
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
}

float Screen::getDeltaTime(){
    return deltaTime;
}

void Screen::keyCallback(GLFWwindow* window, int key,int scancode, int action, int mods){
    Screen* currentScreenn = static_cast<Screen*>(glfwGetWindowUserPointer(window));
}

void Screen::frameBufferCallback(GLFWwindow* window, int w, int h){
    Screen* currentScreen = static_cast<Screen*>(glfwGetWindowUserPointer(window));

    glViewport(0, 0, w, h);

    if(currentScreen != nullptr){
        currentScreen->height = h;
        currentScreen->width = w;

        currentScreen->projection = glm::ortho(0.0f, (float)w, (float)h, 0.0f, -1.0f, 1.0f);

    }
}

void Screen::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
    Screen* currentScreen = static_cast<Screen*>(glfwGetWindowUserPointer(window));

    if(currentScreen == nullptr)return;

    if(button == GLFW_MOUSE_BUTTON_LEFT){
        if(action == GLFW_PRESS){
            currentScreen->isDragging = true;

            glfwGetCursorPos(window, &currentScreen->lastXCursor, &currentScreen->lastYCursor);
            std::cout << currentScreen->lastXCursor << ", " << currentScreen->lastYCursor << std::endl;
        }else if(action == GLFW_RELEASE){
            currentScreen->isDragging = false;
        }
    }

}

void Screen::cursorPosCallback(GLFWwindow* window, double xPos, double yPos){
    Screen* currentScreen = static_cast<Screen*>(glfwGetWindowUserPointer(window));

    if(currentScreen != nullptr && currentScreen->isDragging){
        double deltaX = xPos - currentScreen->lastXCursor;
        double deltaY = yPos - currentScreen->lastYCursor;

        currentScreen->lastXCursor = xPos;
        currentScreen->lastYCursor = yPos;

        currentScreen->cameraX += (float)deltaX;
        currentScreen->cameraY += (float)deltaY;
    }
}

void Screen::scrollCallback(GLFWwindow* window, double xOff, double yOff){
    Screen* currentScreen = static_cast<Screen*>(glfwGetWindowUserPointer(window));

    if(currentScreen == nullptr) return;

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    float worldX = (float)mouseX * currentScreen->zoomLevel - currentScreen->cameraX;
    float worldY = (float)mouseY * currentScreen->zoomLevel - currentScreen->cameraY;

    
    if(yOff < 0){
       currentScreen->zoomLevel += 0.1f; 
    }else{
        currentScreen->zoomLevel -= 0.1f; 
    }

    currentScreen->cameraX = (float)mouseX * currentScreen->zoomLevel - worldX;
    currentScreen->cameraY = (float)mouseY * currentScreen->zoomLevel - worldY;

    currentScreen->projection = glm::ortho(0.0f, (float)currentScreen->width * currentScreen->zoomLevel, (float)currentScreen->height * currentScreen->zoomLevel, 0.0f, -1.0f, 1.0f);
}

void Screen::processMove(){
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) roket->physic.rotateLeft();
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) roket->physic.rotateRight();
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        roket->physic.thrust();
    }else if(glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE){
        roket->physic.setForceUp(0.0f);
    }
}