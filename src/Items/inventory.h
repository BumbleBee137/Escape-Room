//
// Created by beemi on 5/2/2025.
//

#ifndef INVENTORY_H
#define INVENTORY_H

#include "../shapes/shape.h"
#include "../shapes/rect.h"
#include "../shader/shader.h"
#include "../util/color.h"
#include "glm/glm.hpp"
#include "hold.h"
#include <memory>
using std::make_unique, std::shared_ptr, std::unique_ptr, glm::vec2;


class Inventory {
private:
    vector<unique_ptr<Rect>> boxes; //box[0] is the one that goes behind the selected box to emphasize it
    int offsetX, size, index;
    vector<shared_ptr<Hold>> items;


public:
    //constructor
    Inventory(Shader& shader);

    //drawing all shapes
    void setUniformsAndDraw() const;

    //moves box[0] to the position specified, returns text of pointer at that index
    string select(int i);

    //returns pointer of the currently selected object
    shared_ptr<Hold> current();

    //adds an object to the inventory
    string grab(shared_ptr<Hold> hold);

    //removes the currently selected object from the inventory
    void remove();

};

#endif //INVENTORY_H
