/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "maze.h"

#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <math.h>

#define PI 3.14159265358979323846

// void Maze::Load(const char *file, unsigned int levelWidth, unsigned int levelHeight)
// {
//     // clear old data
//     this->Walls.clear();
//     // load from file
//     unsigned int tileCode;
//     Maze level;
//     std::string line;
//     std::ifstream fstream(file);
//     std::vector<std::vector<unsigned int>> tileData;
//     if (fstream)
//     {
//         while (std::getline(fstream, line)) // read each line from level file
//         {
//             std::istringstream sstream(line);
//             std::vector<unsigned int> row;
//             while (sstream >> tileCode) // read each word separated by spaces
//                 row.push_back(tileCode);
//             tileData.push_back(row);
//         }
//         if (tileData.size() > 0)
//             this->init(tileData, levelWidth, levelHeight);
//     }
// }

void Maze::Draw(SpriteRenderer &renderer)
{
    for (GameObject &tile : this->Walls)
    {
        // if (!tile.Destroyed)
        tile.Offset = this->Position;
        tile.Draw(renderer);
    }
}

// bool Maze::IsCompleted()
// {
//     for (GameObject &tile : this->Walls)
//         if (!tile.IsSolid && !tile.Destroyed)
//             return false;
//     return true;
// }



void Maze::init()
{
    // // calculate dimensions
    // unsigned int height = tileData.size();
    // unsigned int width = tileData[0].size(); // note we can index vector at [0] since this function is only called if height > 0
    // float unit_width = levelWidth / static_cast<float>(width), unit_height = levelHeight / height; 
    // // initialize level tiles based on tileData		
    // for (unsigned int y = 0; y < height; ++y)
    // {
    //     for (unsigned int x = 0; x < width; ++x)
    //     {
    //         // check block type from level data (2D level array)
    //         if (tileData[y][x] == 1) // solid
    //         {
    //             glm::vec2 pos(unit_width * x, unit_height * y);
    //             glm::vec2 size(unit_width, unit_height);
    //             GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
    //             obj.IsSolid = true;
    //             this->Walls.push_back(obj);
    //         }
    //         else if (tileData[y][x] > 1)	// non-solid; now determine its color based on level data
    //         {
    //             glm::vec3 color = glm::vec3(1.0f); // original: white
    //             if (tileData[y][x] == 2)
    //                 color = glm::vec3(0.2f, 0.6f, 1.0f);
    //             else if (tileData[y][x] == 3)
    //                 color = glm::vec3(0.0f, 0.7f, 0.0f);
    //             else if (tileData[y][x] == 4)
    //                 color = glm::vec3(0.8f, 0.8f, 0.4f);
    //             else if (tileData[y][x] == 5)
    //                 color = glm::vec3(1.0f, 0.5f, 0.0f);

    //             glm::vec2 pos(unit_width * x, unit_height * y);
    //             glm::vec2 size(unit_width, unit_height);
    //             this->Walls.push_back(GameObject(pos, size, ResourceManager::GetTexture("block"), color));
    //         }
    //     }
    // }
    int num_hor = (rand()%12 + 3), num_vert = (rand()%1 + 3);
    float roomsize = 200;
    float roomdist = 100;
    glm::vec2 room_shape = glm::vec2(roomsize,roomsize);
    glm::vec2 room_offset = glm::vec2(roomsize/2,roomsize/2);
    mazeSize = glm::vec2(roomdist*(num_hor-1)+ roomsize*num_hor + 200, roomdist*(num_vert-1)+ roomsize*num_vert + 200);
    int dir = rand()%1;
    if(dir==0)
    {
        int curr = 0;
        adjList.resize(3*num_hor);
        std::pair<int,int> prep1, prep2;
        int pre_count=0;
        for(int i=0;i<num_hor;i++)
        {
            int num = (rand()%1)+1;
            std::pair<int,int> pp1, pp2;
            if(num==1)
            {
                curr++;
                pp1 = {i*(roomsize+roomdist),(rand()%num_vert)*(roomsize+roomdist)};
                vertexPositions.push_back(pp1);
                if(i!=0)
                {
                    if(pre_count==1)
                    {
                        adjList[curr].push_back(curr-1);
                        adjList[curr-1].push_back(curr);
                        edges.push_back({curr,curr-1});
                    }
                    else
                    {
                        adjList[curr].push_back(curr-1);
                        adjList[curr-1].push_back(curr);
                        edges.push_back({curr,curr-1});
                        adjList[curr].push_back(curr-2);
                        adjList[curr-2].push_back(curr);
                        edges.push_back({curr,curr-2});
                    }
                }
            }
            else
            {
                curr +=2;
                pp1 = {i*(roomsize+roomdist),(rand()%(num_vert/2))*(roomsize+roomdist)};
                pp2 = {i*(roomsize+roomdist),(rand()%(num_vert/2)+(num_vert/2))*(roomsize+roomdist)};
                vertexPositions.push_back(pp1);
                vertexPositions.push_back(pp2);
                adjList[curr].push_back(curr-1);
                adjList[curr-1].push_back(curr);
                edges.push_back({curr,curr-1});
                if(i!=0)
                {
                    if(pre_count==1)
                    {
                        adjList[curr].push_back(curr-2);
                        adjList[curr-2].push_back(curr);
                        edges.push_back({curr,curr-2});
                        adjList[curr-1].push_back(curr-2);
                        adjList[curr-2].push_back(curr-1);
                        edges.push_back({curr-1,curr-2});
                    }
                    else
                    {
                        adjList[curr].push_back(curr-2);
                        adjList[curr-2].push_back(curr);
                        edges.push_back({curr,curr-2});
                        adjList[curr-1].push_back(curr-3);
                        adjList[curr-3].push_back(curr-1);
                        edges.push_back({curr-1,curr-3});
                    }
                }
            }
            pre_count = num;
            prep1 = pp1, prep2 = pp2;
        }
        for(int i=0;i<vertexPositions.size();i++)
        {
            // std::cout<<vertexPositions[i].first<<" "<<vertexPositions[i].second<<'\n';
            glm::vec2 room_pos = glm::vec2(vertexPositions[i].first,vertexPositions[i].second);
            this->Walls.push_back(GameObject(room_pos+room_offset, room_shape, ResourceManager::GetTexture("grey")));     
        }
        for(int i=0;i<edges.size();i++)+
        {
            float rot = 0;
            glm::vec2 room_pos_1 = glm::vec2(vertexPositions[edges[i].first].first,vertexPositions[edges[i].first].second) + room_offset;
            glm::vec2 room_pos_2 = glm::vec2(vertexPositions[edges[i].second].first,vertexPositions[edges[i].second].second) + room_offset;
            if(room_pos_2.x==room_pos_1.x)
            {
                rot = (room_pos_2.y>room_pos_1.y)?-PI/2:PI/2;
            }
            else{
                rot = glm::tanh((room_pos_2.y-room_pos_1.y)/(room_pos_2.x-room_pos_1.x));
            }
            glm::vec2 path_shape = glm::vec2(glm::length(room_pos_1-room_pos_2),75.0);
            GameObject obj = GameObject(room_pos_1, path_shape, ResourceManager::GetTexture("block"));
            obj.Rotation = rot;
            this->Walls.push_back(obj);

        }
    }
    // exit(0);
    // this->Walls.push_back(GameObject(pos2, size2, ResourceManager::GetTexture("block")));
}