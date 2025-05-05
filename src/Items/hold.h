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
    Hold(string t, vec2 p);


    void resize(vec2 l);
    void move(vec2 p);
    void rotate(float angle);

    void setGrabbed(bool tf);
    bool getGrabbed();
    void drop();
    vec2 getPos() const;


};

#endif //HOLD_H
