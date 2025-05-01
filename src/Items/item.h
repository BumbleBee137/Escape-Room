//
// Created by beemi on 4/22/2025.
//

#ifndef ITEM_H
#define ITEM_H

#include "shape.h"
#include "../shader/shader.h"
#include <memory>
using std::make_unique, std::unique_ptr, glm::vec2;

class Item {
private:
    vector<unique_ptr<Shape>> shapes;

public:
    // Constructors
    Item();
    Item(Shader& shader, vec2 pos);

    // Draw the item
    void setUniformsAndDraw() const;

    // Return true if overlaps with a point given
    bool isOverlapping(const vect2& point) const;
};

#endif //ITEM_H
