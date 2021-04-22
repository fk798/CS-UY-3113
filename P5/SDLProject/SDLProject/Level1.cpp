#include "Level1.h"
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8

#define LEVEL1_ENEMY_COUNT 2
using namespace std;

unsigned int level1_data[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    3, 1, 1, 1, 1, 1, 0, 0, 1, 2, 2, 2, 2, 2,
    3, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2
};

void Level1::Initialize(int numLives) {
    
    state.nextScene = -1;
    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1, -2, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 3.0f;
    state.player->textureID = Util::LoadTexture("george_0.png");
    
    state.player->animRight = new int[4] {3, 7, 11, 15};
    state.player->animLeft = new int[4] {1, 5, 9, 13};
    state.player->animUp = new int[4] {2, 6, 10, 14};
    state.player->animDown = new int[4] {0, 4, 8, 12};
    
    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;	
    state.player->animCols = 4;
    state.player->animRows = 4;
    
    state.player->numLives = numLives;
    
    state.player->height = 0.8f;
    state.player->width = 0.8f;
    
    state.player->jumpPower = 7.0f;
    
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");
    
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(5, -5.0f, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].jumpPower = 5.0f;
    state.enemies[0].acceleration = glm::vec3(0, -9.81, 0);
        
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position = glm::vec3(11, 0.0f, 0);
    state.enemies[1].speed = 1;
    state.enemies[1].aiType = WAITANDGO;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].jumpPower = 5.0f;
    state.enemies[1].acceleration = glm::vec3(0, -9.81, 0);
    
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; ++i) {
        state.enemies[i].isActive = true;
    }
}
void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; ++i) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    }
    if (state.player->numLives == 0) {
        state.nextScene = 4;
    }
    bool anyAlive = false;
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; ++i) {
        if (state.enemies[i].isActive == true) {
            anyAlive = true;
            break;
        }
    }
    if (state.player->position.x >= 12 && state.player->position.y <= -3) {
        state.nextScene = 2;
    }
    if (state.player->position.y <= -10 || state.player->isActive == false) {
        state.player->isActive = true;
        state.player->numLives -= 1;
        state.player->position = glm::vec3(1, 0, 0);
    }
}
void Level1::Render(ShaderProgram *program) {
    Util::DrawText(program, Util::LoadTexture("font1.png"), "Level 1", 1.0f, -0.1f, glm::vec3(2, -2, 0));
    Util::DrawText(program, Util::LoadTexture("font1.png"), "Lives " + std::to_string(state.player->numLives), 1.0f, -0.1f, glm::vec3(2, -3, 0));
    Util::DrawText(program, Util::LoadTexture("font1.png"), "Portal->", 1.0f, -0.1f, glm::vec3(6, -4, 0));
    state.map->Render(program);
    state.player->Render(program);
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; ++i) {
        state.enemies[i].Render(program);
    }
}
