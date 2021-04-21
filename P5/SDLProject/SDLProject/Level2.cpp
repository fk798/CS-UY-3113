#include "Level2.h"
#define LEVEL2_WIDTH 14
#define LEVEL2_HEIGHT 8

#define LEVEL2_ENEMY_COUNT 1

using namespace std;

unsigned int level2_data[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
    3, 1, 1, 1, 1, 1, 1, 0, 0, 3, 3, 3, 3, 3,
    3, 2, 2, 2, 2, 2, 2, 0, 0, 3, 3, 3, 3, 3
};

void Level2::Initialize(int numLives) {
    
    state.nextScene = -1;
    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 4, 1);
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, 0, 0);
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
    
    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");
    
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(4, -2.25, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].jumpPower = 5.0f;
    state.enemies[0].acceleration = glm::vec3(0, -9.81, 0);
    
    /*state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position = glm::vec3(1.0f, 0.0f, 0);
    state.enemies[1].speed = 1;
    state.enemies[1].aiType = JUMPER;
    state.enemies[1].aiState = JUMPING;
    state.enemies[1].jumpPower = 5.0f;
    state.enemies[1].acceleration = glm::vec3(0, -9.81, 0);
    
    state.enemies[2].entityType = ENEMY;
    state.enemies[2].textureID = enemyTextureID;
    state.enemies[2].position = glm::vec3(-3.0f, 0.0f, 0);
    state.enemies[2].speed = 1;
    state.enemies[2].aiType = WALKER;
    state.enemies[2].aiState = WALKING;
    state.enemies[2].jumpPower = 5.0f;
    state.enemies[2].acceleration = glm::vec3(0, -9.81, 0);
    */
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; ++i) {
        state.enemies[i].isActive = false;
    }
}
void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);
    if (state.player->numLives == 0) {
        state.nextScene = 4;
    }
    if (state.player->position.x >= 12) {
        state.nextScene = 3;
    }
    
    if (state.player->position.y <= -10) {
        state.player->numLives -= 1;
        cout << state.player->numLives << endl;
        state.player->position = glm::vec3(5, 0, 0);
    }
}
void Level2::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
}
