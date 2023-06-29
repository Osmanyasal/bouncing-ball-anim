#ifndef INCLUDE__SPHERE_H
#define INCLUDE__SPHERE_H
#include <vector>
#include <math.h>
#include "shape.h"

/**
 * @brief Sphere class that initiates it's vertices and indexes 
 * 
 */
class Sphere : public Shape
{

public:
    Sphere(float radius);
    virtual float *get_vertices() override;
    virtual int *get_indexes() override;
    ~Sphere() {}

private:
    std::vector<float> vertices;
    std::vector<int> indexes;
};
#endif