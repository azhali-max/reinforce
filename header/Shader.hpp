#ifndef SHADER_HPP
#define SHADER_HPP

#pragma once
#include <string>
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef std::string str;

class Shader{
    public:
        unsigned int id;
        str filepathVert, filepathFrag;

        Shader(str filepathVert, str filepathFrag);

        std::tuple<str, str> getShader();
        unsigned int initShader();
        void setVec3(const std::string &name, float x, float y, float z);
        void setMat4(const std::string &name, const glm::mat4 &mat);
        void useShader();
        void delShader();

};

#endif