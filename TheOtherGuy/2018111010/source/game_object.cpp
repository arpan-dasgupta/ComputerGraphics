#include "game_object.h"


GameObject::GameObject() 
    : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false) { }

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, glm::vec2 offset, float rotation) 
    : Position(pos), Size(size), Velocity(velocity), Color(color), Sprite(sprite), Offset(offset), IsSolid(false), Destroyed(false), Rotation(rotation) { this->flipped=false;}

void GameObject::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(this->Sprite, this->Position + this->Offset, this->Size, this->Rotation, flipped);
}