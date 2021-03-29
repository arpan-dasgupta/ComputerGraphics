#include <algorithm>
#include <sstream>
#include <iostream>

#include "game.h"
#include "maze.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "power_up.h"
#include "text_renderer.h"


// Game-related State data
SpriteRenderer    *Renderer;
// GameObject        *Player;
GameCharacter         *Player, *Enemy;
Maze              *maze;
// PowerUp                 *powerups;
// BallObject        *Ball;
TextRenderer      *Text;


Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), KeysProcessed(), Width(width), Height(height), Level(0), Lives(3)
{ 

}

Game::~Game()
{
    delete Renderer;
    delete Player;
    delete maze;
    delete Enemy;
    delete Text;
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
    ResourceManager::LoadTexture("../assets/textures/among_us_1.png", true, "player_2");
    ResourceManager::LoadTexture("../assets/textures/among_us_2.png", true, "player_3");
    ResourceManager::LoadTexture("../assets/textures/among_us_3.png", true, "player_4");
    ResourceManager::LoadTexture("../assets/textures/among_us_4.png", true, "player_5");
    ResourceManager::LoadTexture("../assets/textures/amongus_3.png", true, "enemy");
    ResourceManager::LoadTexture("../assets/textures/coin3.png", true, "coin");
    ResourceManager::LoadTexture("../assets/textures/star2.png", true, "star");
    ResourceManager::LoadTexture("../assets/textures/door.png", true, "door");
    ResourceManager::LoadTexture("../assets/textures/button4.png", true, "button");
    ResourceManager::LoadTexture("../assets/textures/danger2.png", true, "danger");

    // // set render-specific controls
    Shader sh = Shader(ResourceManager::GetShader("sprite"));
    Renderer = new SpriteRenderer(sh);
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("../assets/fonts/OCRAEXT.TTF", 24);

    // // load maze
    glm::vec2 mazePos = glm::vec2(CENTER.x, CENTER.y);
    maze =  new Maze(mazePos);
    maze->init();
    MazeInitPos = maze->Position;

    // configure game objects
    glm::vec2 playerPos = glm::vec2(CENTER.x, CENTER.y);
    // Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("player_1"));
    Player = new GameCharacter(playerPos, 0);
    Player->init();
    // exit(0);
    // std::cout<<powerups.size()<<'\n';

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
    // std::cout<<p1->Walls[0].Position.x<<" "<<p1->Walls[0].Position.y<<" "<<p1->Walls[0].Offset.x<<" "<<p1->Walls[0].Offset.y<<" "<<p1->Walls[0].Size.x<<" "<<p1->Walls[0].Size.y<<'\n';

    Enemy = new GameCharacter(exitPos, 1);
    Enemy->init();
    
    tasks = 0;
    this->Health = 100;
    this->Score = 0;
    this->Lights = 1;
    this->Time = 200; 
}


bool CheckCollision(GameObject one, GameObject two) // AABB - AABB collision
{
    // std::cout<<one.Position.x<<" "<<one.Position.y<<" "<<one.Offset.x<<" "<<one.Offset.y<<" "<<one.Size.x<<" "<<one.Size.y<<'\n';
    // std::cout<<two.Position.x<<" "<<two.Position.y<<" "<<two.Offset.x<<" "<<two.Offset.y<<" "<<two.Size.x<<" "<<two.Size.y<<'\n';
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
    if(this->State != GAME_ACTIVE)
        return;
    int flagg = 0;
    std::vector<int> todel;
    int cnt=0;
    for(auto pp : powerups)
    {
        // if(CheckCollision(*Player,pp->Walls[0]))
        // std::cout<<pp->Walls[0].Position.x<<" "<<pp->Walls[0].Position.y<<" "<<pp->Walls[0].Offset.x<<" "<<pp->Walls[0].Offset.y<<" "<<pp->Walls[0].Size.x<<" "<<pp->Walls[0].Size.y<<'\n';
        if(CheckCollision(Player->Walls[0],pp->Walls[0]))
        {
        // exit(0);
        // std::cout<<"OK\n";
            if(tasks==2 && pp->ObjectType==0)
            {
                this->State = GAME_WIN;
                return;
            }
            if(pp->ObjectType==1)
            {
                Enemy->Position = maze->Position + maze->powerUp2Pos - this->MazeInitPos;
                // std::cout<<Enemy->Position.x<<" "<<Enemy->Position.y<<'\n';
                // std::cout<<maze->Position.x<<" "<<maze->Position.y<<'\n';
                // std::cout<<maze->powerUp1Pos.x<<" "<<maze->powerUp1Pos.y<<'\n';
                // std::cout<<CENTER.x<<" "<<CENTER.y<<'\n';
                // std::cout<<powerups[2]->Position.x<<" "<<powerups[2]->Position.y<<'\n';
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
                if(pp->ObjectType==4)  
                    this->Health -= 50;
            }
            if(pp->ObjectType!=0)
                todel.push_back(cnt);
        }
        cnt++;
    }
    // std::cout<<"OKK\n";
    for(int i=0;i<todel.size();i++)
    {
        // std::cout<<todel[i]<<'\n';
        int pp = todel[i];
        delete(powerups[pp]);
        powerups.erase(powerups.begin()+pp);
    }
    // std::cout<<"OKKK\n";
    if(flagg==1)
    {
        glm::vec2 pos1 = glm::vec2(maze->availableRooms[0].first + maze->Position.x,maze->availableRooms[0].second + maze->Position.y);
        glm::vec2 pos2 = glm::vec2(maze->availableRooms[1].first + maze->Position.x,maze->availableRooms[1].second + maze->Position.y);
        glm::vec2 pos3 = glm::vec2(maze->availableRooms[2].first + maze->Position.x,maze->availableRooms[2].second + maze->Position.y);
        glm::vec2 pos4 = glm::vec2(maze->availableRooms[3].first + maze->Position.x,maze->availableRooms[3].second + maze->Position.y);
        PowerUp *p1 = new PowerUp(pos1, 3);
        PowerUp *p2 = new PowerUp(pos2, 3);
        PowerUp *p3 = new PowerUp(pos3, 4);
        PowerUp *p4 = new PowerUp(pos4, 4);
        p1->init();
        p2->init();
        p3->init();
        p4->init();
        powerups.push_back(p1);
        powerups.push_back(p2);
        powerups.push_back(p3);
        powerups.push_back(p4);
    }
    this->Time--;
    if(this->Health<=0)
    {
        this->State = GAME_LOSS;
        return;
    }
    if(CheckCollision(Player->Walls[0], Enemy->Walls[0]))
    {
        this->State = GAME_LOSS;
        return;
    }
    Player->moved = std::max(Player->moved-1,0);
    if(Player->moved==0)
        Player->active = false;
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
    if (this->State == GAME_WIN || this->State == GAME_LOSS)
    {
        if (this->Keys[GLFW_KEY_ENTER])
        {
            exit(0);
        }
    }
    if (this->State == GAME_ACTIVE)
    {
        // std::cout<<"OK " << this->State<<" " <<GAME_ACTIVE<<" ";
        glm::vec2 curmaze = maze->Position;
        float velocity = MAZE_VELOCITY * dt;
        bool didMove=0;
        // move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
            didMove = 1;
            if (maze->Position.x - CENTER.x<= maze->mazeSize.x && maze->checkInside(Player->Position+glm::vec2(-velocity,0.0) - maze->Position) && maze->checkInside(Player->Position+glm::vec2(-velocity,PLAYER_SIZE.y) - maze->Position))
            {
                maze->Position.x += velocity;
            }
        }
        if (this->Keys[GLFW_KEY_D])
        {
            didMove = 1;
            if (maze->Position.x - CENTER.x>= -maze->mazeSize.x && maze->checkInside(Player->Position+glm::vec2(velocity + PLAYER_SIZE.x,PLAYER_SIZE.y) - maze->Position) && maze->checkInside(Player->Position+glm::vec2(velocity + PLAYER_SIZE.x,0.0) - maze->Position))
            {
                maze->Position.x -= velocity;
            }
        }
        if (this->Keys[GLFW_KEY_S])
        {
            didMove = 1;
            if (maze->Position.y - CENTER.y >= -maze->mazeSize.y && maze->checkInside(Player->Position+glm::vec2(PLAYER_SIZE.x,velocity+PLAYER_SIZE.y) - maze->Position) && maze->checkInside(Player->Position+glm::vec2(0.0,PLAYER_SIZE.y + velocity) - maze->Position))
            {
                maze->Position.y -= velocity;
            }
        }
        if (this->Keys[GLFW_KEY_W])
        {
            didMove = 1;
            if (maze->Position.y - CENTER.y <= maze->mazeSize.y && maze->checkInside(Player->Position+glm::vec2(0.0, -velocity) - maze->Position) && maze->checkInside(Player->Position+glm::vec2(PLAYER_SIZE.x,-velocity) - maze->Position))
            {
                maze->Position.y += velocity;
            }
        }
        if(didMove)
        {
            Player->moved = 15;
            Player->active = true;
        }
        // if (this->Keys[GLFW_KEY_SPACE])
        //     Ball->Stuck = false;
        glm::vec2 new_maze = maze->Position;
        for(auto pp: powerups)
        {
            pp->Position += new_maze - curmaze;
        }
        Enemy->Position += new_maze - curmaze;
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE || this->State == GAME_MENU)
    {
        maze->Draw(*Renderer);   

        for(auto aa: powerups)
        {
            aa->Draw(*Renderer);
        }

        Player->Draw(*Renderer);
        Enemy->Draw(*Renderer);

        std::stringstream ss1, ss2, ss3, ss4;
        ss1 << this->Health;
        Text->RenderText("Health: " + ss1.str(), 20.0f, 20.0f, 1.0f);
        ss2 << this->Score;
        Text->RenderText("Score: " + ss2.str(), 20.0f, 50.0f, 1.0f);
        ss3 << (this->Lights)?"On":"Off";
        Text->RenderText((this->Lights)?"Lights: On":"Lights: Off", 20.0f, 80.0f, 1.0f);
        ss4 << this->Score;
        Text->RenderText("Time: " + ss4.str(), 20.0f, 110.0f, 1.0f);
    }
    // if (this->State == GAME_MENU)
    // {
    //     Text->RenderText("Press ENTER to start", 250.0f, this->Height / 2.0f, 1.0f);
    //     Text->RenderText("Press W or S to select level", 245.0f, this->Height / 2.0f + 20.0f, 0.75f);
    // }
    if (this->State == GAME_WIN)
    {
        Text->RenderText("You WON!!!", 320.0f, this->Height / 2.0f - 30.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        Text->RenderText("Press ESC to quit", 250.0f, this->Height / 2.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    }
    if (this->State == GAME_LOSS)
    {
        Text->RenderText("You LOST!!!", 320.0f, this->Height / 2.0f - 30.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        Text->RenderText("Press ESC to quit", 250.0f, this->Height / 2.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    }
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
