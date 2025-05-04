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
    vector<unique_ptr<Rect>> boxes;
    int offsetX, size, index;
    vector<shared_ptr<Hold>> items;


public:
    Inventory(Shader& shader);

    void setUniformsAndDraw() const;
    string select(int i);
    shared_ptr<Hold> current();
    string grab(shared_ptr<Hold> hold);
    void remove();

};

#endif //INVENTORY_H
