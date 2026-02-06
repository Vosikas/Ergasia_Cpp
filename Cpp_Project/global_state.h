#pragma once
#include <list>
#include <string>
#include "game_object.h"

struct MapNode {
    float x, y;
    MapNode* next;
};

class GlobalState {
public:
    static GlobalState* m_instance;
    std::list<GameObject*> m_active_objects;

    MapNode* m_path_start = nullptr;

    bool m_mouse_down = false;
    float m_spawn_timer = 0.0f;

    std::string m_error_msg = "";
    float m_error_timer = 0.0f;

    float m_player_hp = 700.0f;
    float m_max_player_hp = 700.0f;

    GlobalState() {}

public:
    static GlobalState* getInstance();
    void reloadGame();
    void init();
    void update(float dt);
    void draw();

    bool isPointOnPath(float x, float y, float radius);

    void addGameObject(GameObject* obj) {
        m_active_objects.push_back(obj);
    }
    std::list<GameObject*>& getActiveObjects() {
        return m_active_objects;
    }
    
    float money = 150.0f;

    float getMoney() {
        return money;
    }

    const float T_lifespan = 12.0f; 
    const float T_price = 40.0f;    

    float game_duration = 0.0f;

    MapNode* getPathStart() { return m_path_start; }
};