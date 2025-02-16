#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "include/stb/stb_image.h"

#include "Headers/shader.h"
#include "Headers/buffers.h"
#include "Headers/texture.h"

bool isFS;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_F11 && action == GLFW_RELEASE) {
        GLFWmonitor *mon = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(mon);
        int xpos, ypos;
        int wXpos, wYpos;
        wXpos = 640;
        wYpos = 480;
        glfwGetWindowPos(window, &xpos, &ypos);
        if(isFS == false) {
            glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), xpos, ypos, mode->width, mode->height, 0); 
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
        else {
            glfwSetWindowMonitor(window, NULL, xpos, ypos, wXpos, wYpos, 0);
        }
        isFS = !isFS;
    }
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(void)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLuint indexes[] {
        0, 1, 2,
        0, 2, 3, 
        4, 0, 1, 
        4, 1, 5, 
        5, 1, 2, 
        5, 2, 6, 
        6, 2, 3, 
        6, 3, 7, 
        7, 3, 0, 
        7, 0, 4, 
        4, 5, 6, 
        4, 6, 7
    };
    GLFWwindow* window;
    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    isFS = 0;
    GLfloat vertices[] = {
            // COORDS
        -0.5f,   0.0f ,  0.5f,      0.0f, 1.0f,         
        -0.5f,   0.0f , -0.5f,      0.0f, 0.0f,      
         0.5f,   0.0f , -0.5f,      1.0f, 0.0f,
         0.5f,   0.0f ,  0.5f,      1.0f, 1.0f,
        -0.5f,   0.5f ,  0.5f,      0.0f, 1.0f,
        -0.5f,   0.5f , -0.5f,      0.0f, 0.0f,
         0.5f,   0.5f , -0.5f,      1.0f, 1.0f,
         0.5f,   0.5f ,  0.5f,      0.0f, 1.0f
    };

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGL();

    Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag");

    VAO vao;
    VBO vbo(vertices, sizeof(vertices));
    EBO ebo(indexes, sizeof(indexes));

    vao.Bind();
    ebo.Bind();
    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    vao.LinkAttrib(vbo, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3*sizeof(float)));

    GLuint uniRef = glGetUniformLocation(shaderProgram.shaderId, "aspect");

    Texture texture("stone.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    texture.texUnit(shaderProgram, "tex0", 0);

    float rotation = 0.0f;
    double prevTime = glfwGetTime();

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    { 
        glClearColor(0.031, 0.208, 0.4, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float aspect = (float)width / (float)height;
        shaderProgram.Activate();
        glUniform1f(uniRef, aspect);
        texture.bind();
        double crntTime = glfwGetTime();
        if(crntTime - prevTime  >= 1/60) {
            rotation += 0.2f;
            prevTime = crntTime;
        }

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.1f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, -0.3f, -2.0f));
        proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

        int modelLoc = glGetUniformLocation(shaderProgram.shaderId, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(shaderProgram.shaderId, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projLoc = glGetUniformLocation(shaderProgram.shaderId, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        glUniform1f(uniRef, aspect);
        vao.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indexes)/sizeof(int), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    vao.Delete();
    vbo.Delete();
    ebo.Delete();
    shaderProgram.Delete();
    texture.del();
    glfwTerminate();
    return 0;
}