#include "sphere.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sphere::Sphere(float radius)
{
    shape_type = ShapeType::SPHERE;
    int rings = 25;
    int sectors = 30;

    float const R = 1.0f / (float)(rings - 1);
    float const S = 1.0f / (float)(sectors - 1);

    for (int r = 0; r < rings; ++r)
    {

        float theta = static_cast<float>(r) / rings * glm::pi<float>(); // Vertical angle [0, pi]
        float texV = static_cast<float>(r) / rings;
        for (int s = 0; s < sectors; ++s)
        {
            float const y = sin(-M_PI_2 + M_PI * r * R);
            float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
            float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

            float phi = static_cast<float>(s) / sectors * 2.0f * glm::pi<float>(); // Horizontal angle [0, 2pi]
            float texU = static_cast<float>(s) / sectors;                          // Texture U coordinate [0, 1]

            vertices.push_back(x * radius);
            vertices.push_back(y * radius);
            vertices.push_back(z * radius);

            vertices.push_back(texU);
            vertices.push_back(texV);
        }
    }

    for (int r = 0; r < rings - 1; ++r)
    {
        for (int s = 0; s < sectors - 1; ++s)
        {
            int const idx = r * sectors + s;
            indexes.push_back(idx);
            indexes.push_back(idx + 1);
            indexes.push_back(idx + sectors);

            indexes.push_back(idx + sectors);
            indexes.push_back(idx + 1);
            indexes.push_back(idx + sectors + 1);
        }
    }

    vertice_length = vertices.size();
    index_length = indexes.size();
}

float *Sphere::get_vertices()
{
    return vertices.data();
}

int *Sphere::get_indexes()
{
    return indexes.data();
}