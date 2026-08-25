#include "Shader.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(str filepathVert, str filepathFrag): filepathVert(filepathVert), filepathFrag(filepathFrag){}

std::tuple<str, str> Shader::getShader(){
    std::ifstream vert(filepathVert);
    if(!vert.is_open()){
        std::cerr << "ERROR: Failed to open shader file at: " << filepathVert << std::endl;
        return {"", ""};
    }

    std::ifstream frag(filepathFrag);
    if(!frag.is_open()){
        std::cerr << "ERROR: Failed to open shader file at: " << filepathFrag << std::endl;
        return {"", ""};
    }

    std::stringstream bufferVert;
    std::stringstream bufferFrag;
    bufferVert << vert.rdbuf();
    bufferFrag << frag.rdbuf();

    return {bufferVert.str(), bufferFrag.str()};
}

unsigned int Shader::initShader(){
    auto [vertSrc, fragSrc] = Shader::getShader();

    if(vertSrc.empty() || fragSrc.empty()){
        return 0; 
    }

    const char* vertexCode = vertSrc.c_str();
    const char* fragCode = fragSrc.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragCode, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }

    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return 0;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return id;
}

void Shader::setVec3(const std::string &name, float x, float y, float z){
    int location = glGetUniformLocation(id, name.c_str());

    glUniform3f(location, x, y, z);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat){
    int location = glGetUniformLocation(id, name.c_str());

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::useShader(){
    glUseProgram(id);
}

void Shader::delShader(){
    glDeleteProgram(id);
}