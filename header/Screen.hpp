#ifndef SCREEN_HPP
#define SCREEN_HPP

#pragma once
#include <string>
#include <iostream>
#include <GLFW/glfw3.h>
#include "Triangle.hpp"
#include "Physic.hpp"

class Screen{
    private:
        int width, height;  
        float speed;

        float deltaTime = 0.0f;
        float lastTime = 0.0f;

        float mapWidth, mapHeight;

        float cameraX = 0.0f; 
        float cameraY = 0.0f;
        double lastXCursor = 0.0f;
        double lastYCursor = 0.0f;
        bool isDragging = false;

        float zoomLevel = 1.0f;
        float isZooming = false;

        std::string judul;
        GLFWwindow* window;

        glm::mat4 projection;
        glm::mat4 view;

        struct RoketProp
        {
            float mass = 1000.0f;

            std::string vertPath = "assets/shaders/triangle.vert";
            std::string fragPath = "assets/shaders/triangle.frag";

            std::vector<float> verticles ={
                0.0f, -55.0f, 0.0f, 
                10.0f, -20.0f, 0.0f,
                -10.0f, -20.0f, 0.0f,
                10.0f, 20.0f, 0.0f,
                -10.0f, 20.0f, 0.0f, 
                2.5f, 20.0f, 0.0f,
                -2.5f, 20.0f, 0.0f,
                10.0f, 30.0f, 0.0f,
                2.5f, 30.0f, 0.0f,
                -2.5f, 30.0f, 0.0f,
                -10.0f, 30.0f, 0.0f,
                2.5f, -20.0f, 0.0f,
                -2.5f, -20.0f, 0.0f,
            };

            std::vector<unsigned int> indices = {
                1, 0, 2,
                1, 3, 5,
                1, 11, 5,
                11, 5, 6,
                11, 12, 6,
                12, 6, 4,
                12, 4, 2,
                7, 5, 8,
                5, 8, 9, 
                5, 9, 6,
                6, 9, 10

            };
        };

        struct TriangleProp{

            float mass = 100.0f;

            std::string vertPath = "assets/shaders/triangle.vert";
            std::string fragPath = "assets/shaders/triangle.frag";

            std::vector<float> verticles = {
                0.0f, -550.0f, 0.0f, 
                100.0f, -200.0f, 0.0f,
                -100.0f, -200.0f, 0.0f
            };

            std::vector<unsigned int> indices = {
                0, 1, 2
            };
        };

        struct GroundProp{

            float mass = 20.0f * std::pow(10, 12);

            std::string vertPath = "assets/shaders/triangle.vert";
            std::string fragPath = "assets/shaders/triangle.frag";

            std::vector<float> verticles = {
                10000.0f, -100.0f, 0.0f,
                -10000.0f, -100.0f, 0.0f,
                -10000.0f, 10000.0f, 0.0f,
                10000.0f, 10000.0f, 0.0f
            };

            std::vector<unsigned int> indices = {
                0, 1, 2,
                2, 3, 0
            };
        };

        void setVPModel(Triangle* shape);

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void frameBufferCallback(GLFWwindow* window, int w, int h);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);
        static void scrollCallback(GLFWwindow* window, double xOff, double yOff);

    public:
        Triangle* roket = nullptr;
        RoketProp roketProp;        
        
        Triangle* triangle = nullptr;
        TriangleProp triangleProp;        

        Triangle* ground = nullptr;
        GroundProp groundProp;
        
        std::vector<Triangle*> land;

        

        template <typename T>
        bool initObject(Triangle*& shape, T& prop, std::string name, float x, float y){
            shape = new Triangle(prop.verticles, prop.indices, prop.vertPath, prop.fragPath, x, y, prop.mass);
            if(!shape->initTriangel()){
                std::cerr << "ERR : failed to inizialized " << name << std::endl;
                return false;
            }
            return true;
        };
        
        
        void drawObject(Triangle* shape);

        void drawModel();

        Screen(int w, int h, std::string judul, float mapW, float mapH);
        bool createScreen();

        bool createObject();
        void terminateObject();

        bool shouldClose();
        void updateScreen();

        void processMove();

        void setView();

        void calculateDeltaTime();
        float getDeltaTime();
        
        GLFWwindow* getWindow() const { return window; }
};

#endif