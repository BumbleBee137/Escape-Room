//
// Created by beemi on 5/4/2025.
//
#include "move.h"



Move::Move(string t) : Item(t) {}

void Move::resize(float size) {
    for (const shared_ptr<Shape>& s : shapes) {
        s->setSize(size);
    }
}

void Move::move(vec2 p) {
    for (const shared_ptr<Shape>& s : shapes) {
        s->move(p-pos);
    }
}

void Move::rotate(float angle) {
    for (const shared_ptr<Shape>& s : shapes) {
        s->rotate(angle);
    }
}

void Move::pop() {
    shapes.erase(shapes.end() - 1);
}

bool Move::clicked() {
    return changed;
}

void Move::click() {
    changed = true;
}
