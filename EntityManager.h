//
// Created by Sachin Saw on 23/01/24.
//

#ifndef GAMEENGINE_ENTITYMANAGER_H
#define GAMEENGINE_ENTITYMANAGER_H

#include<iostream>
#include <map>
#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVector;
typedef std::unordered_map<std::string, EntityVector> EntityMap;
class EntityManager {

    EntityVector m_entities;
    EntityVector m_entitiesToAdd;
    EntityMap m_entityMap;
    size_t m_totalEntities = 0;

public:
    EntityManager();
    std::shared_ptr<Entity> addEntity(const std::string& tag);
    EntityVector& getEntities();
    EntityVector& getEntities(const std::string& tag);

    void update();
};


#endif //GAMEENGINE_ENTITYMANAGER_H
