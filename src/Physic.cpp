#include "Triangle.hpp"
#include "Physic.hpp"
#include <iostream>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <cmath>

Physic::Physic(Triangle* shape, float mass): 
                shape(shape), mass(mass){}

std::vector<float> Physic::getGravity(Triangle* otherShape, float otherMass){
    float rX = (otherShape->getXOff() - shape->getXOff())/PIXEL;
    float rY = (otherShape->getYOff() - shape->getYOff())/PIXEL;

    float distance = std::sqrt((rX * rX) + (rY * rY));

    if (distance < 1.0f) distance = 1.0f;

    float totalForce = GRAVITY_CONSTAN * ((otherMass * mass) / (distance * distance));

    float xPull = totalForce * (rX / distance);
    float yPull = totalForce * (rY / distance);

    return {xPull, yPull};
}

void Physic::procesGravity(std::vector<Triangle*> land){
    for (Triangle* l : land){

        std::vector<float> force = shape->physic.getGravity(l, l->getMass());
        this->xPull += force[0] * deltaTime;
        this->yPull += force[1] * deltaTime;
    }

    shape->setXOff(shape->getXOff() + xPull * deltaTime);
    shape->setYOff(shape->getYOff() + yPull * deltaTime);

    shape->setAngle(shape->getAngle() + angularVelocity * deltaTime);

    this->xPull *= 0.99f;
    this->yPull *= 0.99f;

    this->angularVelocity *= 0.95f;
}

void Physic::procesCollision(std::vector<Triangle*> other){
    for (Triangle* o : other){
        auto [collision, axis, depth] = shape->checkCollision(o);
        if(collision){
            
            shape->setXOff(shape->getXOff() + (axis.x * depth));
            shape->setYOff(shape->getYOff() + (axis.y * depth));

            // float rad = glm::radians(shape->getAngle());

            // angularVelocity -= std::sin(rad) * forceRotate * deltaTime;
            
            xPull = 0.0f;
            yPull = 0.0f;

            break;
        }
    }
}

//Movement

void Physic::thrust(){
    forceUp += 1000.0f * PIXEL * deltaTime; 
    if(forceUp >= 10000.0f * PIXEL) forceUp = 10000.0f * PIXEL;
    float rad = glm::radians(shape->getAngle());

    float forceX = std::sin(rad) * forceUp * deltaTime;
    float forceY = -std::cos(rad) * forceUp * deltaTime;

    xPull = forceX;
    yPull = forceY;

    std::cout << "\n" << "force x " << forceX << "\n" 
              << "force y " << forceY << "\n"  
              << "y pull " << yPull << "\n"
              << "x pull " << xPull << "\n"
              << "x off " << shape->getXOff() <<  "\n"
              << "y off " << shape->getYOff() << std::endl;
              
}

void Physic::rotateLeft(){
    angularVelocity -= forceRotate * deltaTime;
}

void Physic::rotateRight(){
    angularVelocity += forceRotate * deltaTime;
}


