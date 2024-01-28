//
// Created by Sachin Saw on 23/01/24.
//

#include "Entity.h"

Entity::Entity(const std::string &tag, const size_t id) : m_id(id), m_tag(tag) {

}

void Entity::destroy() {
    this->m_alive = false;
}

bool Entity::isAlive() const {
    return m_alive;
}

const std::string &Entity::tag() const {
    return m_tag;
}

const size_t Entity::id() const {
    return m_id;
}

float Entity::distanceFrom(const std::shared_ptr<Entity> &other) const {
    Vec2 distance = Vec2(other->cTransform->pos.x - this->cTransform->pos.x,
                         other->cTransform->pos.y - this->cTransform->pos.y);
    return distance.length();
}
