//
// Created by beemi on 5/4/2025.
//
#ifndef MOVE_H
#define MOVE_H

#include "item.h"
class Move : public Item {
private:
    vec2 pos;
    bool changed = false;

public:
    Move(string t, vec2 p);

    void resize(vec2 size);
    void move(vec2 p);
    void rotate(float angle);
    void pop();
    bool clicked();
    void click();



};
#endif //MOVE_H
