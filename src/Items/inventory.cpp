//
// Created by beemi on 5/2/2025.
//

#include "inventory.h"

Inventory::Inventory(Shader& shader) {
    size = 88;
    boxes.push_back(make_unique<Rect>(shader, vec2(size,size), vec2(size * 1.5,size * 1.5), color(1, 1, 1, 1)));
    offsetX = 0;
    for (int i = 0; i < 9; i++) {
        boxes.push_back(make_unique<Rect>(shader, vec2(size + offsetX,size), vec2(size * 1.25,size * 1.25), color(.5, .5, .5, .5)));
        offsetX += 2 * size;
    }
}

void Inventory::setUniformsAndDraw() const {
    for (const unique_ptr<Rect>& r : boxes) {
        r->setUniforms();
        r->draw();
    }
}