#pragma once
#include "graphics.h"

class GameObject {
protected:
    float m_pos_x, m_pos_y;
    float m_width, m_height;
    bool m_active = true; 

public:
    GameObject(float x, float y, float w, float h)
        : m_pos_x(x), m_pos_y(y), m_width(w), m_height(h) {
    }

    virtual ~GameObject() {} 

   
    virtual void update(float dt) = 0;
    virtual void draw() = 0;
    virtual void init() = 0;

    bool isActive() const { return m_active; }
    void setActive(bool a) { m_active = a; }
}; 
