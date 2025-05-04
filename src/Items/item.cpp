//
// Created by beemi on 5/1/2025.
//
#include "item.h"

Item::Item(string t) {
    text = t;
}

void Item::setUniformsAndDraw() const {
    for (const shared_ptr<Shape>& s : shapes) {
        s->setUniforms();
        s->draw();
    }
}

bool Item::isOverlapping(const vec2 &point) const {
    for (const shared_ptr<Shape>& s : shapes) {
        if (s->isOverlapping(point)) return true;
    }
    return false;
}

string Item::getText() const {
    return text;
}
void Item::setText(string t) {
    text = t;
}

void Item::pushShape(const shared_ptr<Shape> shape) {
    shapes.push_back(shape);
}

bool Item::pop(int i) {
    if (shapes.size() != i + 1) return false;
    shapes.erase(shapes.end() - 1);
    return true;
}

void Item::clearShapes() {
    shapes.clear();
}

