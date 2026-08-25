#ifndef MESH_HPP
#define MESH_HPP

#pragma once
#include <vector>

typedef std::vector<float> floatArr;
typedef std::vector<unsigned int> uintArr;

class Mesh{
    private:
        floatArr verticles;
        uintArr indices;
        unsigned int VAO, VBO, EBO;
    
    public:
        Mesh(floatArr verticle, uintArr indices);
        
        void bindVao();
        void del();
        bool meshInit();
    };

#endif