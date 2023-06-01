#pragma once
#include "stb.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Material.h"
#include "Object.h"
#include "Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Sprite (Will need to be class in final) NEED TO REFACTOR
/*
void spriteSize(int width, int height, int nWidth, int nHeight)
{

}

void spriteIndexToCoords(int index, int width, int height, int nWidth, int nHeight, int* x, int* y)
{
    int spriteWidth = (int)(width / nWidth);
    int spriteHeight = (int)(height / nHeight);
    *x = (index % nWidth) * (spriteWidth);
    *y = (index % nHeight) * (spriteHeight);
}

void coordsToTexCoords(int x, int y, int maxX, int maxY, float* fx, float* fy)
{
    *fx = (float)x / maxX;
    *fy = (float)y / maxY;
}

void setTexCoords(float* vertexArray, int x, int y, int sizeX, int sizeY, int width, int height, int vLen, int texIndex)
{
    float fx;
    float fy;
    coordsToTexCoords(x, y, width, height, &fx, &fy);
    // top right
    (vertexArray)[texIndex] = fx;
    (vertexArray)[texIndex + 1] = fy;

    // bottom right
    (vertexArray)[texIndex + vLen] = fx;
    (vertexArray)[texIndex + vLen + 1] = fy;
}

void setSpriteIndex(float* vertexArray, int index, int width, int height, int nWidth, int nHeight, int vLen, int texIndex)
{
    int x;
    int y;
    spriteIndexToCoords(index, width, height, nWidth, nHeight, &x, &y);
    setTexCoords(vertexArray, x, y, width, height, vLen, texIndex);
}

*/

int run()
{
    float triangleVertices[] = {
//     |Positions         |Colors           |TexCoords
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom left
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f  // top
    };

    float boxVertices[] = {
//     |Positions         |Colors           |TexCoords
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    };

    for (int i = 0; i < 32; i++)
    {
        std::cout << boxVertices[i] << " ";
    }
    std::cout << std::endl;

    unsigned int boxElements[] = {
        0, 1, 2,
        1, 3, 2
    };

    stbi_set_flip_vertically_on_load(true);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "learn-opengl", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader ourShader(
        "./shaders/example.vs",
        "./shaders/example.fs"
    );

    Texture* tex = new Texture("./images/example.png", {1.0f, 1.0f,
                                                        1.0f, 0.0f,
                                                        0.0f, 1.0f,
                                                        0.0f, 0.0f});

    Material* mat = new Material(tex, new Color(0.0, 0.0, 0.0, 1.0));

    std::vector<Point*> pointVector = { new Point(0.5f, 0.5f, 0.0f),
                                        new Point(0.5f, -0.5f, 0.0f),
                                        new Point(-0.5f, 0.5f, 0.0f),
                                        new Point(-0.5f, -0.5f, 0.0f) };

    unsigned int elements[] = { 0, 1, 2,
                         1, 3, 2 };

    Object* obj = new Object(pointVector, elements, mat);
    

    //setSpriteIndex(boxVertices, 6, width, height, 4, 8, 8, 6);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        ourShader.setFloat3("backgroundColor", 0.2f, 0.3f, 0.3f);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        tex->use();
        ourShader.use();
        glBindVertexArray(obj->VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //obj->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    

    glfwTerminate();

    return 0;
}