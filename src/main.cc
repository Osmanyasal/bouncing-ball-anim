
/*  
                  GROUP MEMBERS
    Berke Derin Berktay Bberktay19@ku.edu.tr
    Osman Yasal         oyasal22@ku.edu.tr

*/


#include <iostream>
#include "screen.h"
#include "shader_utils.h"
#include "sphere.h"
#include "texture_utils.h"

int main()
{   
    // Create a screen with given title
    Screen screen("opengl_hw3"); 
    

    Sphere *sphere = new Sphere{1.0f};

    // draw the screen with given object
    screen.draw(sphere);
    delete sphere;
    return 0;
}
