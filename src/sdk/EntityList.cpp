#include "EntityList.h"
#include "Entity.h"
#include <vector>

std::vector<Entity*>* EntityList::entityList = new std::vector<Entity*>;

Entity* EntityList::GetEntity(int index) {
    return entityList->at(index);
}

int EntityList::GetSize() {
    return entityList->size();
}

void EntityList::Populate() {
    entityList->reserve(60);

    for(int i = 0; i < 60; i++) {
        Entity* entity = new Entity(i);

        entityList->push_back(entity);
    }
}

void EntityList::Update() {
    for(int i = 0; i < EntityList::GetSize(); i++) {
        Entity* entity = entityList->at(i);

        entity->Update();
    }
}