#include <math.h>
#include "screen.h"
#include "shader_utils.h"
#include "sphere.h"
#include "cube.h"
#include "mat.h"

#include "texture_utils.h"

unsigned int Screen::mode = GL_FILL;

Screen::Screen(std::string title)
{
    this->title = title;
    this->textures.push_back("./earth.ppm");
    this->textures.push_back("./basketball.ppm");
    light_direction = glm::normalize(light_direction);

    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
}

Screen::~Screen()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

/**
 * @brief Creates a window with default settings, Set's callback methods
 *
 * @param title title of the screen
 * @return GLFWwindow*
 */
GLFWwindow *Screen::init_screen(std::string title)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwSetWindowUserPointer(window, this);
    glfwSetWindowTitle(window, title.c_str());
    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // SET CALLBACKS
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback_mediator);
    glfwSetMouseButtonCallback(window, mouse_callback_mediator);

    glewInit();
    return window;
}

/**
 * @brief Creates a shader_program then loads and installs vertex and fragment shaders to the program
 *
 * @return unsigned int shader_program
 */
unsigned int Screen::init_shaders()
{

    unsigned int vs = init_vertex_shader("vertex_shader");
    unsigned int fs = init_fragment_shader("fragment_shader");

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return shaderProgram;
}

/**
 * @brief initiates vertex shader by reading it from the given source
 *
 * @param shader_name
 * @return unsigned int vertex_shader
 */
unsigned int Screen::init_vertex_shader(std::string shader_name)
{
    std::string vertex_shader_source = ShaderUtils::load_shader(shader_name);
    std::cout << vertex_shader_source;

    const char *vs_source(vertex_shader_source.c_str());

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vs_source, NULL);
    glCompileShader(vertex_shader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    return vertex_shader;
}

/**
 * @brief initiates fragment shader by reading it from the given source
 *
 * @param shader_name
 * @return unsigned int
 */
unsigned int Screen::init_fragment_shader(std::string shader_name)
{
    std::string fragment_shader_source = ShaderUtils::load_shader(shader_name);
    std::cout << fragment_shader_source;

    const char *vs_source(fragment_shader_source.c_str());

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &vs_source, NULL);
    glCompileShader(fragment_shader);

    int success;
    char infoLog[512];
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    return fragment_shader;
}

/**
 * @brief Generic draw method, draws the given shape
 * by using its vertices and indexes along with its default status (location, forces etc)
 *
 * @param shape
 */
void Screen::draw(Shape *shape)
{
    current_shape = shape;
    if (window == nullptr)
        window = init_screen(title);

    shader_program = init_shaders();

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * current_shape->vertice_length, current_shape->get_vertices(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * current_shape->index_length, current_shape->get_indexes(), GL_STATIC_DRAW);

    apply_texture(textures[(texture_id % textures.size())]);
    texture_id += 1;

    unsigned int texture = glGetUniformLocation(shader_program, "texCoord");
    unsigned int model_pos = glGetUniformLocation(shader_program, "model_pos");
    unsigned int custom_color = glGetUniformLocation(shader_program, "custom_color");

    GLint projectionLocation = glGetUniformLocation(shader_program, "projection");

    // Get the locations of the directional light uniforms in the shader program
    GLuint lightDirectionLoc = glGetUniformLocation(shader_program, "lightDirection");
    GLuint isShadingLoc = glGetUniformLocation(shader_program, "isShading");

    GLuint ambientColorLoc = glGetUniformLocation(shader_program, "ambientColor");
    GLuint diffuseColorLoc = glGetUniformLocation(shader_program, "diffuseColor");
    GLuint specularColorLoc = glGetUniformLocation(shader_program, "specularColor");
    GLuint shininessLoc = glGetUniformLocation(shader_program, "shininess");
    GLuint usephongLog = glGetUniformLocation(shader_program, "use_phong_shading");

    float time = (1 / 60.0f); // 60 fps
    float previous = glfwGetTime();

    // current_shape->xPosition = leftEdge + 0.2f;
    // current_shape->yPosition = topEdge - 0.2f;
    float zNear = 0.1f;  // Near clipping plane
    float zFar = 100.0f; // Far clipping plane

    // Enable back face culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_program);

        float now = glfwGetTime();
        float delta = now - previous;
        previous = now;

        time -= delta;

        if (time <= 0.0f)
        {

            if (current_shape->yPosition < bottomEdge)
            {
                current_shape->ySpeed = -current_shape->ySpeed * 0.8f; // energy loss due to collision
                current_shape->yPosition = bottomEdge;                 // prevent cube from going below the ground
            }
            else
            {
                current_shape->ySpeed -= current_shape->yAcceleration * delta; // simulate gravity
            }

            current_shape->yPosition += current_shape->ySpeed * delta; // update position using speed and time
            current_shape->xPosition += current_shape->xSpeed;
            if (current_shape->xPosition > rightEdge || current_shape->xPosition < leftEdge)
            {

                current_shape->xSpeed = -current_shape->xSpeed; // toggle the x_speed value
            }
            current_shape->xSpeed = current_shape->xSpeed - current_shape->xSpeed * 0.0009f; // reduce the x_speed by 10%

            if (std::abs(current_shape->ySpeed) <= 0.3f && std::abs(current_shape->yPosition) > rightEdge)
            {
                current_shape->ySpeed = 0;
                if (current_shape->xSpeed > 0)
                    current_shape->xSpeed -= current_shape->xSpeed * 0.07;
            }

            // Add one directional light

            glm::mat4 trans{1.0f};
            trans = glm::translate(trans, glm::vec3(current_shape->xPosition, current_shape->yPosition, camera_distance));
            // trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
            // trans = glm::rotate(trans, 30.0f * std::abs(current_shape->xSpeed) * time, glm::vec3(1 * current_shape->xSpeed / std::abs(current_shape->xSpeed), 0, (1 * current_shape->xSpeed) / std::abs(current_shape->xSpeed)));

            trans = glm::scale(trans, glm::vec3(0.08, 0.08, 0.08));
            trans = glm::rotate(trans, 400.0f * std::abs(current_shape->xSpeed) * time, glm::vec3(1 * current_shape->xSpeed / std::abs(current_shape->xSpeed), 0, (1 * current_shape->xSpeed) / std::abs(current_shape->xSpeed)));

            glm::mat4 projection = glm::perspective(glm::radians(this->zoom_factor), (float)this->SCR_WIDTH / (float)this->SCR_HEIGHT , zNear, zFar);
            glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(model_pos, 1, GL_FALSE, glm::value_ptr(trans));
            glUniform4f(custom_color, current_shape->red, current_shape->green, current_shape->blue, current_shape->alpha);

            // Set the directional light uniforms in the shader program
            glUniform3fv(lightDirectionLoc, 1, glm::value_ptr(light_direction));
            glUniform3fv(ambientColorLoc, 1, {glm::value_ptr(ambient_color)});
            glUniform3fv(diffuseColorLoc, 1, glm::value_ptr(diffuse_color));
            glUniform3fv(specularColorLoc, 1, glm::value_ptr(specular_color));
            glUniform1f(shininessLoc, shininess);
            glUniform1i(isShadingLoc,is_shading);
            glUniform1i(usephongLog, use_phong_shading);

            glDrawElements(GL_TRIANGLES, current_shape->index_length, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Screen::apply_texture(std::string file_name)
{

    PPM ppm = TextureUtils::load_ppm(file_name);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ppm.width, ppm.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ppm.pixels.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Screen::mouse_callback(GLFWwindow *window, int button, int action, int mods)
{
}

void Screen::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_H && action == GLFW_PRESS)
        print_help();

    // Change material
    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        is_plastic = !is_plastic;
        if (is_plastic)
        {
            this->diffuse_color = {0.8f, 0.8f, 0.8f};
            this->specular_color = {0.1f, 0.1f, 0.1f};
            this->shininess = 32.0f;
            std::cout << "is_plastic\n";
        }
        else
        {
            this->diffuse_color = {1, 1, 1};
            this->specular_color = {1.0f, 1.0f, 1.0f};
            this->shininess = 128.0f;
            std::cout << "is_metalic\n";
        }
    }

    // Move light source
    if (key == GLFW_KEY_L)
    {
        static float angle = 2.3f; // Initial angle
        float radius = 2.0f;
        float x = radius * sin(angle);
        float z = radius * cos(angle);

        light_direction.x = x; // Update X position
        light_direction.z = z; // Update Z position
        light_direction = glm::normalize(light_direction);

        angle += 0.1f; // Increment the angle by a desired amount
    }
    // Toggle phong shading with gouraud
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        this->use_phong_shading = !this->use_phong_shading;
    }

    // Remove shadings one by one and set default at the end.
    if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
        remove_shading_index++;
        remove_shading_index = remove_shading_index % 4;
        remove_shading();
    }

    // change texture
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        is_shading = false;
        apply_texture(textures[(texture_id % textures.size())]);
        texture_id += 1;
    }

    // Add additional power
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        current_shape->xSpeed = (rand() % 10 > 5 ? 0.05 : -0.05);
        current_shape->ySpeed = (rand() % 10 > 5 ? 5 : -5);
    }

    // Change polygon mode
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        is_shading = false;
        glPolygonMode(GL_FRONT_AND_BACK, (mode = (mode == GL_LINE) ? GL_FILL : GL_LINE));
    }

    // restart
    if (key == GLFW_KEY_I && action == GLFW_PRESS)
    {
        remove_shading_index = -1;
        is_shading = true;
        remove_shading();
        if (current_shape->shape_type == ShapeType::CUBE)
            draw(new Cube{1.0f});
        else
            draw(new Sphere{1.0f});
    }
    // ZOOM OUT
    if (key == GLFW_KEY_Z)
    {
        zoom_factor += 1.0f;
        float adjustment = 1.0f / (zoom_factor + 1.0f);

        this->leftEdge += -adjustment;
        this->rightEdge += adjustment;
        this->topEdge += adjustment;
        this->bottomEdge += -adjustment;
        camera_distance -= adjustment;
    }

    // ZOOM IN
    if (key == GLFW_KEY_W)
    {
        zoom_factor -= 1.0f;
        float adjustment = 1.0f / (zoom_factor - 1.0f);

        this->leftEdge += adjustment;
        this->rightEdge += -adjustment;
        this->topEdge += -adjustment;
        this->bottomEdge += adjustment;
        camera_distance += adjustment;
    }
}

void Screen::print_help()
{
    std::cout << "--------------------------------------------------------" << std::endl
              << "i -- initialize the pose (top left corner of the window)" << std::endl
              << "t -- for wireframe mode" << std::endl
              << "c -- switch between textures." << std::endl
              << "f -- add random force" << std::endl
              << "h -- help; print explanation of your input control (simply to the command line)" << std::endl
              << "q -- quit (exit) the program" << std::endl;
}

// ## GENERIC STATIC CALLBACK METHODS THAT REDIRECT REQUEST TO OBJECTS CALLBACK METHODS
void Screen::mouse_callback_mediator(GLFWwindow *window, int button, int action, int mods)
{
    Screen *instance = reinterpret_cast<Screen *>(glfwGetWindowUserPointer(glfwGetCurrentContext()));
    instance->mouse_callback(window, button, action, mods);
}

void Screen::key_callback_mediator(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    Screen *instance = reinterpret_cast<Screen *>(glfwGetWindowUserPointer(glfwGetCurrentContext()));
    instance->key_callback(window, key, scancode, action, mods);
}
// ## END

void Screen::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
