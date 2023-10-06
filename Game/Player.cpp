#include <Player.hpp>

#include <Graphics/Input.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/ResourceManager.hpp>
#include <Math/Camera2D.hpp>
#include <Graphics/MouseStateTracker.hpp>
#include <Graphics/MouseState.hpp>
#include <Graphics/Mouse.hpp>
#include <Graphics/KeyboardStateTracker.hpp>
#include <Graphics/KeyboardState.hpp>
#include <Graphics/Keyboard.hpp>

#include <map>
#include <string>
#include <iostream>


using namespace Graphics;
using namespace Math;
using namespace std;

static std::map<Player::State, std::string> g_StateString = {
    {Player::State::None, "None"},
    {Player::State::Idle, "Idle"},
    {Player::State::Running, "Running"},
    {Player::State::Attack, "Attack"}
};

Player::Player() = default;

Player::Player(const glm::vec2& pos)
    : Entity{ pos, AABB{{18, 10, 0}, {36, 43, 0}} }
{
    auto idle_sprites = ResourceManager::loadSpriteSheet("assets/Warrior/SpriteSheet/Warrior_SheetnoEffect.png", 64, 44, 0, 0, BlendMode::AlphaBlend);
    IdleAnim = SpriteAnim(idle_sprites, 12, { {0, 1, 2, 3, 4, 5} });
    RunAnim = SpriteAnim(idle_sprites, 12, { {6, 7, 8, 9, 10, 11} });
    AttackAnim = SpriteAnim(idle_sprites, 12, { {14, 15, 16, 17, 18, 19, 20, 21, 22} });

    setState(State::Idle);
    transform.setAnchor({ 25,43 });
}



void Player::update(float deltaTime)
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
    }

    // How to detect direction of player?
    if (velocity.x < 0.0f)
    {
        transform.setScale({ -1, 1 });
    }
    else if (velocity.x > 0.0f)
    {
        transform.setScale({ 1, 1 });
    }
}

void Player::draw(Graphics::Image& image, const Camera2D& camera)
{
    switch (state)
    {
    case State::Idle:
        image.drawSprite(IdleAnim, camera * transform);
        break;
    case State::Running:
        image.drawSprite(RunAnim, camera * transform);
        break;
    case State::Attack:
        image.drawSprite(AttackAnim, camera * transform);
        break;
    }
#if _DEBUG
    image.drawAABB(camera * getAABB(), Color::Yellow, {}, FillMode::WireFrame);
    auto pos = camera * transform;
    image.drawText(Font::Default, g_StateString[state], pos[2][0], pos[2][1], Color::Black);
#endif
}


void Player::doMovement(float deltaTime)
{
    auto initialPos = transform.getPosition();
    auto newPos = initialPos;

    newPos.x += Input::getAxis("Horizontal") * speed * deltaTime;
    newPos.y -= Input::getAxis("Vertical") * speed * deltaTime;

    velocity = (newPos - initialPos) / deltaTime;

    transform.setPosition(newPos);
}

void Player::doIdle(float deltaTime)
{
    doMovement(deltaTime);

    if (glm::length(velocity) > 0)
    {
        setState(State::Running);
    }

    IdleAnim.update(deltaTime);
}

void Player::doRunning(float deltaTime)
{
    doMovement(deltaTime);

    if (glm::length(velocity) == 0.0f)
    {
        setState(State::Idle);
    }

    RunAnim.update(deltaTime);
}

void Player::doAttack(float deltaTime)
{
    doMovement(deltaTime);

    if (Input::getKeyDown(""))
    {
        setState(State::Attack);
    }

    AttackAnim.update(deltaTime);
}



void Player::setState(State newState)
{
    if (newState != state)
    {
        switch (newState)
        {
        case State::Idle:
            break;
        case State::Running:
            break;
        case State::Attack:
            break;
        }
        state = newState;
    }
}




