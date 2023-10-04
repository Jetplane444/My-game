#include <Player.hpp>

#include <Graphics/Input.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/ResourceManager.hpp>
#include <Math/Camera2D.hpp>

#include <map>
#include <string>

using namespace Graphics;
using namespace Math;

static std::map<Player::State, std::string> g_stateMap = {
    {Player::State::None, "None"},
    {Player::State::Idle, "Idle"},
    {Player::State::Running, "Running"},
    {Player::State::Attack, "Attack"},
    {Player::State::Dead, "Dead"},
};

Player::Player() = default;

Player::Player(const glm::vec2& pos)
    : transform{ pos }
    , aabb{ {18, 10, 0}, {36, 43, 0} }
{
    auto idle_sprites = ResourceManager::loadSpriteSheet("assets/Warrior/SpriteSheet/Warrior_SheetnoEffect.png", 64, 44, 0, 0, BlendMode::AlphaBlend);
    IdleAnim = SpriteAnim(idle_sprites, 12, { {0, 1, 2, 3, 4, 5} });
    RunningAnim = SpriteAnim(idle_sprites, 12, { {6, 7, 8, 9, 10, 11} });

    setState(State::Idle);
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
    }
}

void Player::draw(Graphics::Image& image, const glm::mat3& transform)
{
    // Translation matrix.
    // Use a Transform2D instead!
    glm::mat3 t = {
        1, 0, 0,
        0, 1, 0,
        position.x, position.y, 1
    };

    t = transform * t;

    switch (state)
    {
    case State::Idle:
        image.drawSprite(IdleAnim, t);
        break;
    case State::Running:
        image.drawSprite(RunningAnim, t);
        break;
    }
#if _DEBUG
    image.drawAABB(transform * getAABB(), Color::Yellow, {}, FillMode::WireFrame);
    image.drawText(Font::Default, g_stateMap[state], t[2][0], t[2][1], Color::Black);
#endif
}

void Player::setPosition(const glm::vec2& pos)
{
    transform.setPosition(pos);
}

const glm::vec2& Player::getPosition() const
{
    return transform.getPosition();
}

void Player::translate(const glm::vec2& t)
{
    transform.translate(t);
}

const Math::AABB Player::getAABB() const
{
    return transform * aabb;
}

void Player::setState(State newState)
{
    if (newState != state)
    {
        switch (newState)
        {
        case State::Idle:
            // Do stuff when transition to the new state
            break;
        case State::Running:
            break;
        }
        state = newState;
    }
}

void Player::doMovement(float deltaTime)
{
    auto initialPos = position;
    position.x += Input::getAxis("Horisontal") * speed * deltaTime;
    position.y -= Input::getAxis("Horisontal") * speed * deltaTime;

    velocity = (position - initialPos) / deltaTime;
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

    RunningAnim.update(deltaTime);
}




