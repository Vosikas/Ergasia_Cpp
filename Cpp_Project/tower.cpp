#include "tower.h"
#include <cmath>
#include "enemy.h"
#include "global_state.h"

Tower::Tower(float x, float y) : GameObject(x, y, 40, 40) {
    init();
}

void Tower::init() {
    m_life = GlobalState::getInstance()->T_lifespan;
}

void Tower::draw() {
    graphics::Brush br;

    br.texture = ""; 

    br.fill_opacity = 0.0f;

    br.outline_opacity = 1.0f;

    br.fill_color[0] = 0.5f;
    br.fill_color[1] = 1.0f;
     

    graphics::drawDisk(m_pos_x, m_pos_y, m_range, br);

    br.fill_opacity = 1.0f;
    br.texture = "assets/defense.png";

    graphics::drawRect(m_pos_x, m_pos_y, 100, 100, br);
}

void Tower::update(float dt) {
    m_time_since_last_shot += dt;

    m_life -= dt;
    if (m_life <= 0.0f) {
        m_active = false;
    }

    auto& objects_list = GlobalState::getInstance()->getActiveObjects();
    for (auto* obj : objects_list) {
        Enemy* enemy = dynamic_cast<Enemy*>(obj);
        if (enemy != nullptr && enemy->isActive()) {
            float dx = enemy->getX() - m_pos_x;
            float dy = enemy->getY() - m_pos_y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance <= m_range && m_time_since_last_shot > m_cooldown) {
                enemy->updateHP(m_damage);
                m_time_since_last_shot = 0.0f;
                break;
            }
        }
    }
}