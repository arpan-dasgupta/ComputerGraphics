

#include <glad/glad.h>
#include <GLFW/glfw3.h>
// #include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
// #include "decagonalPrism.h"
#include "dodecahedron.h"
// #include "greatDodecahedron.h"

#include <iostream>
#include <bits/stdc++.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const float radius = 3.0;
glm::vec3 origin      = glm::vec3(0.0f, 0.0f,  0.0f);
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
glm::vec3 modelOffset       = glm::vec3(0.0f, 0.0f,  0.0f);
glm::vec3 modelPosition     = glm::vec3(0.0f, 0.0f,  0.0f);
float RotationOffset    = 0;
float revolutionOffset    = 0;
bool flag = 0, flag2 = 0;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("../source/demo.vs", "../source/demo.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    ourShader.use();

    glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 projection    = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);
    ourShader.setMat4("model", model);

    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // activate shader
        ourShader.use();

        if(revolutionOffset!=0)
        {
            // radius = sqrt(pow(cameraPos[0],2)+pow(cameraPos[1],2)+pow(cameraPos[2],2));
            float camX = cameraPos[0];
            float camZ = cameraPos[2];
            cameraPos[0] = camX*cos(revolutionOffset) - camZ*sin(revolutionOffset);
            cameraPos[2] = camX*sin(revolutionOffset) + camZ*cos(revolutionOffset);

            camX = cameraFront[0];
            camZ = cameraFront[2];
            cameraFront[0] = camX*cos(revolutionOffset) - camZ*sin(revolutionOffset);
            cameraFront[2] = camX*sin(revolutionOffset) + camZ*cos(revolutionOffset);

            revolutionOffset=0;
        }
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        if(flag2==1)
        {
            view = glm::lookAt(cameraPos, origin, cameraUp);
            flag2 = 0;
        }
        ourShader.setMat4("view", view);

        if(flag==1)
        {
            // std::cout<<"ye ";
            glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            transform = glm::translate(model,modelOffset);
            model = glm::rotate(transform, RotationOffset, glm::vec3(0.0f, 0.0f, 1.0f));
            // unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
            // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            ourShader.setMat4("model",model);
            modelOffset = glm::vec3(0.0f, 0.0f, 0.0f);
            RotationOffset=0;
            flag = 0;
        }

        // render box
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, triangleCount * 3);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {cameraPos += cameraSpeed * cameraFront;revolutionOffset=0;}
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {cameraPos -= cameraSpeed * cameraFront;revolutionOffset=0;}
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;revolutionOffset=0;}
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;revolutionOffset=0;}
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {cameraPos += cameraUp * cameraSpeed;revolutionOffset=0;}
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {cameraPos -= cameraUp * cameraSpeed;revolutionOffset=0;}

    const float objectSpeed = 0.01f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
        modelOffset += glm::vec3(1.0f, 0.0f, 0.0f) * objectSpeed;
        flag=1;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
        modelOffset -= glm::vec3(1.0f, 0.0f, 0.0f) * objectSpeed;
        flag=1;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){
        modelOffset += glm::vec3(0.0f, 1.0f, 0.0f) * objectSpeed;
        flag=1;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
        modelOffset -= glm::vec3(0.0f, 1.0f, 0.0f) * objectSpeed;
        flag=1;
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS){
        modelOffset += glm::vec3(0.0f, 0.0f, 1.0f) * objectSpeed;
        flag=1;
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS){
        modelOffset -= glm::vec3(0.0f, 0.0f, 1.0f) * objectSpeed;
        flag=1;
    }

    const float rotationSpeed = 0.01f;
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS){
        RotationOffset += rotationSpeed;
        flag=1;
    }

    const float revolutionSpeed = 0.15f;
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
        revolutionOffset += revolutionSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
        cameraPos = glm::vec3(3.0f, 0.0f,  0.0f);
        cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
        cameraFront = origin - cameraPos;
        flag2=1;
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS){
        cameraPos = glm::vec3(0.0f, 3.0f,  0.0f);
        cameraUp = glm::vec3(0.0f, 0.0f,  1.0f);
        cameraFront = origin - cameraPos;
        flag2=1;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
        cameraPos = glm::vec3(1.0f, 1.0f,  1.0f);
        cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
        cameraFront = origin - cameraPos;
        flag2=1;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        cameraPos = glm::vec3(0.0f, 0.0f,  3.0f);
        cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
        cameraFront = origin - cameraPos;
        revolutionOffset = 0;
        flag2=1;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

