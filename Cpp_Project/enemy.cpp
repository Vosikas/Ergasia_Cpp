#include "enemy.h"
#include "global_state.h"
#include "graphics.h"
#include <cmath> 



Enemy::Enemy(float x, float y) : GameObject(x, y, 30, 30) {
    MapNode* start = GlobalState::getInstance()->getPathStart();
    if (start && start->next) {
        m_target_node = start->next;
    }
    init();
}

void Enemy::init() {
    m_hp = 100.0f;
    m_max_hp = 100.0f;
    m_speed = 100.0f;
    m_damage = 10.0f;
    m_reward = 10.0f;
}

void Enemy::update(float dt) {
    if (!m_target_node) return;

    float dx = m_target_node->x - m_pos_x;
    float dy = m_target_node->y - m_pos_y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance < 5.0f) {
        m_target_node = m_target_node->next;

        if (!m_target_node) {
            m_active = false;
            GlobalState::getInstance()->m_player_hp -= m_damage;
        }
    }
    else {
        m_pos_x += (dx / distance) * m_speed * dt;
        m_pos_y += (dy / distance) * m_speed * dt;
    }
}


void Enemy::draw() {
    graphics::Brush br;
    br.fill_color[0] = 1.0f; br.fill_color[1] = 0.0f; br.fill_color[2] = 0.0f;
    br.outline_opacity = 0.0f;
    graphics::drawDisk(m_pos_x, m_pos_y, 15, br);
}



TankEnemy::TankEnemy(float x, float y) : Enemy(x, y) {
    m_hp = 400.0f;     
    m_max_hp = 400.0f;
    m_speed = 30.0f;    
    m_damage = 50.0f;   
    m_reward = 60.0f;
}

void TankEnemy::draw() {
    graphics::Brush br;

    
    br.texture = "assets/tank.png";
    br.outline_opacity = 0.0f;
    br.fill_opacity = 1.0f;

   
    graphics::drawRect(m_pos_x, m_pos_y, 80, 80, br);

  
    br.texture = ""; 

    float hp_pct = m_hp / m_max_hp;
    if (hp_pct < 0) hp_pct = 0;

 
    br.fill_color[0] = 0.0f; br.fill_color[1] = 0.0f; br.fill_color[2] = 0.0f;
    graphics::drawRect(m_pos_x, m_pos_y - 40, 50, 8, br);

    // Green bar
    br.fill_color[0] = 0.0f; br.fill_color[1] = 1.0f; br.fill_color[2] = 0.0f;
    graphics::drawRect(m_pos_x, m_pos_y - 40, 50 * hp_pct, 8, br);
}



TroopEnemy::TroopEnemy(float x, float y) : Enemy(x, y) {
    m_hp = 60.0f;       
    m_max_hp = 60.0f;
    m_speed = 150.0f;  
    m_damage = 5.0f;    
    m_reward = 10.0f;   
}

void TroopEnemy::draw() {
    graphics::Brush br;

    
    br.texture = "assets/soldier.png";
    br.outline_opacity = 0.0f;
    br.fill_opacity = 1.0f;

    graphics::drawRect(m_pos_x, m_pos_y, 50, 50, br);

    br.texture = "";

    float hp_pct = m_hp / m_max_hp;
    if (hp_pct < 0) hp_pct = 0;

    br.fill_color[0] = 0.0f; br.fill_color[1] = 0.0f; br.fill_color[2] = 0.0f;
    graphics::drawRect(m_pos_x, m_pos_y - 25, 35, 5, br);

    br.fill_color[0] = 0.0f; br.fill_color[1] = 1.0f; br.fill_color[2] = 0.0f;
    graphics::drawRect(m_pos_x, m_pos_y - 25, 35 * hp_pct, 5, br);
}



RangeEnemy::RangeEnemy(float x, float y) : Enemy(x, y) {
    m_hp = 90.0f;      
    m_max_hp = 90.0f;
    m_speed = 100.0f;   
    m_damage = 15.0f;   
    m_reward = 20.0f;
    m_attack_range = 280.0f;
}

void RangeEnemy::update(float dt) {
    MapNode* kingNode = m_target_node;
    while (kingNode && kingNode->next != nullptr) {
        kingNode = kingNode->next;
    }

    if (kingNode) {
        float dx = kingNode->x - m_pos_x;
        float dy = kingNode->y - m_pos_y;
        float distToBase = std::sqrt(dx * dx + dy * dy);

        if (distToBase <= m_attack_range) {
            GlobalState::getInstance()->m_player_hp -= m_damage * dt;
            return;
        }
    }
    Enemy::update(dt);
}

void RangeEnemy::draw() {
    graphics::Brush br;

   
    br.texture = "assets/range.png";
    br.outline_opacity = 0.0f;
    br.fill_opacity = 1.0f;

   
    graphics::drawRect(m_pos_x, m_pos_y, 100, 100, br);

    
    br.texture = "";

    float hp_pct = m_hp / m_max_hp;
    if (hp_pct < 0) hp_pct = 0;

    br.fill_color[0] = 0.0f; br.fill_color[1] = 0.0f; br.fill_color[2] = 0.0f;
    graphics::drawRect(m_pos_x, m_pos_y - 25, 35, 5, br);

    br.fill_color[0] = 0.0f; br.fill_color[1] = 1.0f; br.fill_color[2] = 0.0f;
    graphics::drawRect(m_pos_x, m_pos_y - 25, 35 * hp_pct, 5, br);
}