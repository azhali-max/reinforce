#include <iostream>
#include <glad/glad.h>
#include "Screen.hpp"
#include <functional>

//contructor
Screen::Screen(int w, int h, std::string judul) : 
                width(w), 
                height(h), 
                judul(judul), 
                window(nullptr){}

// inisialisasi screen
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



// membentuk semua object yang dibutuhkan
bool Screen::createObject(){
    //(shape object, prop object, name, x, y)
    if(!initObject(roket, roketProp, "roket", 400.0f, 400.0f))return false;
    if(!initObject(triangle, triangleProp, "triangle", 0.0f, 0.0f))return false;
    // if(!initObject(ground, groundProp, "ground", 1000.0f, 700.0f))return false;
    if(!initObject(planet, planetprop, "planet", 400.0f, 5800.0f))return false;

    // yang merupakan land

    // land.push_back(ground); 
    land.push_back(triangle);
    land.push_back(planet);
    return true;
}

// method gambar object dalam screen dan set model
void Screen::drawObject(Triangle* shape){
    setVPModel(shape);
    shape->drawTriangle();
}

// gambar object yang diperlukan
void Screen::drawModel(){
    drawObject(roket);
    drawObject(triangle);
    // drawObject(ground);
    drawObject(planet);
}

// menghapus object dari pointer
void Screen::terminateObject(){
    roket->terminateTriangle();
    triangle->terminateTriangle();
    // ground->terminateTriangle();
    planet->terminateTriangle();

    delete(roket);
    delete(triangle);
    // delete(ground);
    delete(planet);
}

std::tuple<std::vector<float>, std::vector<unsigned int>, float> Screen::makePoligon(int count, float length, float mass){

    
    if(count < 3){
        std::cerr << "count must be over 2" << std::endl;
        return {};
    }
    
    std::vector<float> vert;
    std::vector<unsigned int> indices;
    float rad = glm::radians(360.0f/count);

    float x = 0.0f;
    float y = length;

    float pivot = 0.0f;
    float oldX = 0.0f;
    float oldY = length;

    vert.push_back(pivot);
    vert.push_back(pivot);
    vert.push_back(pivot);

    vert.push_back(oldX);
    vert.push_back(oldY);
    vert.push_back(0.0f);

    float cosRad = std::cos(rad);
    float sinRad = std::sin(rad);

    for(int i = 0; i < count-1; i++){
        oldX = x;
        oldY = y;

        x = (oldX - pivot)*cosRad - (oldY - pivot)*sinRad + pivot;
        y = (oldX - pivot)*sinRad + (oldY - pivot)*cosRad + pivot;

        vert.push_back(x);
        vert.push_back(y);
        vert.push_back(0.0f);
    }

    for(int i = 0; i < count; i++){
        indices.push_back(0);

        indices.push_back(i + 1);
        indices.push_back(((i + 1) % count) + 1);

    }

    return {vert, indices, mass};
}

// set view 
void Screen::setView(){
    view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(cameraX, cameraY, 0.0f));
}

// method set view pada model
void Screen::setVPModel(Triangle* shape){
    shape->triangelShader.useShader();
    shape->triangelShader.setMat4("view", view);
    shape->triangelShader.setMat4("projection", projection);
}

// jika window di tutup
bool Screen::shouldClose(){
    return glfwWindowShouldClose(window);
}

// untuk per frame
void Screen::updateScreen(){
    glfwSwapBuffers(window);
    glfwPollEvents();
}

// menghitung delta time 
void Screen::calculateDeltaTime(){
    float currentTime = static_cast<float>(glfwGetTime());

    if (lastTime == 0.0f) {
        lastTime = currentTime;
    }

    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    if (deltaTime > 0.005f) {
        deltaTime = 0.005f; 
    }

    std::cout << deltaTime << std::endl;
}

// pass delta time 
float Screen::getDeltaTime(){
    return deltaTime;
}

// detect perubahan bentuk screen minimize/maximize
void Screen::frameBufferCallback(GLFWwindow* window, int w, int h){
    Screen* currentScreen = static_cast<Screen*>(glfwGetWindowUserPointer(window));

    glViewport(0, 0, w, h);

    if(currentScreen != nullptr){
        currentScreen->height = h;
        currentScreen->width = w;

        currentScreen->projection = glm::ortho(0.0f, (float)w * currentScreen->zoomLevel, (float)h * currentScreen->zoomLevel, 0.0f, -1.0f, 1.0f);

    }
}

// detect mouse button
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

//detect cursor movemnet
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

//detect scrolling
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

// method untuk memproses movement
void Screen::processMove(){
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) roket->physic.rotateLeft();
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) roket->physic.rotateRight();
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        roket->physic.thrust();
    }else if(glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE){
        roket->physic.setForceUp(0.0f);
    }
}