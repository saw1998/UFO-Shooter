//
// Created by Sachin Saw on 23/01/24.
//

#include<iostream>
#include "EntityManager.h"

void EntityManager::update() {
    for(auto e : m_entitiesToAdd){
        m_entities.push_back(e);
        m_entityMap[e->tag()].push_back(e);
    }

    m_entitiesToAdd.clear();

    m_entities.erase(std::remove_if(
            m_entities.begin(), m_entities.end(),
            [](const std::shared_ptr<Entity> & e){
                return !e->m_alive;
            }),m_entities.end());

    for(auto &[tag,entity_list] : m_entityMap){
        entity_list.erase(std::remove_if(
                entity_list.begin(), entity_list.end(),
                [](const std::shared_ptr<Entity> & e){
                    return !e->m_alive;
                }), entity_list.end());
    }
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag) {
    //create a new Entity object
    auto entity = std::shared_ptr<Entity>(new Entity(tag, m_totalEntities++));

    //store it in the vector of all entities
    m_entitiesToAdd.push_back(entity);

    //return the shared pointer pointing to that entity
    return entity;
}

EntityVector &EntityManager::getEntities() {
    return m_entities;
}

EntityVector &EntityManager::getEntities(const std::string &tag) {
    return m_entityMap[tag];
}

EntityManager::EntityManager() {

}


