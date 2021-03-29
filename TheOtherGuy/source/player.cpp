// #include "power_up.h"
#include "maze.h"

#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <math.h>
#include <time.h>

#define PI 3.14159265358979323846

void GameCharacter::Draw(SpriteRenderer &renderer)
{
    for (GameObject &tile : this->Walls)
    {
        // std::cout<<this->ObjectType<<"\n";
        tile.Offset = this->Position;
        tile.Draw(renderer);
    }
}

void GameCharacter::init()
{
    // std::cout<<"OKay\n";
    GameObject gg = GameObject(glm::vec2(0.0,0.0), glm::vec2(50.0f, 60.0f), ResourceManager::GetTexture("player_1"));
    if(this->ObjectType==1)
    {
        gg = GameObject(glm::vec2(-25.0,-30.0), glm::vec2(50.0f, 60.0f), ResourceManager::GetTexture("enemy"));
    }
    gg.Offset = glm::vec2(this->Position);
    this->Walls.push_back(gg);
    
    // exit(0);
    // this->active = true;
    // if(ObjectType==0)
    // {
    //     this->Walls.push_back(GameObject(glm::vec2(0.0,0.0), glm::vec2(50.0,50.0), ResourceManager::GetTexture("door")));
    // }
    // else if(ObjectType==1)
    // {
    //     this->Walls.push_back(GameObject(glm::vec2(0.0,0.0), glm::vec2(50.0,50.0), ResourceManager::GetTexture("star")));
    // }
    // else if(ObjectType==2)
    // {
    //     this->Walls.push_back(GameObject(glm::vec2(0.0,0.0), glm::vec2(50.0,50.0), ResourceManager::GetTexture("button")));
    // }
    // else if(ObjectType==3)
    // {
    //     this->Walls.push_back(GameObject(glm::vec2(0.0,0.0), glm::vec2(50.0,50.0), ResourceManager::GetTexture("coin")));
    // }
    // else if(ObjectType==4)
    // {
    //     this->Walls.push_back(GameObject(glm::vec2(0.0,0.0), glm::vec2(50.0,50.0), ResourceManager::GetTexture("danger")));
    // }
}