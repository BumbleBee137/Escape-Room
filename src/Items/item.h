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
using std::make_unique, std::shared_ptr, glm::vec2;

class Item {
protected:
    vector<shared_ptr<Shape>> shapes;
    string text;

public:
    // Constructor
    Item(string t);

    // Draw the item
    void setUniformsAndDraw() const;

    // Return true if overlaps with a point given
    bool isOverlapping(const vec2& point) const;

    //text associated with item (what's displayed at the top of the screen)
    string getText() const;
    void setText(string t);

    //editing the shape makeup of the item
    void pushShape(const std::shared_ptr<Shape> shape);
    void clearShapes();


};

#endif //ITEM_H
