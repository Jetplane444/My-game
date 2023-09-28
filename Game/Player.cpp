#include <Player.hpp>

#include <Graphics/Input.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/ResourceManager.hpp>
#include <Math/Camera2D.hpp>

using namespace Graphics;
using namespace Math;

Player::Player() = default;

Player::Player(const glm::vec2& pos)
    : transform{ pos }
    , aabb{ {8, 16, 0}, {24, 38, 0} }
{
    auto idle_sprites = ResourceManager::loadSpriteSheet("assets/Warrior/SpriteSheet/Warrior_SheetnoEffect.png", 64, 44, 0, 0, BlendMode::AlphaBlend);
    IdleAnim = SpriteAnim(idle_sprites, 12, { {0, 1, 2, 3, 4, 5} });
    RunAnim = SpriteAnim(idle_sprites, 12, { {6, 7, 8, 9, 10, 11} });
}

void Player::update(float deltaTime)
{
    auto initialPos = transform.getPosition();
    auto newPos = initialPos;

    newPos.x += Input::getAxis("Horizontal") * speed * deltaTime;
    newPos.y -= Input::getAxis("Vertical") * speed * deltaTime;

    velocity = (newPos - initialPos) / deltaTime;

    transform.setPosition(newPos);

    if (glm::length(velocity) > 0)
    {
        setState(State::Running);
    }
    else
    {
        setState(State::Idle);
    }

    IdleAnim.update(deltaTime);
    RunAnim.update(deltaTime);
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
    }
#if _DEBUG
    image.drawAABB(getAABB(), Color::Yellow, {}, FillMode::WireFrame);
    auto pos = camera * transform;
    image.drawText(Font::Default, "State...", pos[2][0], pos[2][1], Color::Black);
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
            break;
        case State::Running:
            break;
        }
        state = newState;
    }
}




