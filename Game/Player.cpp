#include <Player.hpp>

#include <Graphics/Input.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/ResourceManager.hpp>

using namespace Graphics;

Player::Player() = default;

Player::Player(const glm::vec2& pos)
    : position{ pos }
    , aabb{ {8, 16, 0}, {24, 38, 0} }
{
    auto idle_sprites = ResourceManager::loadSpriteSheet("assets/Warrior/SpriteSheet/Warrior_SheetnoEffect.png", 64, 44, 0, 0, BlendMode::AlphaBlend);
    IdleAnim = SpriteAnim(idle_sprites, 12, { {0, 1, 2, 3, 4, 5} });
    RunAnim = SpriteAnim(idle_sprites, 12, { {6, 7, 8, 9, 10, 11} });
}

void Player::update(float deltaTime)
{
    auto initialPos = position;

    position.x += Input::getAxis("Horizontal") * speed * deltaTime;
    position.y -= Input::getAxis("Vertical") * speed * deltaTime;

    velocity = (position - initialPos) / deltaTime;

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

void Player::draw(Graphics::Image& image)
{
    switch (state)
    {
    case State::Idle:
        image.drawSprite(IdleAnim, position.x, position.y);
        break;
    case State::Running:
        image.drawSprite(RunAnim, position.x, position.y);
        break;
    }
#if _DEBUG
    image.drawAABB(getAABB(), Color::Yellow, {}, FillMode::WireFrame);
    image.drawText(Font::Default, "State...", position.x, position.y, Color::Black);
#endif
}

void Player::setPosition(const glm::vec2& pos)
{
    position = pos;
}

const glm::vec2& Player::getPosition() const
{
    return position;
}

void Player::translate(const glm::vec2& t)
{
    position += t;
}

const Math::AABB Player::getAABB() const
{
    return aabb + glm::vec3{ position, 0 };
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




