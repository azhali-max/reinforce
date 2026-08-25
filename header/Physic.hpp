#ifndef PHYSIC_HPP
#define PHYSIC_HPP

#pragma once

class Triangle;

class Physic{
    private:
        Triangle* shape = nullptr;
        const float GRAVITY_CONSTAN = 0.000000000066743f;
        const float PIXEL = 32;
        float mass;
        float xPull = 0.0f, 
        yPull = 0.0f, 
        forceUp = 0.0f, 
        forceRotate = 100.0f, 
        angularVelocity = 0.0f;

        float deltaTime = 0.0f;

    public:

        Physic(Triangle* shape, float mass);
        void setDeltaTime(float deltaTime){this->deltaTime = deltaTime;}

        std::vector<float> getGravity(Triangle* otherShape, float otherMass);

        float getMass() const {return mass;}

        void procesGravity(std::vector<Triangle*> land);
        void procesCollision(std::vector<Triangle*> other);

        void thrust();
        void rotateRight();
        void rotateLeft();

        void setForceUp(float force) {this->forceUp = force;}
};

#endif