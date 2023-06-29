#ifndef _INCLUDE__SCREEN_H
#define _INCLUDE__SCREEN_H

#include <iostream>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shape.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cube.h"
/**
 * @brief This is the Screen class that creates the whole scene
 *
 */
class Screen
{
public:
    Screen(std::string title);
    ~Screen();
    void draw(Shape *shape);
    std::vector<std::string> textures;

    bool use_phong_shading = true;
    bool is_plastic = true;
    bool is_shading = true;
    
    glm::vec3 ambient_color{0.2f, 0.2f, 0.2f};
    
    // start with plastic
    glm::vec3 diffuse_color{0.8f, 0.8f, 0.8f}; //{0.8f, 0.8f, 0.8f};
    glm::vec3 specular_color{0.1f, 0.1f, 0.1f};//{1.0f, 1.0f, 1.0f};
    glm::vec3 light_direction{1.0f, 1.0f, -1.0f}; 
    float shininess = 32.0f;


    int remove_shading_index = -1;
    void remove_shading()
    {
        ambient_color = glm::vec3{0.2f, 0.2f, 0.2f};
        diffuse_color = glm::vec3{0.8f, 0.8f, 0.8f};
        specular_color = glm::vec3{1.0f, 1.0f, 1.0f};

        if (remove_shading_index == 0)
        {
            ambient_color = glm::vec3{0, 0, 0};
            std::cout << "ambient removed\n";
        }
        else if (remove_shading_index == 1)
        {
            diffuse_color = glm::vec3{0, 0, 0};
            std::cout << "diffuse removed\n";
        }
        else if (remove_shading_index == 2){
            specular_color = glm::vec3{0, 0, 0};
            std::cout << "specular removed\n";
        }
        else {
            std::cout << "shading reset!\n";
        }
    }

private:
    int texture_id = 100;
    std::string title;
    static unsigned int mode;
    Shape *current_shape = nullptr;
    GLFWwindow *window = nullptr;
    unsigned int shader_program;
    const unsigned int SCR_WIDTH = 800  ;
    const unsigned int SCR_HEIGHT = 600;

    float zoom_factor = 45.0f;
    float aspectRatio = (float)this->SCR_WIDTH / (float)this->SCR_HEIGHT;
    float zoomedWidth = 2.0f / this->zoom_factor;
    float zoomedHeight = zoomedWidth / aspectRatio;

    float leftEdge = -0.9;
    float rightEdge = .9;
    float topEdge = .9;
    float bottomEdge = -.9;
    float camera_distance = -2.5;

    GLFWwindow *init_screen(std::string title);

    // TEXTURE
    void apply_texture(std::string file_name);

    // SHADER METHODS : where we read and install shaders
    unsigned int init_shaders();
    unsigned int init_vertex_shader(std::string shader_name);
    unsigned int init_fragment_shader(std::string shader_name);

    // CALLBACK MEDIATORS: redirect requests to objects callback methods
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    static void mouse_callback_mediator(GLFWwindow *window, int button, int action, int mods);
    static void key_callback_mediator(GLFWwindow *window, int key, int scancode, int action, int mods);

    // OBJECT CALLBACK METHODS: where we process user input.
    void mouse_callback(GLFWwindow *window, int button, int action, int mods);
    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

    // HELPER : press "h" to see helper
    static void print_help();
};

#endif