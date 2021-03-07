

#include <glad/glad.h>
#include <GLFW/glfw3.h>
// #include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
// #include "decagonalPrism.h"
// #include "dodecahedron.h"
#include "greatDodecahedron.h"

#include <iostream>
#include <bits/stdc++.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const float radius = 3.0;
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
glm::vec3 modelOffset       = glm::vec3(0.0f, 0.0f,  0.0f);
glm::vec3 modelPosition     = glm::vec3(0.0f, 0.0f,  0.0f);
float RotationOffset    = 0;
float CameraAngle    = 0;
bool flag = 0, flag2 = 0;

// float vertices[] = {
//         0.5f,  0.2f,   0.0f,   0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 
//         0.4045084971874737f,   0.2f,   0.29389262614623657f,   0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 
//         0.40450849718747367f,  0.2f,   -0.2938926261462367f,   0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 

//         0.4045084971874737f,   0.2f,   0.29389262614623657f,   0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 
//         0.15450849718747373f,  0.2f,   0.47552825814757677f,   0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 
//         0.40450849718747367f,  0.2f,   -0.2938926261462367f,   0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 

//         0.15450849718747373f,  0.2f,   0.47552825814757677f,   0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 
//         -0.15450849718747367f, 0.2f,   0.4755282581475768f,    0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 
//         0.40450849718747367f,  0.2f,   -0.2938926261462367f,   0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 

//         -0.15450849718747367f, 0.2f,   0.4755282581475768f,    0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 
//         -0.40450849718747367f, 0.2f,   0.2938926261462366f,    0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 
//         0.40450849718747367f,  0.2f,   -0.2938926261462367f,   0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 

//         -0.40450849718747367f, 0.2f,   0.2938926261462366f,    0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 
//         -0.5f, 0.2f,   0.0f, 0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 
//         0.40450849718747367f,  0.2f,   -0.2938926261462367f,   0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 

//         -0.5f, 0.2f,   0.0f, 0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 
//         -0.4045084971874738f,  0.2f,   -0.2938926261462365f,   0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 
//         0.40450849718747367f,  0.2f,   -0.2938926261462367f,   0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 

//         -0.4045084971874738f,  0.2f,   -0.2938926261462365f,   0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 
//         -0.15450849718747378f, 0.2f,   -0.47552825814757677f,  0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 
//         0.40450849718747367f,  0.2f,   -0.2938926261462367f,   0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 

//         -0.15450849718747378f, 0.2f,   -0.47552825814757677f,  0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 
//         0.15450849718747361f,  0.2f,   -0.4755282581475768f,   0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 
//         0.40450849718747367f,  0.2f,   -0.2938926261462367f,   0.23780013029915437f, 0.40850608106892494f, 0.6680582541017235f, 


//         0.5f,  -0.2f,  0.0f,   0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 
//         0.4045084971874737f,   -0.2f,  0.29389262614623657f,   0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 
//         0.40450849718747367f,  -0.2f,  -0.2938926261462367f,   0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 

//         0.4045084971874737f,   -0.2f,  0.29389262614623657f,   0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 
//         0.15450849718747373f,  -0.2f,  0.47552825814757677f,   0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 
//         0.40450849718747367f,  -0.2f,  -0.2938926261462367f,   0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 

//         0.15450849718747373f,  -0.2f,  0.47552825814757677f,   0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 
//         -0.15450849718747367f, -0.2f,  0.4755282581475768f,    0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 
//         0.40450849718747367f,  -0.2f,  -0.2938926261462367f,   0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 

//         -0.15450849718747367f, -0.2f,  0.4755282581475768f,    0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 
//         -0.40450849718747367f, -0.2f,  0.2938926261462366f,    0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 
//         0.40450849718747367f,  -0.2f,  -0.2938926261462367f,   0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 

//         -0.40450849718747367f, -0.2f,  0.2938926261462366f,    0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 
//         -0.5f, -0.2f,  0.0f, 0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 
//         0.40450849718747367f,  -0.2f,  -0.2938926261462367f,   0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 

//         -0.5f, -0.2f,  0.0f, 0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 
//         -0.4045084971874738f,  -0.2f,  -0.2938926261462365f,   0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 
//         0.40450849718747367f,  -0.2f,  -0.2938926261462367f,   0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 

//         -0.4045084971874738f,  -0.2f,  -0.2938926261462365f,   0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 
//         -0.15450849718747378f, -0.2f,  -0.47552825814757677f,  0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 
//         0.40450849718747367f,  -0.2f,  -0.2938926261462367f,   0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 

//         -0.15450849718747378f, -0.2f,  -0.47552825814757677f,  0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 
//         0.15450849718747361f,  -0.2f,  -0.4755282581475768f,   0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 
//         0.40450849718747367f,  -0.2f,  -0.2938926261462367f,   0.1321129154311057f, 0.8871231304581646f, 0.5042544742166691f, 


//         0.4045084971874737f,   0.2f,   0.29389262614623657f,   0.3958989635122119f, 0.8574571407528007f, 0.7991206983308492f, 
//         0.5f,  0.2f,   0.0f,   0.3958989635122119f, 0.8574571407528007f, 0.7991206983308492f, 
//         0.4045084971874737f,   -0.2f,  0.29389262614623657f,   0.3958989635122119f, 0.8574571407528007f, 0.7991206983308492f, 

//         0.5f,  0.2f,   0.0f,   0.3958989635122119f, 0.8574571407528007f, 0.7991206983308492f, 
//         0.5f,  -0.2f,  0.0f,   0.3958989635122119f, 0.8574571407528007f, 0.7991206983308492f, 
//         0.4045084971874737f,   -0.2f,  0.29389262614623657f,   0.3958989635122119f, 0.8574571407528007f, 0.7991206983308492f, 


//         0.15450849718747373f,  0.2f,   0.47552825814757677f,   0.2787746929887114f, 0.15951092545882373f, 0.5166875235945589f, 
//         0.4045084971874737f,   0.2f,   0.29389262614623657f,   0.2787746929887114f, 0.15951092545882373f, 0.5166875235945589f, 
//         0.15450849718747373f,  -0.2f,  0.47552825814757677f,   0.2787746929887114f, 0.15951092545882373f, 0.5166875235945589f, 

//         0.4045084971874737f,   0.2f,   0.29389262614623657f,   0.2787746929887114f, 0.15951092545882373f, 0.5166875235945589f, 
//         0.4045084971874737f,   -0.2f,  0.29389262614623657f,   0.2787746929887114f, 0.15951092545882373f, 0.5166875235945589f, 
//         0.15450849718747373f,  -0.2f,  0.47552825814757677f,   0.2787746929887114f, 0.15951092545882373f, 0.5166875235945589f, 


//         -0.15450849718747367f, 0.2f,   0.4755282581475768f,    0.5958578280990036f, 0.22753704567230615f, 0.013097098278626373f, 
//         0.15450849718747373f,  0.2f,   0.47552825814757677f,   0.5958578280990036f, 0.22753704567230615f, 0.013097098278626373f, 
//         -0.15450849718747367f, -0.2f,  0.4755282581475768f,    0.5958578280990036f, 0.22753704567230615f, 0.013097098278626373f, 

//         0.15450849718747373f,  0.2f,   0.47552825814757677f,   0.5958578280990036f, 0.22753704567230615f, 0.013097098278626373f, 
//         0.15450849718747373f,  -0.2f,  0.47552825814757677f,   0.5958578280990036f, 0.22753704567230615f, 0.013097098278626373f, 
//         -0.15450849718747367f, -0.2f,  0.4755282581475768f,    0.5958578280990036f, 0.22753704567230615f, 0.013097098278626373f, 


//         -0.40450849718747367f, 0.2f,   0.2938926261462366f,    0.7910654502735829f, 0.11395893262246348f, 0.28825986339929754f, 
//         -0.15450849718747367f, 0.2f,   0.4755282581475768f,    0.7910654502735829f, 0.11395893262246348f, 0.28825986339929754f, 
//         -0.40450849718747367f, -0.2f,  0.2938926261462366f,    0.7910654502735829f, 0.11395893262246348f, 0.28825986339929754f, 

//         -0.15450849718747367f, 0.2f,   0.4755282581475768f,    0.7910654502735829f, 0.11395893262246348f, 0.28825986339929754f, 
//         -0.15450849718747367f, -0.2f,  0.4755282581475768f,    0.7910654502735829f, 0.11395893262246348f, 0.28825986339929754f, 
//         -0.40450849718747367f, -0.2f,  0.2938926261462366f,    0.7910654502735829f, 0.11395893262246348f, 0.28825986339929754f, 


//         -0.5f, 0.2f,   0.0f, 0.1706585319444004f, 0.7793473300724979f, 0.2331129723318519f, 
//         -0.40450849718747367f, 0.2f,   0.2938926261462366f,    0.1706585319444004f, 0.7793473300724979f, 0.2331129723318519f, 
//         -0.5f, -0.2f,  0.0f, 0.1706585319444004f, 0.7793473300724979f, 0.2331129723318519f, 

//         -0.40450849718747367f, 0.2f,   0.2938926261462366f,    0.1706585319444004f, 0.7793473300724979f, 0.2331129723318519f, 
//         -0.40450849718747367f, -0.2f,  0.2938926261462366f,    0.1706585319444004f, 0.7793473300724979f, 0.2331129723318519f, 
//         -0.5f, -0.2f,  0.0f, 0.1706585319444004f, 0.7793473300724979f, 0.2331129723318519f, 


//         -0.5f, -0.2f,  0.0f, 0.39684446130296436f, 0.3472137192098623f, 0.29105631717847036f, 
//         -0.4045084971874738f,  -0.2f,  -0.2938926261462365f,   0.39684446130296436f, 0.3472137192098623f, 0.29105631717847036f, 
//         -0.5f, 0.2f,   0.0f, 0.39684446130296436f, 0.3472137192098623f, 0.29105631717847036f, 

//         -0.4045084971874738f,  -0.2f,  -0.2938926261462365f,   0.39684446130296436f, 0.3472137192098623f, 0.29105631717847036f, 
//         -0.4045084971874738f,  0.2f,   -0.2938926261462365f,   0.39684446130296436f, 0.3472137192098623f, 0.29105631717847036f, 
//         -0.5f, 0.2f,   0.0f, 0.39684446130296436f, 0.3472137192098623f, 0.29105631717847036f, 


//         -0.15450849718747378f, 0.2f,   -0.47552825814757677f,  0.25035502089756145f, 0.42430252327911644f, 0.6769179268313267f, 
//         -0.4045084971874738f,  0.2f,   -0.2938926261462365f,   0.25035502089756145f, 0.42430252327911644f, 0.6769179268313267f, 
//         -0.15450849718747378f, -0.2f,  -0.47552825814757677f,  0.25035502089756145f, 0.42430252327911644f, 0.6769179268313267f, 

//         -0.4045084971874738f,  0.2f,   -0.2938926261462365f,   0.25035502089756145f, 0.42430252327911644f, 0.6769179268313267f, 
//         -0.4045084971874738f,  -0.2f,  -0.2938926261462365f,   0.25035502089756145f, 0.42430252327911644f, 0.6769179268313267f, 
//         -0.15450849718747378f, -0.2f,  -0.47552825814757677f,  0.25035502089756145f, 0.42430252327911644f, 0.6769179268313267f, 


//         0.15450849718747361f,  0.2f,   -0.4755282581475768f,   0.1111487444162923f, 0.33987164484160404f, 0.06466975691073729f, 
//         -0.15450849718747378f, 0.2f,   -0.47552825814757677f,  0.1111487444162923f, 0.33987164484160404f, 0.06466975691073729f, 
//         0.15450849718747361f,  -0.2f,  -0.4755282581475768f,   0.1111487444162923f, 0.33987164484160404f, 0.06466975691073729f, 

//         -0.15450849718747378f, 0.2f,   -0.47552825814757677f,  0.1111487444162923f, 0.33987164484160404f, 0.06466975691073729f, 
//         -0.15450849718747378f, -0.2f,  -0.47552825814757677f,  0.1111487444162923f, 0.33987164484160404f, 0.06466975691073729f, 
//         0.15450849718747361f,  -0.2f,  -0.4755282581475768f,   0.1111487444162923f, 0.33987164484160404f, 0.06466975691073729f, 


//         0.40450849718747367f,  0.2f,   -0.2938926261462367f,   0.6294836095629364f, 0.38483184275774995f, 0.6892384615516608f, 
//         0.15450849718747361f,  0.2f,   -0.4755282581475768f,   0.6294836095629364f, 0.38483184275774995f, 0.6892384615516608f, 
//         0.40450849718747367f,  -0.2f,  -0.2938926261462367f,   0.6294836095629364f, 0.38483184275774995f, 0.6892384615516608f, 

//         0.15450849718747361f,  0.2f,   -0.4755282581475768f,   0.6294836095629364f, 0.38483184275774995f, 0.6892384615516608f, 
//         0.15450849718747361f,  -0.2f,  -0.4755282581475768f,   0.6294836095629364f, 0.38483184275774995f, 0.6892384615516608f, 
//         0.40450849718747367f,  -0.2f,  -0.2938926261462367f,   0.6294836095629364f, 0.38483184275774995f, 0.6892384615516608f, 


//         0.5f,  0.2f,   0.0f,   0.6704696680626454f, 0.5205436582623677f, 0.1633116725205188f, 
//         0.40450849718747367f,  0.2f,   -0.2938926261462367f,   0.6704696680626454f, 0.5205436582623677f, 0.1633116725205188f, 
//         0.5f,  -0.2f,  0.0f,   0.6704696680626454f, 0.5205436582623677f, 0.1633116725205188f, 

//         0.40450849718747367f,  0.2f,   -0.2938926261462367f,   0.6704696680626454f, 0.5205436582623677f, 0.1633116725205188f, 
//         0.40450849718747367f,  -0.2f,  -0.2938926261462367f,   0.6704696680626454f, 0.5205436582623677f, 0.1633116725205188f, 
//         0.5f,  -0.2f,  0.0f,   0.6704696680626454f, 0.5205436582623677f, 0.1633116725205188f,  
//     };

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


    // // texture coord attribute
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);


    // // load and create a texture 
    // // -------------------------
    // unsigned int texture1, texture2;
    // // texture 1
    // // ---------
    // glGenTextures(1, &texture1);
    // glBindTexture(GL_TEXTURE_2D, texture1);
    // // set the texture wrapping parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // // set texture filtering parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // // load image, create texture and generate mipmaps
    // int width, height, nrChannels;
    // stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // unsigned char *data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
    // if (data)
    // {
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    // }
    // else
    // {
    //     std::cout << "Failed to load texture" << std::endl;
    // }
    // stbi_image_free(data);
    // // texture 2
    // // ---------
    // glGenTextures(1, &texture2);
    // glBindTexture(GL_TEXTURE_2D, texture2);
    // // set the texture wrapping parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // // set texture filtering parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // // load image, create texture and generate mipmaps
    // data = stbi_load(FileSystem::getPath("resources/textures/awesomeface.png").c_str(), &width, &height, &nrChannels, 0);
    // if (data)
    // {
    //     // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    // }
    // else
    // {
    //     std::cout << "Failed to load texture" << std::endl;
    // }
    // stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    // ourShader.setInt("texture1", 0);
    // ourShader.setInt("texture2", 1);

    glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 projection    = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);
    ourShader.setMat4("model", model);

    // modelOffset = new ModelMatrix();
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // // bind textures on corresponding texture units
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture1);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, texture2);

        // activate shader
        ourShader.use();

        // create transformations
        // glm::mat4 view          = glm::mat4(1.0f);
        // glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        // glm::mat4 projection    = glm::mat4(1.0f);
        
        // model = glm::rotate(model, (float)0, glm::vec3(0.5f, 1.0f, 0.0f));
        
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        if(CameraAngle!=0)
        {
            float camX = radius * sin(CameraAngle);
            float camZ = radius * cos(CameraAngle);
            view = glm::lookAt(glm::vec3(camX, 0.0, camZ), modelPosition, cameraUp);
        }
        if(flag2==1)
        {
            view = glm::lookAt(cameraPos, modelPosition, cameraUp);
            flag2 = 0;
        }
        ourShader.setMat4("view", view);

        if(flag==1)
        {
            for(int iVertex = 0; iVertex < sizeof(vertices); iVertex += 6)
            {
                vertices[iVertex] += modelOffset[0];
                vertices[iVertex + 1] += modelOffset[1];
                vertices[iVertex + 2] += modelOffset[2];
                float x = vertices[iVertex];
                float y = vertices[iVertex+1];
                vertices[iVertex] = x*cos(RotationOffset) - y*sin(RotationOffset);
                vertices[iVertex+1] = x*sin(RotationOffset) + y*cos(RotationOffset);
            }
            // std::cout<<RotationOffset<<" ";
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            flag = 0;
            modelPosition += modelOffset;
            modelOffset = glm::vec3(0.0f, 0.0f, 0.0f);
            RotationOffset = 0;
        }
        // else if (flag==2)
        // {
        //     for(int iVertex = 0; iVertex < sizeof(vertices); iVertex += 6)
        //     {
        //         float x = vertices[iVertex];
        //         float y = vertices[iVertex+1];
        //         vertices[iVertex] = x*cos(RotationOffset) - y*sin(RotationOffset);
        //         vertices[iVertex+1] = x*sin(RotationOffset) + y*cos(RotationOffset);
        //         std::cout<<vertices[iVertex]<<" ";
        //     }
        //     glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        //     flag=0;
        //     RotationOffset = 0;
        // }
        // model = modelOffset;
        // ourShader.setMat4("model", model);

        // retrieve the matrix uniform locations
        // unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        // unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");
        // pass them to the shaders (3 different ways)
        // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

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
        {cameraPos += cameraSpeed * cameraFront;CameraAngle=0;}
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {cameraPos -= cameraSpeed * cameraFront;CameraAngle=0;}
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;CameraAngle=0;}
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;CameraAngle=0;}
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {cameraPos += cameraUp * cameraSpeed;CameraAngle=0;}
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {cameraPos -= cameraUp * cameraSpeed;CameraAngle=0;}

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

    const float rotationSpeed = 0.1f;
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS){
        RotationOffset += rotationSpeed;
        flag=1;
    }

    const float revolutionSpeed = 0.1f;
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
        CameraAngle += revolutionSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
        cameraPos = glm::vec3(3.0f, 0.0f,  0.0f);
        cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
        cameraFront = modelPosition - cameraPos;
        flag2=1;
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS){
        cameraPos = glm::vec3(0.0f, 3.0f,  0.0f);
        cameraUp = glm::vec3(0.0f, 0.0f,  1.0f);
        cameraFront = modelPosition - cameraPos;
        flag2=1;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
        cameraPos = glm::vec3(1.0f, 1.0f,  1.0f);
        cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
        cameraFront = modelPosition - cameraPos;
        flag2=1;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

