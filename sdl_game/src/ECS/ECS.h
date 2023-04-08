#ifndef ECS_H
#define ECS_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getComponentTypeID() {
    static ComponentID lastID = 0;
    return lastID++;
}

template<typename T> inline ComponentID getComponentTypeID() noexcept {
    static ComponentID typeID = getComponentTypeID();
    return typeID;
}

constexpr std::size_t maxComponent = 32;
constexpr std::size_t maxGroups = 32;
using ComponentBitSet = std::bitset<maxComponent>;
using GroupBitSet = std::bitset<maxGroups>;
using ComponentArray = std::array<Component*, maxComponent>;

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
    std::vector<std::unique_ptr<Component>> components;

    ComponentArray componentArray;
    ComponentBitSet componentBitSet;
    GroupBitSet groupBitset;
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
        return componentBitSet[getComponentTypeID<T>()];
    }

    //???
    template<typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs) {
        T* c(new T(std::forward<TArgs>(mArgs)...));
        c->entity = this;
        std::unique_ptr<Component> uPtr(c);
        components.emplace_back(std::move(uPtr));

        componentArray[getComponentTypeID<T>()] = c;
        componentBitSet[getComponentTypeID<T>()] = true;

        c->init();

        return *c;
    }

    //???
    template<typename T>
    T& getComponent() const {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T*>(ptr);
    }
    
    void addGroup(Group group);
    
    bool hasGroup(Group group) {
    	return groupBitset[group];
    }
    
    void delGroup(Group group) {
    	groupBitset[group] = false;
    }
    	
};

class Manager {
private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::array<std::vector<Entity*>, maxGroups> groupedEntities;
public:
    void update() {
        for (auto& e: entities) e->update();
    }
    void draw() {
        for (auto& e: entities) e->draw();
    }

    void refresh() {
    	int i = 0;
    	auto outOfGroupPred = [&i](Entity* entity) { return !entity->isActive() || !entity->hasGroup(i); };

    	for (; i < maxGroups; ++i) {
    		std::vector<Entity*>& curr = groupedEntities[i];
    		auto removeIter = std::remove_if(curr.begin(), curr.end(), outOfGroupPred);
    		curr.erase(removeIter, curr.end());
    	}    		
   		  	    	
        auto inactivePred = [](const std::unique_ptr<Entity>& mEntity) { return !mEntity->isActive(); };
        auto remIter = std::remove_if(std::begin(entities), std::end(entities), inactivePred);
        entities.erase(remIter, std::end(entities));
    }
    
    Entity& addEntity() {
        Entity* e = new Entity{this};
        entities.emplace_back(e);
        return *e;
    }
    
	void addToGroup(Entity* entity, Group group) {
    	groupedEntities[group].push_back(entity);
	}
    
    std::vector<Entity*>& getGroup(Group group) {
    	return groupedEntities[group];
    }
};







#endif // ECS_H
