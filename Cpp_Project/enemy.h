#pragma once
#include "game_object.h"
#include "graphics.h"
#include "global_state.h" 

class Enemy : public GameObject {
protected:
    float m_hp ;        
    float m_max_hp=m_hp ;    
    float m_speed ;
    float m_damage;
    float m_reward;
    
    MapNode* m_target_node = nullptr; 

public:
    
    Enemy(float x, float y);

    void update(float dt) override;
    void draw() override;
    void init() override;
    float getX() const {
        return m_pos_x;
    }
    float getY() const {
        return m_pos_y;
    }
    float getHP()  {
        return m_hp;
    }
    void updateHP(float dmg) {
        m_hp -= dmg;
        if (m_hp <= 0.0f) {
            m_active = false; 
            GlobalState::getInstance()->money += m_reward;
        }
    }
};

class TankEnemy : public Enemy {
public:
    TankEnemy(float x, float y);

    void draw() override;
};
class RangeEnemy : public Enemy {
private:
    float m_attack_range;
public:
    RangeEnemy(float x, float y);

    void update(float dt) override;
    void draw() override;
};
class TroopEnemy : public Enemy {
    public:
    TroopEnemy(float x, float y);
    
    void draw() override;
};
