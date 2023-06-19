#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <iostream>
#include "stb.h"

#include "Shader.h"
#include "DefaultMeshes.h"

const int wWIDTH = 800;
const int wHEIGHT = 600;

float lastX = wWIDTH / 2;
float lastY = wHEIGHT / 2;
float pitch = 0.0f;
float yaw = -90.0f;
float fov = 45.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

class Camera
{
public:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    float cameraSpeed;

    Camera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp)
    {
        this->cameraPos = cameraPos;
        this->cameraFront = cameraFront;
        this->cameraUp = cameraUp;
        this->cameraSpeed = 2.5f;
    }

    void processInput(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Keyboard Inputs
        float frameCameraSpeed = cameraSpeed * deltaTime;

        // Controlling Speed
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            frameCameraSpeed *= 2;

        // WASD
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += frameCameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= frameCameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * frameCameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * frameCameraSpeed;

        // Up and Down
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            cameraPos += frameCameraSpeed * cameraUp;
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            cameraPos -= frameCameraSpeed * cameraUp;

        // Mouse Inputs
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
    }
};

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    const float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    pitch = glm::clamp(pitch, -90.0f, 90.0f);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    fov -= (float)yOffset;
    fov = glm::clamp(fov, 1.0f, 45.0f);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int run()
{
    stbi_set_flip_vertically_on_load(true);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 0.0f, -2.0f),
        glm::vec3(1.2f, 1.0f, 2.0f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };


    unsigned int boxElements[] = {
        1, 3, 2,
        0, 1, 2
    };

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(wWIDTH, wHEIGHT, "learn-opengl", NULL, NULL);
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

    Shader objectShader(
        "./shaders/example.vs",
        "./shaders/example.fs"
    );

    Shader lightShader(
        "./shaders/lightExample.vs",
        "./shaders/lightExample.fs"
    );

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(DefaultMeshes::normalCube), DefaultMeshes::normalCube, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    unsigned int lightVBO;
    glGenBuffers(1, &lightVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(DefaultMeshes::cubeLight), DefaultMeshes::cubeLight, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    Camera cam = Camera(cameraPos, cameraFront, cameraUp);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glEnable(GL_DEPTH_TEST);
    unsigned int frames = 0;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        cam.processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //std::cout << frames << "  " << deltaTime << std::endl;

        glm::mat4 view;
        view = glm::lookAt(cam.cameraPos, cam.cameraPos + cam.cameraFront, cam.cameraUp);

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(fov), (float) wWIDTH / (float) wHEIGHT, 0.1f, 100.0f);

        objectShader.use();
        objectShader.setFloat3("objectColor", 1.0f, 0.5f, 0.31f);
        objectShader.setFloat3("lightColor", 1.0f, 1.0f, 1.0f);
        objectShader.setVec3("lightPos", cubePositions[2]);

        glm::vec3 lightColor;
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);

        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

        // Material
        objectShader.setFloat3("material.ambient", 1.0f, 0.5f, 0.31f);
        objectShader.setFloat3("material.diffuse", 1.0f, 0.5f, 0.31f);
        objectShader.setFloat3("material.specular", 0.5f, 0.5f, 0.5f);
        objectShader.setFloat1("material.shininess", 32.0f);

        // Light
        objectShader.setVec3("light.ambient", ambientColor);
        objectShader.setVec3("light.diffuse", diffuseColor);
        objectShader.setFloat3("light.specular", 1.0f, 1.0f, 1.0f);

        objectShader.setMat4("view", view);
        objectShader.setMat4("projection", projection);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[0]);
        //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        objectShader.setMat4("model", model);
        objectShader.setVec3("viewPos", cam.cameraPos);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightShader.use();

        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[2]);
        model = glm::scale(model, glm::vec3(0.2f));
        lightShader.setMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
        frames++;
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &lightVBO);
    //glDeleteBuffers(1, &EBO);
    glfwTerminate();

    return 0;
}