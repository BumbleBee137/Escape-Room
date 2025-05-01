//
// Created by beemi on 4/22/2025.
//

#ifndef ITEM_H
#define ITEM_H

#include "../shapes/shape.h"
#include "../shader/shader.h"
#include "../util/color.h"
#include "glm/glm.hpp"
#include <memory>
using std::make_unique, std::unique_ptr, glm::vec2;

class Item {
private:
    vector<unique_ptr<Shape>> shapes;

public:
    // Constructors
    Item();

    // Draw the item
    void setUniformsAndDraw() const;

    // Return true if overlaps with a point given
    virtual bool isOverlapping(const vec2& point) const;
};

#endif //ITEM_H
