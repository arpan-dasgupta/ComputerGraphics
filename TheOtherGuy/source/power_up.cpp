// #include "power_up.h"
#include "maze.h"

#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <math.h>
#include <time.h>

#define PI 3.14159265358979323846

void PowerUp::Draw(SpriteRenderer &renderer)
{
    for (GameObject &tile : this->Walls)
    {
        // if (!tile.Destroyed)
        tile.Offset = this->Position;
        tile.Draw(renderer);
        // std::cout<<this->Position.x<<" "<<this->Position.y<<"\n";
    }
}
void PowerUp::init()
{
    this->active = true;
    if(ObjectType==0)
    {
        this->Walls.push_back(GameObject(glm::vec2(-25.0,-25.0), glm::vec2(50.0,50.0), ResourceManager::GetTexture("door")));
    }
    else if(ObjectType==1)
    {
        this->Walls.push_back(GameObject(glm::vec2(-25.0,-25.0), glm::vec2(50.0,50.0), ResourceManager::GetTexture("star")));
    }
    else if(ObjectType==2)
    {
        this->Walls.push_back(GameObject(glm::vec2(-25.0,-25.0), glm::vec2(50.0,50.0), ResourceManager::GetTexture("button")));
    }
    else if(ObjectType==3)
    {
        this->Walls.push_back(GameObject(glm::vec2(-25.0,-25.0), glm::vec2(50.0,50.0), ResourceManager::GetTexture("coin")));
    }
    else if(ObjectType==4)
    {
        this->Walls.push_back(GameObject(glm::vec2(-25.0,-25.0), glm::vec2(50.0,50.0), ResourceManager::GetTexture("danger")));
    }
    for (GameObject &tile : this->Walls)
    {
        // if (!tile.Destroyed)
        tile.Offset = this->Position;
        // tile.Draw(renderer);
        // std::cout<<this->Position.x<<" "<<this->Position.y<<"\n";
    }
}