#pragma once
#include "game_object.h"
#include "graphics.h"

class Tower : public GameObject {
    float m_damage = 25.0f;
    float m_range = 140.0f;
    float m_cooldown = 0.6f;
    float m_time_since_last_shot = 0.0f;
    float m_life = 0.0f;

public:
    Tower(float x, float y);

    void update(float dt) override;
    void draw() override;
    void init() override;
};