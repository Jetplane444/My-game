#include "LDtkLoader/Project.hpp"
#include "Player.hpp"

#include "Level.hpp"

#include <Graphics/BlendMode.hpp>
#include <Graphics/Color.hpp>
#include <Graphics/ResourceManager.hpp>

#include <numbers>
#include <random>

static std::random_device rd;
static std::minstd_rand   rng(rd());

using namespace Math;
using namespace Graphics;


Level::Level(const ldtk::Project& project, const ldtk::World& world, const ldtk::Level& level)
    : world{ &world }
    , level{ &level }
    , levelName{ level.name }
{
    const std::filesystem::path projectPath = project.getFilePath().directory();


    // Parse collisions.
    const auto& entities = level.getLayer("Entities");
    const auto& collisions = entities.getEntitiesByName("Collision");
    for (auto& collision : collisions)
    {
        auto& e = collision.get();
        auto& p = e.getPosition();
        auto& s = e.getSize();

        Collider collider{
            .type = ColliderType::Default,
            .aabb = AABB { { p.x, p.y, 0.0 }, { p.x + s.x - 1, p.y + s.y - 1, 0.0f } },
        };

        colliders.push_back(collider);
    }



    // Parse the level tile map.
    {
        const auto& tilesLayer = level.getLayer("Tiles");
        const auto& intGrid = level.getLayer("IntGrid");  // TODO: Should probably rename this layer..

        const auto& gridSize = tilesLayer.getGridSize();
        const auto& tileSet = tilesLayer.getTileset();
    }

}

void Level::update(float deltaTime)
{
  updateCollisions(deltaTime);
  updateEffects(deltaTime);
  updateBoxes(deltaTime);
}

void Level::updateCollisions(float deltaTime)
{
    glm::vec2 prevPos = player.getPosition();

    player.update(deltaTime);

    // CHeck the player collision
    AABB playerAABB = player.getAABB();

    glm::vec2 vel = player.getVelocity();
    glm::vec2 pos = player.getPosition();

    const float padding = 3.0f;

    for (auto& collider : colliders)
    {
        AABB colliderAABB = collider.aabb;

        // if idle or running
        {
           // Check to see if the player is colliding with the top edge of the collider.
            Line topEdge{ { colliderAABB.min.x + padding, colliderAABB.min.y, 0 }, { colliderAABB.max.x - padding, colliderAABB.min.y, 0 } };
            if (playerAABB.intersect(topEdge))
            {
                // Set the player's position to the left edge of the collider.
                pos.x = colliderAABB.min.x - playerAABB.width() * 0.5f;
                // And 0 the X velocity.
                vel.x = 0.0f;
            }
        }

        // Check to see if the player is colliding with the left edge of the collider.
        Line leftEdge{ { colliderAABB.min.x, colliderAABB.min.y + padding, 0 }, { colliderAABB.min.x, colliderAABB.max.y - padding, 0 } };
        if (!collider.isOneWay && playerAABB.intersect(leftEdge))
        {
            // Set the player's position to the left edge of the collider.
            pos.x = colliderAABB.min.x - playerAABB.width() * 0.5f;
            // And 0 the X velocity.
            vel.x = 0.0f;

            continue;
        }
        // Check to see if the player is colliding with the right edge of the collider.
        Line rightEdge{ { colliderAABB.max.x, colliderAABB.min.y + padding, 0 }, { colliderAABB.max.x, colliderAABB.max.y - padding, 0 } };
        if (!collider.isOneWay && playerAABB.intersect(rightEdge))
        {
            // Set the player's position to the right edge of the collider.
            pos.x = colliderAABB.max.x + playerAABB.width() * 0.5f;
            // And 0 the X velocity.
            vel.x = 0.0f;

            continue;
        }
    }
}

void Level::updateEffects(float deltaTime)
{
    for (auto iter = effects.begin(); iter != effects.end(); )
    {
        iter->update(deltaTime);
        if (iter->isDone())
        {
            iter = effects.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void Level::updateBoxes(float deltaTime)
{
    glm::vec2 vel = player.getVelocity();
    glm::vec2 pos = player.getPosition();
    AABB      playerAABB = player.getAABB();

    for (auto iter = boxes.begin(); iter != boxes.end(); ++iter)
    {
        auto& box = *iter;

        AABB boxCollider = box->getAABB();

        {
            // Check to see if the player is colliding with the top edge of the collider.
            Line topEdge{ { boxCollider.min.x , boxCollider.min.y, 0 }, { boxCollider.max.x , boxCollider.min.y, 0 } };
            if (playerAABB.intersect(topEdge))
            {
                // Set the player's position to the top edge of the box.
                pos.x = boxCollider.min.x - playerAABB.width() * 0.5f;
                // And 0 the X velocity.
                vel.x = 0.0f;

                continue;
            }
        }

        // Check to see if the player is colliding with the left edge of the box.
        Line leftEdge{ { boxCollider.min.x, boxCollider.min.y, 0 }, { boxCollider.min.x, boxCollider.max.y, 0 } };
        if (playerAABB.intersect(leftEdge))
        {
            // Set the player's position to the left edge of the box.
            pos.x = boxCollider.min.x - playerAABB.width() * 0.5f;
            // And 0 the X velocity.
            vel.x = 0.0f;

            continue;
        }

        // Check to see if the player is colliding with the right edge of the collider.
        Line rightEdge{ { boxCollider.max.x, boxCollider.min.y, 0 }, { boxCollider.max.x, boxCollider.max.y, 0 } };
        if (playerAABB.intersect(rightEdge))
        {
            // Set the player's position to the right edge of the collider.
            pos.x = boxCollider.max.x + playerAABB.width() * 0.5f;
            // And 0 the X velocity.
            vel.x = 0.0f;

            continue;
        }

        // Check to see if the player is colliding with the bottom edge of the collider.
        Line bottomEdge{ { boxCollider.max.x, boxCollider.max.y, 0 }, { boxCollider.max.x, boxCollider.max.y, 0 } };
        if (playerAABB.intersect(rightEdge))
        {
            // Set the player's position to the right edge of the collider.
            pos.x = boxCollider.max.x + playerAABB.width() * 0.5f;
            // And 0 the X velocity.
            vel.x = 0.0f;

            continue;
        }
    }

    // Update the boxes.
    for (auto iter = boxes.begin(); iter != boxes.end(); )
    {
        auto& box = *iter;

        // Update the box.
        box->update(deltaTime);
    }
}


void Level::reset()
{
    player.setPosition(playerStart);
    player.setVelocity({ 0, 0 });
}

void Level::draw(Graphics::Image& image) const
{
    tileMap.draw(image);

    for (auto& box : boxes)
    {
        box->draw(image);
    }

#if _DEBUG
    for (const auto& collider : colliders)
    {
        image.drawAABB(collider.aabb, collider.isOneWay ? Color::Yellow : Color::Red, BlendMode::Disable, FillMode::WireFrame);
    }
#endif
}

