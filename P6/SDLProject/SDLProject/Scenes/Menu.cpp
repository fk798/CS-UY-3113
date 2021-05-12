#include "Menu.h"

void Menu::Initialize(Entity *player) {
    state.nextScene = -1;
    state.player = player;
}
void Menu::Update(float deltaTime) {
    if (deltaTime == -1) {
        state.nextScene = 1;
    }
}
void Menu::Render(ShaderProgram *program) {
    Util::DrawText(program, Util::LoadTexture("font1.png"), "Escapist", 1, -0.5, glm::vec3(0.75f, -2.0f, 0));
    Util::DrawText(program, Util::LoadTexture("font1.png"), "By Faisal Karim", 1, -0.5, glm::vec3(1.0f, -4.0f, 0));
    Util::DrawText(program, Util::LoadTexture("font1.png"), "Press Enter", 1, -0.5, glm::vec3(2.25f, -5.0f, 0));
    Util::DrawText(program, Util::LoadTexture("font1.png"), "to Start", 1, -0.5, glm::vec3(3.25f, -6.0f, 0));
}

