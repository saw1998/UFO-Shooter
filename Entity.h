//
// Created by Sachin Saw on 23/01/24.
//

#ifndef GAMEENGINE_ENTITY_H
#define GAMEENGINE_ENTITY_H

#include <memory>
#include <string>
#include "Components.h"

class Entity {
    friend class EntityManager;

    bool m_alive = true;
    const size_t m_id = 0;
    std::string m_tag = "default";

    //constructor and destructor
    Entity(const std::string & tag, const size_t id);


public:
    // component pointers
    std::shared_ptr<CTransform> cTransform;
    std::shared_ptr<CShape> cShape;
    std::shared_ptr<CCollision> cCollision;
    std::shared_ptr<CInput> cInput;
    std::shared_ptr<CScore> cScore;
    std::shared_ptr<CLifespan> cLifeSpan;

    //private member access functions

    bool isAlive() const;
    const std::string & tag() const;
    const size_t id() const;
    void destroy();
    float distanceFrom(const std::shared_ptr<Entity> & other ) const;
};


#endif //GAMEENGINE_ENTITY_H
