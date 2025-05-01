//
// Created by beemi on 5/1/2025.
//
#include "item.h"

Item::Item() {}

void Item::setUniformsAndDraw() const {
    for (const unique_ptr<Shape>& s : shapes) {
        s->setUniforms();
        s->draw();
    }
}

bool Item::isOverlapping(const vec2 &point) const {
    for (const unique_ptr<Shape>& s : shapes) {
        return s->isOverlapping(point);
    }
}

