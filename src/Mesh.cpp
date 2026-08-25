#include "Mesh.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

Mesh::Mesh(floatArr verticles, uintArr indices): verticles(verticles), indices(indices) {}

void Mesh::bindVao(){
    glBindVertexArray(VAO);
}

void Mesh::del(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

bool Mesh::meshInit(){

    if (verticles.empty() || indices.empty()) {
        std::cerr << "ERROR::MESH: Cannot initialize mesh with empty geometry data!" << std::endl;
        return false;
    }
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    if (VAO == 0 || VBO == 0 || EBO == 0) {
        std::cerr << "ERROR::MESH: OpenGL failed to generate buffer IDs!" << std::endl;
        return false;
    }

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticles.size() * sizeof(float), verticles.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return true;
    
}