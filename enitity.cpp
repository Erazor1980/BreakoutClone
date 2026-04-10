#include "entity.h"

sf::FloatRect Entity::get_bounding_box() const noexcept
{
    return m_sprite.getGlobalBounds();
}

bool Entity::is_destroyed() const noexcept
{
    return m_bDestroyed;
}