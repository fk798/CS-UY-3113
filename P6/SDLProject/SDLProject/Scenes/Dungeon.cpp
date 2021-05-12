#include "Dungeon.h"
#define DUNGEON_WIDTH 10
#define DUNGEON_HEIGHT 10

#define DUNGEON_ENEMY_COUNT 1
using namespace std;

unsigned int dungeon_data[] =
{
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 3, 3, 3, 0, 0, 3, 3, 3, 3
};

void Dungeon::Initialize(Entity *player) {
    
    state.nextScene = -1;
    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(DUNGEON_WIDTH, DUNGEON_HEIGHT, dungeon_data, mapTextureID, 1.0f, 4, 1);
    // Initialize Player
    state.player = player;
    
    state.enemies = new Entity[DUNGEON_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");
    
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(5, -3.0f, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    //state.enemies[0].jumpPower = 5.0f;
    //state.enemies[0].acceleration = glm::vec3(0, 0, 0);
        
    /*state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position = glm::vec3(11, 0.0f, 0);
    state.enemies[1].speed = 1;
    state.enemies[1].aiType = WAITANDGO;
    state.enemies[1].aiState = IDLE;
    //state.enemies[1].jumpPower = 5.0f;
    state.enemies[1].acceleration = glm::vec3(0, 0, 0);*/
    
    for (int i = 0; i < DUNGEON_ENEMY_COUNT; ++i) {
        state.enemies[i].isActive = true;
    }
}
void Dungeon::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, DUNGEON_ENEMY_COUNT, state.map);
    for (int i = 0; i < DUNGEON_ENEMY_COUNT; ++i) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, DUNGEON_ENEMY_COUNT, state.map);
    }
    if (state.player->numLives == 0) {
        state.nextScene = 4;
    }
    bool anyAlive = false;
    for (int i = 0; i < DUNGEON_ENEMY_COUNT; ++i) {
        if (state.enemies[i].isActive == true) {
            anyAlive = true;
            break;
        }
    }
    cout << "X: " << state.player->position.x << endl;
    cout << "Y: " << state.player->position.y << endl;
    // exiting dungeon
    if ((state.player->position.x >= 4 && state.player->position.x <= 7) && (state.player->position.y <= -8 && state.player->position.y >= -10)) {
        state.nextScene = 2;
        state.player->position.x = state.player->newPosition.x;
        state.player->position.y = state.player->newPosition.y;
    }
    /*if (state.player->position.x >= 12 && state.player->position.y <= -3) {
        state.nextScene = 2;
    }
    if (state.player->position.y <= -10 || state.player->isActive == false) {
        state.player->isActive = true;
        state.player->numLives -= 1;
        state.player->position = glm::vec3(1, 0, 0);
    }*/
}
void Dungeon::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    for (int i = 0; i < DUNGEON_ENEMY_COUNT; ++i) {
        state.enemies[i].Render(program);
    }
}
