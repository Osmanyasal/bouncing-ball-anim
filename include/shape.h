#ifndef _INCLUDE__SHAPE_H
#define _INCLUDE__SHAPE_H

/**
 * @brief Main Shape object that holds vertices and indexes of the object
 * along with status of the object (position,location,any forces on it st acceleration,x-y speeds etc.)
 *
 */
enum class ShapeType
{
    SPHERE = 0,
    CUBE = 1
};
class Shape
{
public:
    ShapeType shape_type;
    ~Shape() {}
    int vertice_length;
    int index_length;

    // DEFAULT STATUS OF THE SHAPE
    float initialXPosition = 0.0f;
    float initialYPosition = 0.0f;
    float xPosition = -initialXPosition;
    float yPosition = initialYPosition;
    float yAcceleration = 9.81f;
    float xSpeed = 0.025f;
    float ySpeed = 0.0f;
    bool falling = true;

    float red = 204/255.0;
    float green = 102/255.0;
    float blue = 0;
    float alpha = 1;

    virtual void copy_status(Shape *target);
    virtual float *get_vertices() = 0;
    virtual int *get_indexes() = 0;
};

#endif