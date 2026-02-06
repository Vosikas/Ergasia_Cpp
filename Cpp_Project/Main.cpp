#include "graphics.h"
#include "global_state.h"

void update(float ms) {
    float dt = ms / 1000.0f;
    GlobalState::getInstance()->update(dt);
}

void draw() {
    graphics::Brush br;
    br.fill_color[0] = 0.1f;
    br.fill_color[1] = 0.1f;
    br.fill_color[2] = 0.1f;
    graphics::setWindowBackground(br);

    GlobalState::getInstance()->draw();
}

int main() {
    graphics::createWindow(1280, 720, "Tower Defense");

    GlobalState::getInstance()->init();

    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);

    graphics::setCanvasSize(1600, 900);

    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);

    graphics::startMessageLoop();
    return 0;
}