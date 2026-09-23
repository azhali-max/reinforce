#ifndef SCREEN_HPP
#define SCREEN_HPP

#pragma once
#include <string>
#include <iostream>
#include <GLFW/glfw3.h>
#include "Triangle.hpp"
#include "Physic.hpp"
#include <tuple>

class Screen{
    private:
        //properti screen
        int width, height;  

        //untuk menghitung deltaTime
        float deltaTime = 0.0f;
        float lastTime = 0.0f;

        //camera pos
        float cameraX = 0.0f; 
        float cameraY = 0.0f;

        //cursor pos
        double lastXCursor = 0.0f;
        double lastYCursor = 0.0f;

        //drag condition
        bool isDragging = false;

        //zoom
        float zoomLevel = 1.0f;
        float isZooming = false;

        //for screen
        std::string judul;
        GLFWwindow* window;

        //shader projection view
        glm::mat4 projection;
        glm::mat4 view;


        //Roket properties
        struct RoketProp
        {
            float mass = 1000.0f;

            std::string vertPath = "assets/shaders/triangle.vert";
            std::string fragPath = "assets/shaders/triangle.frag";

            std::vector<float> verticles ={
                0.0, 0.0, 0.0,
                -10.0, -20.0, 0.0,
                0.0, -35.0, 0.0,
                10.0, -20.0, 0.0,
                -10.0, 20.0, 0.0,
                -2.5, 20.0, 0.0,
                2.5, 20.0, 0.0,
                10.0, 20.0, 0.0,
                0.0, 27.5, 0.0,
                -10.0, 35.0, 0.0,
                -2.5, 35.0, 0.0,
                2.5, 35.0, 0.0,
                10.0, 35.0, 0.0,
                10.0, 35.0, 0.0,
                10.0, 35.0, 0.0
            };

            std::vector<unsigned int> indices = {
                3, 2, 1,
                0, 3, 1,
                0, 1, 4,
                0, 4, 7,
                0, 7, 3,
                8, 6, 5,
                8, 5, 10,
                8, 10, 11,
                8, 10, 6,
                5, 9, 10,
                6, 10, 12
            };
        };


        //Triangle properties
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


        //Ground properties
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

        //Planet properties
        struct PlanetProp{
            std::vector<float> verticles;
            std::vector<unsigned int>indices;
            float mass;

            std::string vertPath = "assets/shaders/triangle.vert";
            std::string fragPath = "assets/shaders/triangle.frag";

            PlanetProp(){
                std::tie(verticles, indices, mass) = Screen::makePoligon(100, 3000.0f, 20.0f * std::pow(10, 12));
            }

        };

        void setVPModel(Triangle* shape);

        //event detector
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void frameBufferCallback(GLFWwindow* window, int w, int h);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);
        static void scrollCallback(GLFWwindow* window, double xOff, double yOff);

    public:
        //constructor
        Screen(int w, int h, std::string judul);
        
        //method init screen
        bool createScreen();
        
        //check window close
        bool shouldClose();
        
        //per frame
        void updateScreen();
        
        //inisialisasi view
        void setView();
        
        //menghitung deltaTime
        void calculateDeltaTime();
        float getDeltaTime();

        //get window
        GLFWwindow* getWindow() const { return window; }

// ======================================INISIALISASI OBJECT==================================================================================

        Triangle* roket = nullptr;
        RoketProp roketProp;        
        
        Triangle* triangle = nullptr;
        TriangleProp triangleProp;        

        // Triangle* ground = nullptr;
        // GroundProp groundProp;
        
        Triangle* planet = nullptr;
        PlanetProp planetprop;

        std::vector<Triangle*> land;

// ===========================================================================================================================================
        
        // Buat Object Poligon 
        static std::tuple<std::vector<float>, std::vector<unsigned int>, float> makePoligon(int count, float length, float mass);

// ======================================METHOD INISIALISASI OBJECT==================================================================================

        template <typename T>
        bool initObject(Triangle*& shape, T& prop, std::string name, float x, float y){
            shape = new Triangle(prop.verticles, prop.indices, prop.vertPath, prop.fragPath, x, y, prop.mass);
            if(!shape->initTriangel()){
                std::cerr << "ERR : failed to inizialized " << name << std::endl;
                return false;
            }
            return true;
        };

// ==================================================================================================================================================
        
// ======================================MEMASUKAN OBJECT KE SCREEN==================================================================================
        bool createObject();
        void drawObject(Triangle* shape);
        void drawModel();
        void terminateObject();

// ==================================================================================================================================================

        //method proses movement
        void processMove();

};

#endif