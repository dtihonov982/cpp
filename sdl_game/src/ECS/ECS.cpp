#include "ECS/ECS.h"

void Entity::addGroup(Group group) {
    groupBitset[group] = true;
    managerPtr->addToGroup(this, group);
}
