
#include "shape.h"

void Shape::copy_status(Shape* target){
    target->initialXPosition = this->initialXPosition;
    target->initialYPosition = this->initialYPosition;
    target->xPosition = this->xPosition;
    target->yPosition = this->yPosition;
    target->yAcceleration = this->yAcceleration;
    target->xSpeed = this->xSpeed;
    target->ySpeed = this->ySpeed;
    target->falling = this->falling;

    target->red = this->red;
    target->blue = this->blue;
    target->green = this->green;
    target->alpha = this->alpha;
}