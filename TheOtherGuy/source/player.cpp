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
    if(this->ObjectType==0)
    {
        if(!this->active)
        {
            this->counter = 0;
            GameObject &tile = this->Walls[0];
            tile.Offset = this->Position;
            tile.flipped = this->flipped;
            tile.Draw(renderer);    
        }
        else
        {
            this->counter++;
            GameObject &tile = this->Walls[(this->counter/10)%5];
            tile.Offset = this->Position;
            tile.flipped = this->flipped;
            tile.Draw(renderer);    
        }
        return;
    }
    for (GameObject &tile : this->Walls)
    {
        // std::cout<<this->ObjectType<<"\n";
        tile.Offset = this->Position;
        tile.flipped = this->flipped;
        tile.Draw(renderer);
    }
}

void GameCharacter::init()
{
    // std::cout<<"OKay\n";
    if(this->ObjectType==1)
    {
        GameObject gg = GameObject(glm::vec2(-25.0,-30.0), glm::vec2(50.0f, 60.0f), ResourceManager::GetTexture("enemy"));
        gg.Offset = glm::vec2(this->Position);
        this->Walls.push_back(gg);
    }
    else{
        GameObject gg = GameObject(glm::vec2(0.0,0.0), glm::vec2(50.0f, 60.0f), ResourceManager::GetTexture("player_1"));
        gg.Offset = glm::vec2(this->Position);
        this->Walls.push_back(gg);
        GameObject gg2 = GameObject(glm::vec2(0.0,0.0), glm::vec2(50.0f, 60.0f), ResourceManager::GetTexture("player_3"));
        gg2.Offset = glm::vec2(this->Position);
        this->Walls.push_back(gg2);
        GameObject gg3 = GameObject(glm::vec2(0.0,0.0), glm::vec2(50.0f, 60.0f), ResourceManager::GetTexture("player_4"));
        gg3.Offset = glm::vec2(this->Position);
        this->Walls.push_back(gg3);
        GameObject gg4 = GameObject(glm::vec2(0.0,0.0), glm::vec2(50.0f, 60.0f), ResourceManager::GetTexture("player_2"));
        gg4.Offset = glm::vec2(this->Position);
        this->Walls.push_back(gg4);
        GameObject gg5 = GameObject(glm::vec2(0.0,0.0), glm::vec2(50.0f, 60.0f), ResourceManager::GetTexture("player_5"));
        gg5.Offset = glm::vec2(this->Position);
        this->Walls.push_back(gg5);
    }
    this->counter = 0;
    
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