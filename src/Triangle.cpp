#include <iostream>
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Triangle.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//constructor
Triangle::Triangle(std::vector<float> verticles, std::vector<unsigned int> indices, std::string vertPath, std::string fragPath, float x, float y, float mass):
                    verticles(verticles), indices(indices), mesh(verticles, indices), triangelShader(vertPath, fragPath), xOff(x), yOff(y), mass(mass), physic(this, mass){}


//inisialisasi bentuk
bool Triangle::initTriangel(){

    unsigned int id = triangelShader.initShader();

    if (!id) {
        std::cout << "ERROR::TRIANGLE: Shader initialization failed." << std::endl;
        return false;
    }

    if (!mesh.meshInit()) {
        std::cout << "ERROR::TRIANGLE: Mesh initialization failed." << std::endl;
        return false;
    }

    return true;
}

//gambar bentuk
void Triangle::drawTriangle(){
    triangelShader.useShader();
    
    setPosition(xOff, yOff);
    mesh.bindVao();

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

//hapus bentuk
void Triangle::terminateTriangle(){
    mesh.del();
    triangelShader.delShader();
}

//set posisi dan rotasi
void Triangle::setPosition(float x, float y){
    triangelShader.useShader();

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(x, y, 0.0f));

    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

    triangelShader.setMat4("model", model);
}

//mengambil posisi asli masing masing verticle
std::vector<glm::vec2> Triangle::getObjectWorldPos(){
    std::vector<glm::vec2> vertPos;

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(xOff, yOff, 0.0f));

    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

    for(int i = 0; i < verticles.size(); i += 3){
        glm::vec4 localPos(verticles[i], verticles[i+1], 0.0f, 1.0f);
        glm::vec4 worldPos = model * localPos;

        vertPos.push_back(glm::vec2(worldPos.x, worldPos.y));
    }
    return vertPos;
}

//cek overlap object
std::tuple<bool, glm::vec2, float> Triangle::checkCollision(Triangle* other){
    std::vector<glm::vec2> poly1 = this->getObjectWorldPos();
    std::vector<glm::vec2> poly2 = other->getObjectWorldPos();

    std::vector<std::vector<glm::vec2>> shape = {poly1, poly2};
    glm::vec2 overlapAxis;
    float smallestOverlap = 999999.0f;

    for (int i = 0; i < shape.size(); i++){
        std::vector<glm::vec2> currentShape = shape[i];
        for (int i = 0; i < currentShape.size(); i++){
            glm::vec2 p1 = currentShape[i];
            glm::vec2 p2 = currentShape[(i+1) % currentShape.size()];
            glm::vec2 edge = p2 - p1;

            glm::vec2 axis = glm::normalize(glm::vec2(-edge.y, edge.x));

            float minA = 999999.0f, maxA = -999999.0f;
            for (glm::vec2& p : poly1){
                float projection = glm::dot(p, axis);
                if(projection > maxA) maxA = projection;
                if(projection < minA) minA = projection;
            }

            float minB = 999999.0f, maxB = -999999.0f;
            for (glm::vec2& p : poly2){
                float projection = glm::dot(p, axis);
                if(projection > maxB) maxB = projection;
                if(projection < minB) minB = projection;
            }

            if(maxA < minB || maxB < minA)return {false, glm::vec2(0.0f), 0.0f};


            float overlap = std::min(maxA, maxB) - std::max(minA, minB);
            if(smallestOverlap > overlap){
                smallestOverlap = overlap;
                overlapAxis = axis;
            }

        }
    }

    glm::vec2 center1(0.0f), center2(0.0f);
    for(const auto& p: poly1){
        center1 += p;
    }
    center1 /= poly1.size();

    for(const auto& p: poly2){
        center2 += p;
    }
    center2 /= poly2.size();

    glm::vec2 dir = center2 - center1;

    if (glm::dot(dir, overlapAxis) > 0.0f) {
        overlapAxis = -overlapAxis; 
    }

    return {true, overlapAxis, smallestOverlap};
}



