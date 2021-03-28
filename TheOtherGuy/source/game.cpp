/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include <algorithm>
#include <sstream>
#include <iostream>

// #include <learnopengl/filesystem.h>

// #include <irrklang/irrKlang.h>
// using namespace irrklang;

#include "game.h"
#include "maze.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "power_up.h"
// #include "ball_object.h"
// #include "particle_generator.h"
// #include "post_processor.h"
#include "text_renderer.h"


// Game-related State data
SpriteRenderer    *Renderer;
GameObject        *Player;
Maze              *maze;
// PowerUp                 *powerups;
// BallObject        *Ball;
// ParticleGenerator *Particles;
// PostProcessor     *Effects;
// ISoundEngine      *SoundEngine = createIrrKlangDevice();
TextRenderer      *Text;

float ShakeTime = 0.0f;


Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), KeysProcessed(), Width(width), Height(height), Level(0), Lives(3)
{ 

}

Game::~Game()
{
    delete Renderer;
    delete Player;
    delete maze;
    // delete Ball;
    // delete Particles;
    // delete Effects;
    // delete Text;
    // SoundEngine->drop();
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("../source/sprite.vs", "../source/sprite.fs", nullptr, "sprite");

    // // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    ResourceManager::LoadTexture("../assets/textures/block.png", false, "block");
    ResourceManager::LoadTexture("../assets/textures/grey.jpg", false, "grey");
    ResourceManager::LoadTexture("../assets/textures/among_us_0.png", true, "player_1");
    ResourceManager::LoadTexture("../assets/textures/coin1.png", true, "coin");
    ResourceManager::LoadTexture("../assets/textures/star2.png", true, "star");
    ResourceManager::LoadTexture("../assets/textures/door.png", true, "door");
    ResourceManager::LoadTexture("../assets/textures/button3.png", true, "button");
    ResourceManager::LoadTexture("../assets/textures/danger1.png", true, "danger");

    // // set render-specific controls
    Shader sh = Shader(ResourceManager::GetShader("sprite"));
    Renderer = new SpriteRenderer(sh);
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("../assets/fonts/OCRAEXT.TTF", 24);

    // // load maze
    glm::vec2 mazePos = glm::vec2(CENTER.x, CENTER.y);
    maze =  new Maze(mazePos);
    maze->init();

    glm::vec2 exitPos = maze->exitPos, but1 = maze->powerUp1Pos, but2 = maze->powerUp2Pos;
    PowerUp *p1 = new PowerUp(exitPos, 0);
    PowerUp *p2 = new PowerUp(but1, 1);
    PowerUp *p3 = new PowerUp(but2, 2);
    p1->init();
    p2->init();
    p3->init();
    powerups.push_back(p1);
    powerups.push_back(p2);
    powerups.push_back(p3);

    // configure game objects
    glm::vec2 playerPos = glm::vec2(CENTER.x, CENTER.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("player_1"));
    tasks = 0;

}


bool CheckCollision(GameObject one, GameObject two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x + one.Offset.x >= two.Position.x + two.Offset.x &&
        two.Position.x + two.Size.x + two.Offset.x >= one.Position.x + one.Offset.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y + one.Offset.y >= two.Position.y + two.Offset.y &&
        two.Position.y + two.Size.y + two.Offset.y >= one.Position.y + one.Offset.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

void Game::Update(float dt)
{
    int flagg = 0;
    std::vector<int> todel;
    int cnt=0;
    for(auto pp : powerups)
    {
        if(CheckCollision(*Player,pp->Walls[0]))
        {
            if(tasks==2 && pp->ObjectType==0)
            {
                this->State = GAME_WIN;
                exit(0);
                return;
            }
            if(pp->ObjectType==1)
            {
                tasks++;
            }
            if(pp->ObjectType==2)
            {
                flagg=1;
                tasks++;
            }
            if(pp->ObjectType>2)
            {
                this->Score += (pp->ObjectType==3)?50:-50;
            }
            if(pp->ObjectType!=0)
                todel.push_back(cnt);
        }
        cnt++;
    }
    for(int i=0;i<todel.size();i++)
    {
        int pp = todel[i];
        delete(powerups[pp]);
        powerups.erase(powerups.begin()+pp);
    }
    if(flagg==1)
    {
        glm::vec2 pos1 = glm::vec2(maze->availableRooms[0].first + maze->Position.x,maze->availableRooms[0].second + maze->Position.y);
        glm::vec2 pos2 = glm::vec2(maze->availableRooms[1].first + maze->Position.x,maze->availableRooms[1].second + maze->Position.y);
        glm::vec2 pos3 = glm::vec2(maze->availableRooms[2].first + maze->Position.x,maze->availableRooms[2].second + maze->Position.y);
        PowerUp *p1 = new PowerUp(pos1, 3);
        PowerUp *p2 = new PowerUp(pos2, 3);
        PowerUp *p3 = new PowerUp(pos3, 4);
        p1->init();
        p2->init();
        p3->init();
        powerups.push_back(p1);
        powerups.push_back(p2);
        powerups.push_back(p3);
    }
    
    // // update objects
    // Ball->Move(dt, this->Width);
    // // check for collisions
    // this->DoCollisions();
    // // update particles
    // Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2.0f));
    // // update PowerUps
    // this->UpdatePowerUps(dt);
    // // reduce shake time
    // if (ShakeTime > 0.0f)
    // {
    //     ShakeTime -= dt;
    //     if (ShakeTime <= 0.0f)
    //         Effects->Shake = false;
    // }
    // // check loss condition
    // if (Ball->Position.y >= this->Height) // did ball reach bottom edge?
    // {
    //     --this->Lives;
    //     // did the player lose all his lives? : game over
    //     if (this->Lives == 0)
    //     {
    //         this->ResetLevel();
    //         this->State = GAME_MENU;
    //     }
    //     this->ResetPlayer();
    // }
    // // check win condition
    // if (this->State == GAME_ACTIVE && this->Levels[this->Level].IsCompleted())
    // {
    //     this->ResetLevel();
    //     this->ResetPlayer();
    //     Effects->Chaos = true;
    //     this->State = GAME_WIN;
    // }
}


void Game::ProcessInput(float dt)
{
    // if (this->State == GAME_MENU)
    // {
    //     if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
    //     {
    //         this->State = GAME_ACTIVE;
    //         this->KeysProcessed[GLFW_KEY_ENTER] = true;
    //     }
    //     if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W])
    //     {
    //         this->Level = (this->Level + 1) % 4;
    //         this->KeysProcessed[GLFW_KEY_W] = true;
    //     }
    //     if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S])
    //     {
    //         if (this->Level > 0)
    //             --this->Level;
    //         else
    //             this->Level = 3;
    //         //this->Level = (this->Level - 1) % 4;
    //         this->KeysProcessed[GLFW_KEY_S] = true;
    //     }
    // }
    // if (this->State == GAME_WIN)
    // {
    //     if (this->Keys[GLFW_KEY_ENTER])
    //     {
    //         this->KeysProcessed[GLFW_KEY_ENTER] = true;
    //         Effects->Chaos = false;
    //         this->State = GAME_MENU;
    //     }
    // }
    if (this->State == GAME_ACTIVE)
    {
        // std::cout<<"OK " << this->State<<" " <<GAME_ACTIVE<<" ";
        glm::vec2 curmaze = maze->Position;
        float velocity = MAZE_VELOCITY * dt;
        // move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
            if (maze->Position.x - CENTER.x<= maze->mazeSize.x && maze->checkInside(Player->Position+glm::vec2(-velocity,0.0) - maze->Position) && maze->checkInside(Player->Position+glm::vec2(-velocity,PLAYER_SIZE.y) - maze->Position))
            {
                maze->Position.x += velocity;
                // if (Ball->Stuck)
                //     Ball->Position.x -= velocity;
            }
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (maze->Position.x - CENTER.x>= -maze->mazeSize.x && maze->checkInside(Player->Position+glm::vec2(velocity + PLAYER_SIZE.x,PLAYER_SIZE.y) - maze->Position) && maze->checkInside(Player->Position+glm::vec2(velocity + PLAYER_SIZE.x,0.0) - maze->Position))
            {
                maze->Position.x -= velocity;
                // if (Ball->Stuck)
                //     Ball->Position.x += velocity;
            }
        }
        if (this->Keys[GLFW_KEY_S])
        {
            if (maze->Position.y - CENTER.y >= -maze->mazeSize.y && maze->checkInside(Player->Position+glm::vec2(PLAYER_SIZE.x,velocity+PLAYER_SIZE.y) - maze->Position) && maze->checkInside(Player->Position+glm::vec2(0.0,PLAYER_SIZE.y + velocity) - maze->Position))
            {
                maze->Position.y -= velocity;
                // if (Ball->Stuck)
                //     Ball->Position.x += velocity;
            }
        }
        if (this->Keys[GLFW_KEY_W])
        {
            if (maze->Position.y - CENTER.y <= maze->mazeSize.y && maze->checkInside(Player->Position+glm::vec2(0.0, -velocity) - maze->Position) && maze->checkInside(Player->Position+glm::vec2(PLAYER_SIZE.x,-velocity) - maze->Position))
            {
                maze->Position.y += velocity;
                // if (Ball->Stuck)
                //     Ball->Position.x += velocity;
            }
        }
        // if (this->Keys[GLFW_KEY_SPACE])
        //     Ball->Stuck = false;
        glm::vec2 new_maze = maze->Position;
        for(auto pp: powerups)
        {
            pp->Position += new_maze - curmaze;
        }
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE || this->State == GAME_MENU || this->State == GAME_WIN)
    {
        // std::cout<<"OK ";
        // // begin rendering to postprocessing framebuffer
        // Effects->BeginRender();

        // draw background
        // Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        // draw level
        // this->Levels[this->Level].Draw(*Renderer);
        // draw player
        maze->Draw(*Renderer);   

        for(auto aa: powerups)
        {
            aa->Draw(*Renderer);
        }

        Player->Draw(*Renderer);

        std::stringstream ss;
        ss << this->Score;
        Text->RenderText("Score:" + ss.str(), 80.0f, 80.0f, 1.0f);

        // std::cout<<powerups.size()<<'\n';
        // exit(0);
        // // draw PowerUps
        // for (PowerUp &powerUp : this->PowerUps)
        //     if (!powerUp.Destroyed)
        //         powerUp.Draw(*Renderer);
        // // draw particles	
        // Particles->Draw();
        // draw ball
        // Texture2D tt = ResourceManager::GetTexture("block");
        // Renderer->DrawSprite(tt, glm::vec2(100.0f, 100.0f), glm::vec2(100.0f, 100.0f), 45.0f, glm::vec3(0.0f, 0.0f, 0.0f));         
        // // end rendering to postprocessing framebuffer
        // Effects->EndRender();
        // // render postprocessing quad
        // Effects->Render(glfwGetTime());
        // // render text (don't include in postprocessing)
        // std::stringstream ss; ss << this->Lives;
        // Text->RenderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);
    }
    // if (this->State == GAME_MENU)
    // {
    //     Text->RenderText("Press ENTER to start", 250.0f, this->Height / 2.0f, 1.0f);
    //     Text->RenderText("Press W or S to select level", 245.0f, this->Height / 2.0f + 20.0f, 0.75f);
    // }
    // if (this->State == GAME_WIN)
    // {
    //     Text->RenderText("You WON!!!", 320.0f, this->Height / 2.0f - 20.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    //     Text->RenderText("Press ENTER to retry or ESC to quit", 130.0f, this->Height / 2.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    // }
}


// void Game::ResetLevel()
// {
//     if (this->Level == 0)
//         this->Levels[0].Load("levels/one.lvl", this->Width, this->Height / 2);
//     else if (this->Level == 1)
//         this->Levels[1].Load("levels/two.lvl", this->Width, this->Height / 2);
//     else if (this->Level == 2)
//         this->Levels[2].Load("levels/three.lvl", this->Width, this->Height / 2);
//     else if (this->Level == 3)
//         this->Levels[3].Load("levels/four.lvl", this->Width, this->Height / 2);

//     this->Lives = 3;
// }

// void Game::ResetPlayer()
// {
//     // reset player/ball stats
//     Player->Size = PLAYER_SIZE;
//     Player->Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
//     Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
//     // also disable all active powerups
//     Effects->Chaos = Effects->Confuse = false;
//     Ball->PassThrough = Ball->Sticky = false;
//     Player->Color = glm::vec3(1.0f);
//     Ball->Color = glm::vec3(1.0f);
// }


// // powerups
// bool IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type);

// void Game::UpdatePowerUps(float dt)
// {
//     for (PowerUp &powerUp : this->PowerUps)
//     {
//         powerUp.Position += powerUp.Velocity * dt;
//         if (powerUp.Activated)
//         {
//             powerUp.Duration -= dt;

//             if (powerUp.Duration <= 0.0f)
//             {
//                 // remove powerup from list (will later be removed)
//                 powerUp.Activated = false;
//                 // deactivate effects
//                 if (powerUp.Type == "sticky")
//                 {
//                     if (!IsOtherPowerUpActive(this->PowerUps, "sticky"))
//                     {	// only reset if no other PowerUp of type sticky is active
//                         Ball->Sticky = false;
//                         Player->Color = glm::vec3(1.0f);
//                     }
//                 }
//                 else if (powerUp.Type == "pass-through")
//                 {
//                     if (!IsOtherPowerUpActive(this->PowerUps, "pass-through"))
//                     {	// only reset if no other PowerUp of type pass-through is active
//                         Ball->PassThrough = false;
//                         Ball->Color = glm::vec3(1.0f);
//                     }
//                 }
//                 else if (powerUp.Type == "confuse")
//                 {
//                     if (!IsOtherPowerUpActive(this->PowerUps, "confuse"))
//                     {	// only reset if no other PowerUp of type confuse is active
//                         Effects->Confuse = false;
//                     }
//                 }
//                 else if (powerUp.Type == "chaos")
//                 {
//                     if (!IsOtherPowerUpActive(this->PowerUps, "chaos"))
//                     {	// only reset if no other PowerUp of type chaos is active
//                         Effects->Chaos = false;
//                     }
//                 }
//             }
//         }
//     }
//     // Remove all PowerUps from vector that are destroyed AND !activated (thus either off the map or finished)
//     // Note we use a lambda expression to remove each PowerUp which is destroyed and not activated
//     this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
//         [](const PowerUp &powerUp) { return powerUp.Destroyed && !powerUp.Activated; }
//     ), this->PowerUps.end());
// }

// bool ShouldSpawn(unsigned int chance)
// {
//     unsigned int random = rand() % chance;
//     return random == 0;
// }
// void Game::SpawnPowerUps(GameObject &block)
// {
//     if (ShouldSpawn(75)) // 1 in 75 chance
//         this->PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position, ResourceManager::GetTexture("powerup_speed")));
//     if (ShouldSpawn(75))
//         this->PowerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position, ResourceManager::GetTexture("powerup_sticky")));
//     if (ShouldSpawn(75))
//         this->PowerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position, ResourceManager::GetTexture("powerup_passthrough")));
//     if (ShouldSpawn(75))
//         this->PowerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.Position, ResourceManager::GetTexture("powerup_increase")));
//     if (ShouldSpawn(15)) // Negative powerups should spawn more often
//         this->PowerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_confuse")));
//     if (ShouldSpawn(15))
//         this->PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_chaos")));
// }

// void ActivatePowerUp(PowerUp &powerUp)
// {
//     if (powerUp.Type == "speed")
//     {
//         Ball->Velocity *= 1.2;
//     }
//     else if (powerUp.Type == "sticky")
//     {
//         Ball->Sticky = true;
//         Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
//     }
//     else if (powerUp.Type == "pass-through")
//     {
//         Ball->PassThrough = true;
//         Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
//     }
//     else if (powerUp.Type == "pad-size-increase")
//     {
//         Player->Size.x += 50;
//     }
//     else if (powerUp.Type == "confuse")
//     {
//         if (!Effects->Chaos)
//             Effects->Confuse = true; // only activate if chaos wasn't already active
//     }
//     else if (powerUp.Type == "chaos")
//     {
//         if (!Effects->Confuse)
//             Effects->Chaos = true;
//     }
// }

// bool IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type)
// {
//     // Check if another PowerUp of the same type is still active
//     // in which case we don't disable its effect (yet)
//     for (const PowerUp &powerUp : powerUps)
//     {
//         if (powerUp.Activated)
//             if (powerUp.Type == type)
//                 return true;
//     }
//     return false;
// }


// // collision detection
// bool CheckCollision(GameObject &one, GameObject &two);
// Collision CheckCollision(BallObject &one, GameObject &two);
// Direction VectorDirection(glm::vec2 closest);

// void Game::DoCollisions()
// {
//     for (GameObject &box : this->Levels[this->Level].Bricks)
//     {
//         if (!box.Destroyed)
//         {
//             Collision collision = CheckCollision(*Ball, box);
//             if (std::get<0>(collision)) // if collision is true
//             {
//                 // destroy block if not solid
//                 if (!box.IsSolid)
//                 {
//                     box.Destroyed = true;
//                     this->SpawnPowerUps(box);
//                     SoundEngine->play2D(FileSystem::getPath("resources/audio/bleep.mp3").c_str(), false);
//                 }
//                 else
//                 {   // if block is solid, enable shake effect
//                     ShakeTime = 0.05f;
//                     Effects->Shake = true;
//                     SoundEngine->play2D(FileSystem::getPath("resources/audio/bleep.mp3").c_str(), false);
//                 }
//                 // collision resolution
//                 Direction dir = std::get<1>(collision);
//                 glm::vec2 diff_vector = std::get<2>(collision);
//                 if (!(Ball->PassThrough && !box.IsSolid)) // don't do collision resolution on non-solid bricks if pass-through is activated
//                 {
//                     if (dir == LEFT || dir == RIGHT) // horizontal collision
//                     {
//                         Ball->Velocity.x = -Ball->Velocity.x; // reverse horizontal velocity
//                         // relocate
//                         float penetration = Ball->Radius - std::abs(diff_vector.x);
//                         if (dir == LEFT)
//                             Ball->Position.x += penetration; // move ball to right
//                         else
//                             Ball->Position.x -= penetration; // move ball to left;
//                     }
//                     else // vertical collision
//                     {
//                         Ball->Velocity.y = -Ball->Velocity.y; // reverse vertical velocity
//                         // relocate
//                         float penetration = Ball->Radius - std::abs(diff_vector.y);
//                         if (dir == UP)
//                             Ball->Position.y -= penetration; // move ball bback up
//                         else
//                             Ball->Position.y += penetration; // move ball back down
//                     }
//                 }
//             }
//         }    
//     }

//     // also check collisions on PowerUps and if so, activate them
//     for (PowerUp &powerUp : this->PowerUps)
//     {
//         if (!powerUp.Destroyed)
//         {
//             // first check if powerup passed bottom edge, if so: keep as inactive and destroy
//             if (powerUp.Position.y >= this->Height)
//                 powerUp.Destroyed = true;

//             if (CheckCollision(*Player, powerUp))
//             {	// collided with player, now activate powerup
//                 ActivatePowerUp(powerUp);
//                 powerUp.Destroyed = true;
//                 powerUp.Activated = true;
//                 SoundEngine->play2D(FileSystem::getPath("resources/audio/powerup.wav").c_str(), false);
//             }
//         }
//     }

//     // and finally check collisions for player pad (unless stuck)
//     Collision result = CheckCollision(*Ball, *Player);
//     if (!Ball->Stuck && std::get<0>(result))
//     {
//         // check where it hit the board, and change velocity based on where it hit the board
//         float centerBoard = Player->Position.x + Player->Size.x / 2.0f;
//         float distance = (Ball->Position.x + Ball->Radius) - centerBoard;
//         float percentage = distance / (Player->Size.x / 2.0f);
//         // then move accordingly
//         float strength = 2.0f;
//         glm::vec2 oldVelocity = Ball->Velocity;
//         Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength; 
//         //Ball->Velocity.y = -Ball->Velocity.y;
//         Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity); // keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
//         // fix sticky paddle
//         Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);

//         // if Sticky powerup is activated, also stick ball to paddle once new velocity vectors were calculated
//         Ball->Stuck = Ball->Sticky;

//         SoundEngine->play2D(FileSystem::getPath("resources/audio/bleep.wav").c_str(), false);
//     }
// }

// Collision CheckCollision(BallObject &one, GameObject &two) // AABB - Circle collision
// {
//     // get center point circle first 
//     glm::vec2 center(one.Position + one.Radius);
//     // calculate AABB info (center, half-extents)
//     glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
//     glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
//     // get difference vector between both centers
//     glm::vec2 difference = center - aabb_center;
//     glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
//     // now that we know the the clamped values, add this to AABB_center and we get the value of box closest to circle
//     glm::vec2 closest = aabb_center + clamped;
//     // now retrieve vector between center circle and closest point AABB and check if length < radius
//     difference = closest - center;
    
//     if (glm::length(difference) < one.Radius) // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
//         return std::make_tuple(true, VectorDirection(difference), difference);
//     else
//         return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
// }

// // calculates which direction a vector is facing (N,E,S or W)
// Direction VectorDirection(glm::vec2 target)
// {
//     glm::vec2 compass[] = {
//         glm::vec2(0.0f, 1.0f),	// up
//         glm::vec2(1.0f, 0.0f),	// right
//         glm::vec2(0.0f, -1.0f),	// down
//         glm::vec2(-1.0f, 0.0f)	// left
//     };
//     float max = 0.0f;
//     unsigned int best_match = -1;
//     for (unsigned int i = 0; i < 4; i++)
//     {
//         float dot_product = glm::dot(glm::normalize(target), compass[i]);
//         if (dot_product > max)
//         {
//             max = dot_product;
//             best_match = i;
//         }
//     }
//     return (Direction)best_match;
// }
