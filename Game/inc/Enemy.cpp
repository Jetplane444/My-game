#include "Enemy.hpp"

#include <Graphics/Input.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/ResourceManager.hpp>
#include <Math/Camera2D.hpp>

#include <map>
#include <string>
#include <iostream>

using namespace Graphics;
using namespace Math;
using namespace std;

static std::map<Enemy::State, std::string> g_StateString = {
    {Enemy::State::None, "None"},
    {Enemy::State::Idle, "Idle"},
    {Enemy::State::Running, "Running"},
    {Enemy::State::Attack, "Attack"},
    {Enemy::State::Dead, "Dead"}
};

Enemy::Enemy() = default;

Enemy::Enemy(const glm::vec2& pos)
    : Entity{ pos, { {8, 16, 0}, {24, 38, 0} } }
{
    auto idle_sprites = ResourceManager::loadSpriteSheet("assets/Spirit Boxer/Idle.png", 137, 44, 0, 0, BlendMode::AlphaBlend);
    auto run_sprites = ResourceManager::loadSpriteSheet("assets/Spirit Boxer/Run.png", 137, 44, 0, 0, BlendMode::AlphaBlend);
    auto attack_sprites = ResourceManager::loadSpriteSheet("assets/Spirit Boxer/attack 1.png", 137, 44, 0, 0, BlendMode::AlphaBlend);
    auto death_sprites = ResourceManager::loadSpriteSheet("assets/Spirit Boxer/Damaged & Death.png", 137, 44, 0, 0, BlendMode::AlphaBlend);
    idleAnim = SpriteAnim{ idle_sprites, 6 };
    runAnim = SpriteAnim{ run_sprites, 6 };
    attackAnim = SpriteAnim{ attack_sprites, 6 };
    deathAnim = SpriteAnim{ death_sprites, 6 };

    setState(State::Idle);
    transform.setAnchor({ 17, 44 });
}




void Enemy::update(float deltaTime)
{
    switch (state)
    {
    case State::Idle:
        doIdle(deltaTime);
        break;
    case State::Running:
        doRunning(deltaTime);
        break;
    case State::Attack:
        doAttack(deltaTime);
        break;
    case State::Dead:
        doDie(deltaTime);
        break;
    }


    if (velocity.x < 0.0f)
    {
        transform.setScale({ -1, 1 });
    }
    else if (velocity.x > 0.0f)
    {
        transform.setScale({ 1, 1 });
    }
}


void Enemy::draw(Graphics::Image& image, const Math::Camera2D& camera)
{
    switch (state)
    {
    case State::Idle:
        image.drawSprite(idleAnim, camera * transform);
        break;
    case State::Running:
        image.drawSprite(runAnim, camera * transform);
        break;
    case State::Attack:
        image.drawSprite(attackAnim, camera * transform);
        break;
    case State::Dead:
        image.drawSprite(deathAnim, camera * transform);
    }
#if _DEBUG
    image.drawAABB(camera * getAABB(), Color::Yellow, {}, FillMode::WireFrame);
    auto pos = camera * transform;
    image.drawText(Font::Default, g_StateString[state], pos[2][0], pos[2][1], Color::Black);
    if(target)
        image.drawCircle(Math::Circle(camera.transformPoint(target->getPosition()), 5), Color::Red);
#endif
}

void Enemy::setTarget(Entity* target)
{
    this->target = target;
}

void Enemy::doMovement(float deltaTime)
{
 auto initialPos = transform.getPosition();
    auto targetPos = target ? target->getPosition() : initialPos;

    auto direction = targetPos - initialPos;
    auto length = glm::length(direction);

    direction = glm::length(direction) > 0 ? glm::normalize(direction) : direction ;
    velocity = direction * speed;
    initialPos += velocity * deltaTime;
  
    transform.setPosition(initialPos);
}

void Enemy::doIdle(float deltaTime)
{
    doMovement(deltaTime);

    if (glm::length(velocity) > 0)
    {
        setState(State::Running);
    }
    
    idleAnim.update(deltaTime);
}

void Enemy::doRunning(float deltaTime)
{
    doMovement(deltaTime);

    if (glm::length(velocity) == 0.0f)
    {
        setState(State::Idle);
    }

    if (target)
    {
        auto distance = glm::distance(getPosition(), target->getPosition());
        if (distance <= 15)
        {
            setState(State::Attack);
        }
    }

    runAnim.update(deltaTime);
}

void Enemy::doAttack(float deltaTime)
{
    //Update the attack animation.
    attackAnim.update(deltaTime);

    //Check if the animation is done playing.
    if (attackAnim.isDone())
    {
        // Switch back to idle
        setState(State::Idle);
    }
}

void Enemy::doDie(float deltaTime)
{
    
}


void Enemy::setState(State newState)
{
    if (newState != state)
    {
        switch (newState)
        {
        case State::Idle:
            idleAnim.reset();
            break;
        case State::Running:
            runAnim.reset();
            break;
        case State::Attack:
            attackAnim.reset();
            break;
        case State::Dead:
            break;
        }
        state = newState;
    }
}


