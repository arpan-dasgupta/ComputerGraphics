/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"


/// GameLevel holds all Tiles as part of a Breakout level and 
/// hosts functionality to Load/render levels from the harddisk.
class Maze
{
public:

    std::vector<GameObject> Walls;
    glm::vec2   Position;
    glm::vec2   topLeft, bottomRight;
    glm::vec2   mazeSize;

    std::vector<std::vector<int>> adjList;
    std::vector<std::vector<std::pair<int,float>>> adjListWeighted;
    std::vector<std::pair<float,float>> vertexPositions;
    std::vector<std::pair<int,int>> edges;
    glm::vec2   playerInitPos;
    std::vector<glm::vec2> roomCoords, pathCoords;

    // constructor
    Maze() { }
    Maze(glm::vec2 pos) { Position = pos; }
    // loads level from file
    void Load(const char *file, unsigned int levelWidth, unsigned int levelHeight);
    // render level
    void Draw(SpriteRenderer &renderer);
    // check if the level is completed (all non-solid tiles are destroyed)
    bool IsCompleted();
    void init();
    int checkInside(glm::vec2 point);
private:
    glm::vec4 get_corners(glm::vec2 position, float rotate, glm::vec2 size, int room);
    // initialize level from tile data
    // void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};

#endif