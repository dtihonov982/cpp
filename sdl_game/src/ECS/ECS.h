#ifndef ECS_H
#define ECS_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include "../PolyIndex.h"
#include <unordered_set>

class Component;
class Entity;
class Manager;

constexpr std::size_t maxComponent = 32;

class Component {
public:
    Entity* entity;
    virtual void init() {}
    virtual void draw() {}
    virtual void update() {}

    virtual ~Component() {};
};

class Entity {
private:
	Manager* managerPtr = nullptr;
    bool active = true;
    std::vector<std::shared_ptr<Component>> components;

    PolyIndex<Component, maxComponent> index;
public:
	Entity(Manager* manager): managerPtr(manager) {}
	Entity() {}
	
    void update() {
        for (auto& c: components) c->update();

    }
    void draw() {
        for (auto& c: components) c->draw();
    }
    bool isActive() const { return active; }

    void destroy() { active = false; }

    template<typename T>
    bool hasComponent() const {
        return index.has<T>();
    }

    //???
    template<typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs) {
        T* c = new T(std::forward<TArgs>(mArgs)...);
        c->entity = this;

        std::shared_ptr<Component> uPtr{c};
        components.push_back(std::move(uPtr));

        index.add<T>(*c);

        c->init();

        return *c;
    }

    template<typename T>
    T& getComponent() const {
        return index.get<T>();
    }
    
    #if 0
    void addGroup(Group group);
    
    bool hasGroup(Group group) {
    	return groupBitset[group];
    }
    
    void delroup(Group group) {
    	groupBitset[group] = false;
    }
    #endif
    	
};

enum class GroupLabel {all, tiles, players, enemies, colliders }; 
using Group = std::unordered_set<std::shared_ptr<Entity>>; 

class Manager {
private:
    std::unordered_map<GroupLabel, Group> groups = {{GroupLabel::all, Group{}}};;
public:

    void update() {
        for (auto& e: groups[GroupLabel::all]) e->update();
    }

    void draw() {
        for (auto& e: groups[GroupLabel::all]) e->draw();
    }

    void draw(const std::vector<GroupLabel>& labels) {
        for (auto label: labels) {
            for (auto e: groups[label]) {
                e->draw();
            }
        }
    }
    
    void refresh() {
    #if 0
        auto inactivePred = [](const std::shared_ptr<Entity> entity) { return !entity->isActive(); };
        for (auto& [label, group]: groups) {
            auto remIter = std::remove_if(std::begin(group), std::end(group), inactivePred);
            group.erase(remIter, std::end(group));
        }
    #endif
    }
    
    Entity& addEntity(GroupLabel label) {
        std::shared_ptr<Entity> ptr = std::make_shared<Entity>(this);
        groups[GroupLabel::all].insert(ptr);
        groups[label].insert(ptr);
        return *ptr;
    }

    #if 0 
	void addToGroup(Entity* entity, GroupLabel group) {
    	groups[group].push_back(entity);
	}
    
    Group& getGroup(GroupLabel label) {
    	return groups[label];
    }
    #endif
};

#endif // ECS_H
