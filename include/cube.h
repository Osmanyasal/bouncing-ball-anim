#ifndef INCLUDE__CUBE_H
#define INCLUDE__CUBE_H

#include "shape.h"

/**
 * @brief Cube class, that initiates its vertices and indexes,
 * 
 * 
 */
class Cube : public Shape
{
private:
    float size;

public:
    Cube(float size)
    {
        shape_type = ShapeType::CUBE;
        this->size = size;

        for (int i = 0; i < 72; i++)
        {
            vertices[i] = vertices[i] * size;
        } 
        vertice_length = 72;
        index_length = 36;
    }
    virtual float *get_vertices() override;
    virtual int *get_indexes() override;
    ~Cube() {}

private:
    float vertices[72] = {
        // front face
        -1.0f, -1.0f, 1.0f, // bottom-left
        1.0f, -1.0f, 1.0f,  // bottom-right
        1.0f, 1.0f, 1.0f,   // top-right
        -1.0f, 1.0f, 1.0f,  // top-left

        // back face
        -1.0f, -1.0f, -1.0f, // bottom-left
        -1.0f, 1.0f, -1.0f,  // top-left
        1.0f, 1.0f, -1.0f,   // top-right
        1.0f, -1.0f, -1.0f,  // bottom-right

        // left face
        -1.0f, -1.0f, -1.0f, // bottom-back
        -1.0f, -1.0f, 1.0f,  // bottom-front
        -1.0f, 1.0f, 1.0f,   // top-front
        -1.0f, 1.0f, -1.0f,  // top-back

        // right face
        1.0f, -1.0f, 1.0f,  // bottom-front
        1.0f, -1.0f, -1.0f, // bottom-back
        1.0f, 1.0f, -1.0f,  // top-back
        1.0f, 1.0f, 1.0f,   // top-front

        // top face
        -1.0f, 1.0f, 1.0f,  // front-left
        1.0f, 1.0f, 1.0f,   // front-right
        1.0f, 1.0f, -1.0f,  // back-right
        -1.0f, 1.0f, -1.0f, // back-left

        // bottom face
        -1.0f, -1.0f, -1.0f, // back-left
        1.0f, -1.0f, -1.0f,  // back-right
        1.0f, -1.0f, 1.0f,   // front-right
        -1.0f, -1.0f, 1.0f   // front-left
    };
    int indexes[36] = {
        0, 1, 2, // front face
        2, 3, 0,

        4, 5, 6, // back face
        6, 7, 4,

        8, 9, 10, // left face
        10, 11, 8,

        12, 13, 14, // right face
        14, 15, 12,

        16, 17, 18, // top face
        18, 19, 16,

        20, 21, 22, // bottom face
        22, 23, 20};
};

#endif