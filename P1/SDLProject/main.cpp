#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, shipMatrix, asteroidMatrix, projectionMatrix;

// scale variables for shipMatrix
float xCoord;
float yCoord;
bool moveRight;

// rotate variable for asteroidMatrix
float rotateAsteroid;

// for deltaTime
float lastTicks = 0.0f;

//textures
GLuint shipTextureID;
GLuint asteroidTextureID;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);
    return textureID;
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Project 1!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    shipMatrix = glm::mat4(1.0f);
    asteroidMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    xCoord = 0.0f;
    yCoord = 0.0f;
    moveRight = true;
    
    rotateAsteroid = 0.0f;

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    glEnable(GL_BLEND);
    // Good setting for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shipTextureID = LoadTexture("playerShip3_red.png");
    asteroidTextureID = LoadTexture("meteorGrey_big4.png");
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}

void Update() { 
    //modelMatrix = glm::mat4(1.0f);
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    // change direction of ship
    if (xCoord >= 5.0f) {
        moveRight = false;
    }
    else if (xCoord <= -5.0f) {
        moveRight = true;
    }

    if (moveRight == true) {
        xCoord += 2.0f*deltaTime;
    }
    else {
        xCoord -= 2.0f*deltaTime;
    }
    
    rotateAsteroid += 90.0f * deltaTime;
    
    shipMatrix = glm::mat4(1.0f);
    shipMatrix = glm::translate(shipMatrix, glm::vec3(xCoord, -2.0f, 0.0f)); // translate

    asteroidMatrix = glm::mat4(1.0f);
    asteroidMatrix = glm::rotate(asteroidMatrix, glm::radians(rotateAsteroid), glm::vec3(0.0f, 0.0f, 1.0f)); // rotate
    //modelMatrix = glm::scale(modelMatrix1, glm::vec3(1.01f, 1.01f, 1.0f)); // scaling
}

void DrawShip() {
    program.SetModelMatrix(shipMatrix);
    glBindTexture(GL_TEXTURE_2D, shipTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DrawAsteroid() {
    program.SetModelMatrix(asteroidMatrix);
    glBindTexture(GL_TEXTURE_2D, asteroidTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    //program.SetModelMatrix(modelMatrix);
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    DrawShip();
    DrawAsteroid();
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

    SDL_GL_SwapWindow(displayWindow);
}

/*void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    //program.SetModelMatrix(modelMatrix1);
    //program.SetModelMatrix(modelMatrix2);
    float vertices1[] = { 0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5f };
    //float vertices2[] = { 0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5f };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices1);
    //glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
    glEnableVertexAttribArray(program.positionAttribute);
    //glDrawArrays(GL_TRIANGLES, 0, 3);

    DrawShip();
    DrawAsteroid();

    glDisableVertexAttribArray(program.positionAttribute);

    SDL_GL_SwapWindow(displayWindow);
}*/

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}