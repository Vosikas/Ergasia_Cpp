#include "global_state.h"
#include "graphics.h"
#include "enemy.h"
#include "tower.h"
#include <string>
#include <cmath>
#include <algorithm>
#include <cstdlib> 
#include <ctime>   

GlobalState* GlobalState::m_instance = nullptr;

GlobalState* GlobalState::getInstance() {
    if (!m_instance)
        m_instance = new GlobalState();
    return m_instance;
}

void GlobalState::init() {
    srand(time(nullptr));

    graphics::setFont("assets/orange juice 2.0.ttf");

    // --- ΔΗΜΙΟΥΡΓΙΑ ΧΑΡΤΗ ---
    MapNode* n6 = new MapNode{ 1450, 750, nullptr };
    MapNode* n5 = new MapNode{ 1450, 200, n6 };
    MapNode* n4 = new MapNode{ 900, 200, n5 };
    MapNode* n3 = new MapNode{ 900, 700, n4 };
    MapNode* n2 = new MapNode{ 300, 700, n3 };
    MapNode* n1 = new MapNode{ 100, 200, n2 };

    m_path_start = n1;
}

void GlobalState::reloadGame() {
    for (auto* obj : m_active_objects) {
        delete obj;
    }
    m_active_objects.clear();

    m_player_hp = m_max_player_hp;
    money = 150.0f;
    game_duration = 0.0f;
    m_spawn_timer = 0.0f;
}

bool GlobalState::isPointOnPath(float x, float y, float radius) {
    MapNode* current = m_path_start;
    while (current != nullptr && current->next != nullptr) {
        float x1 = current->x;
        float y1 = current->y;
        float x2 = current->next->x;
        float y2 = current->next->y;
        float dx = x2 - x1;
        float dy = y2 - y1;
        float t = ((x - x1) * dx + (y - y1) * dy) / (dx * dx + dy * dy);
        t = std::max(0.0f, std::min(1.0f, t));
        float closestX = x1 + t * dx;
        float closestY = y1 + t * dy;
        float distX = x - closestX;
        float distY = y - closestY;
        if (std::sqrt(distX * distX + distY * distY) < radius) return true;
        current = current->next;
    }
    return false;
}

void GlobalState::update(float dt) {
    
    if (m_player_hp <= 0.0f) {
        if (graphics::getKeyState(graphics::SCANCODE_R)) reloadGame();
        if (graphics::getKeyState(graphics::SCANCODE_ESCAPE)) graphics::stopMessageLoop();
        return;
    }

    
    game_duration += dt;
    money += 1.0f * dt;
    m_spawn_timer += dt;

    if (m_spawn_timer > 1.8f) {
        if (m_path_start) {
            int chance = rand() % 100;
            Enemy* enemy = nullptr;
            if (chance < 50) enemy = new TroopEnemy(m_path_start->x, m_path_start->y);
            else if (chance < 80) enemy = new RangeEnemy(m_path_start->x, m_path_start->y);
            else enemy = new TankEnemy(m_path_start->x, m_path_start->y);

            if (enemy != nullptr) addGameObject(enemy);
        }
        m_spawn_timer = 0.0f;
    }

    if (m_error_timer > 0.0f) m_error_timer -= dt;

    for (auto* obj : m_active_objects) if (obj->isActive()) obj->update(dt);

    auto it = m_active_objects.begin();
    while (it != m_active_objects.end()) {
        if (!(*it)->isActive()) {
            delete* it;
            it = m_active_objects.erase(it);
        }
        else ++it;
    }

    graphics::MouseState ms;
    graphics::getMouseState(ms);
    float mx = graphics::windowToCanvasX(ms.cur_pos_x);
    float my = graphics::windowToCanvasY(ms.cur_pos_y);

    if (ms.button_left_pressed && !m_mouse_down) {
        if (isPointOnPath(mx, my, 40.0f)) {
            m_error_msg = "You can't build here!";
            m_error_timer = 2.0f;
        }
        else if (getMoney() < T_price) {
            m_error_msg = "Not enough money!";
            m_error_timer = 2.0f;
        }
        else {
            Tower* t = new Tower(mx, my);
            addGameObject(t);
            money -= T_price;
        }
    }
    m_mouse_down = ms.button_left_pressed;
}

void GlobalState::draw() {
    graphics::Brush br;

    br.outline_opacity = 0.0f;
    br.texture = "assets/background.png";
    br.fill_color[0] = 1.0f; br.fill_color[1] = 1.0f; br.fill_color[2] = 1.0f;

    graphics::drawRect(800, 450, 1605, 905, br);

    br.texture = "";

    br.fill_opacity = 0.0f;
    br.outline_opacity = 0.3f;
    br.fill_color[0] = 1.0f; br.fill_color[1] = 0.0f; br.fill_color[2] = 0.0f;

    MapNode* current = m_path_start;
    while (current != nullptr) {
        if (current->next == nullptr) {
            graphics::Brush kingBr;
            kingBr.texture = "assets/kingtower.png";
            kingBr.outline_opacity = 0.0f;
            graphics::drawRect(current->x, current->y, 100, 100, kingBr);

            kingBr.texture = "";
            float hp_pct = m_player_hp / m_max_player_hp;
            if (hp_pct < 0) hp_pct = 0;

            kingBr.fill_color[0] = 0.5f; kingBr.fill_color[1] = 0.0f; kingBr.fill_color[2] = 0.0f;
            graphics::drawRect(current->x, current->y - 60, 80, 10, kingBr);

            kingBr.fill_color[0] = 0.0f; kingBr.fill_color[1] = 0.8f; kingBr.fill_color[2] = 0.0f;
            graphics::drawRect(current->x, current->y - 60, 80 * hp_pct, 10, kingBr);
        }
        else {
            graphics::drawDisk(current->x, current->y, 10, br);
            graphics::drawLine(current->x, current->y, current->next->x, current->next->y, br);
        }
        current = current->next;
    }

    
    for (auto* obj : m_active_objects) if (obj->isActive()) obj->draw();

   
    br.fill_opacity = 0.6f;
    br.outline_opacity = 0.0f;
    br.texture = "";
    br.fill_color[0] = 0.1f; br.fill_color[1] = 0.1f; br.fill_color[2] = 0.1f;

   
    graphics::drawRect(800, 860, 1600, 80, br);

    br.fill_opacity = 1.0f;

    br.fill_color[0] = 1.0f; br.fill_color[1] = 0.8f; br.fill_color[2] = 0.0f;
    std::string money_str = "Money: " + std::to_string((int)money) + "$";
    graphics::drawText(50, 870, 35, money_str, br);

    int min = (int)game_duration / 60;
    int sec = (int)game_duration % 60;
    std::string time_str = std::to_string(min) + " min " + (sec < 10 ? "0" : "") + std::to_string(sec) + " sec";

    br.fill_color[0] = 1.0f; br.fill_color[1] = 1.0f; br.fill_color[2] = 1.0f;
    graphics::drawText(1250, 870, 35, "Time: " + time_str, br);

    if (m_error_timer > 0.0f) {
        br.fill_color[0] = 1.0f; br.fill_color[1] = 0.2f; br.fill_color[2] = 0.2f;
        graphics::drawText(600, 100, 50, m_error_msg, br);
    }

   
    if (m_player_hp <= 0.0f) {
        br.texture = "";
        br.outline_opacity = 0.0f;
        br.fill_color[0] = 0.0f; br.fill_color[1] = 0.0f; br.fill_color[2] = 0.0f;
        br.fill_opacity = 0.8f;
        graphics::drawRect(800, 450, 1600, 900, br);

        br.fill_opacity = 1.0f;

        br.fill_color[0] = 0.5f; br.fill_color[1] = 1.0f; br.fill_color[2] = 1.0f;
        graphics::drawText(550, 350, 60, "You Survived: " + time_str, br);

        br.fill_color[0] = 1.0f; br.fill_color[1] = 0.2f; br.fill_color[2] = 0.2f;
        graphics::drawText(500, 480, 120, "GAME OVER", br);

        br.fill_color[0] = 1.0f; br.fill_color[1] = 1.0f; br.fill_color[2] = 0.0f;
        graphics::drawText(1100, 800, 40, "Try again {R}", br);

        br.fill_color[0] = 1.0f; br.fill_color[1] = 1.0f; br.fill_color[2] = 1.0f;
        graphics::drawText(100, 800, 40, "Exit {ESC}", br);
    }
}