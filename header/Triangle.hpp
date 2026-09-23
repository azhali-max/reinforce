#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#pragma once
#include "Shader.hpp"
#include "Physic.hpp"
#include "Mesh.hpp"
#include <vector>

typedef std::string str;

class Triangle{

    private:
        //inisialisasi mesh
        Mesh mesh;

        //properties 
        std::vector<float> verticles;
        std::vector<unsigned int> indices;

        //physic
        float angle = 0.0f;
        float xOff;
        float yOff;
        float mass;
    
    public:

        Shader triangelShader;

        Physic physic;

        Triangle(std::vector<float> verticles, std::vector<unsigned int> indices, std::string vertPath, std::string fragPath, float x, float y, float mass);

        float getXOff() const {return this->xOff;};
        float getYOff() const {return this->yOff;};
        void setXOff(float xOff) {this->xOff = xOff;}
        void setYOff(float yOff) {this->yOff = yOff;}
        float getMass() const {return this->mass;}
        void setAngle(float angle) {this->angle = angle;}
        float getAngle() const {return this->angle;}

        bool initTriangel();
        void drawTriangle();
        void terminateTriangle();

        void setPosition(float x, float y);

        void setDeltaTime(float deltaTime);

        std::vector<glm::vec2> getObjectWorldPos();
        std::tuple<bool, glm::vec2, float> checkCollision(Triangle* other);
        glm::vec2 getCentroid();

        //move
        void moveRight();
        void moveLeft();
        void moveUp();
        void moveDown();
        void increaseSpeed();
        void decreaseSpeed();

        void gravityActive(bool Active);

};

#endif