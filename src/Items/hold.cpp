//
// Created by beemi on 5/2/2025.
//
#include "hold.h"

Hold::Hold(string t, vec2 p) : Item(t) {
    grabbed = false;
    pos = p;
}

void Hold::resize(vec2 l) {
    for (const shared_ptr<Shape>& s : shapes) {
        s->setSize(l);
    }
}

void Hold::move(vec2 p) {
    for (const shared_ptr<Shape>& s : shapes) {
        s->move(p-pos);
        s->setPos(p);
    }
}

void Hold::rotate(float angle) {
    for (const shared_ptr<Shape>& s : shapes) {
        s->rotate(angle);
    }
}

void Hold::setGrabbed(bool tf) {
    grabbed = tf;
}
bool Hold::getGrabbed() {
    return grabbed;
}

void Hold::drop() {
    this->resize(size);
}

vec2 Hold::getPos() const {
    return pos;
}
