#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H
#include "Components.h"

class PositionComponent: public Component
{
public:
    PositionComponent() {
        setPos(0, 0);
    }
    PositionComponent(int x, int y) {
        setPos(x, y);
    }
    int x() {return xpos;}
    int y() {return ypos;}
    void setPos(int x, int y) {
        xpos = x;
        ypos = y;
    }

    void update() {
        xpos++;
        ypos++;
    }

private:
    int xpos;
    int ypos;
};

#endif // POSITIONCOMPONENT_H
