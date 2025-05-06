//
// Created by beemi on 5/2/2025.
//

#ifndef HOLD_H
#define HOLD_H

#include "item.h"

class Hold : public Item {
private:
    bool grabbed;
    vec2 pos, size;

public:
    //constructor
    Hold(string t, vec2 p);

    //tools to move the object around on the screen
    void resize(vec2 l);
    void move(vec2 p);
    void rotate(float angle);

    //tools for using object in the inventory
    void setGrabbed(bool tf);
    bool getGrabbed();
    void drop();

    //getter for overall position - used for moving object
    vec2 getPos() const;


};

#endif //HOLD_H
