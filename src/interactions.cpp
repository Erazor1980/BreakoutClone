#include <cstdlib>
#include "interactions.h"

bool is_overlapping(const Entity& e1, const Entity& e2)
{
    const auto bb_e1 = e1.get_bounding_box();
    const auto bb_e2 = e2.get_bounding_box();

    return bb_e1.findIntersection(bb_e2).has_value();
}

BrickCollisionResult handle_collision(Ball& ball, Brick& brick)
{
    const auto ballBB = ball.get_bounding_box();
    const auto brickBB = brick.get_bounding_box();

    BrickCollisionResult result{};

    // No overlap means no collision in this frame.
    if (!ballBB.findIntersection(brickBB).has_value())
    {
        return result;
    }

    // The brick was hit and will take damage exactly once for this ball in this frame.
    result.m_bHit = true;
    brick.damage();

    // We use the previous ball position to determine from which side
    // the ball entered the brick. This is more stable than using the
    // current overlap only.
    const sf::Vector2f previousPos = ball.getPreviousPosition();

    const float previousTop = previousPos.y - ballBB.size.y * 0.5f;
    const float previousBottom = previousTop + ballBB.size.y;
    const float previousLeft = previousPos.x - ballBB.size.x * 0.5f;
    const float previousRight = previousLeft + ballBB.size.x;

    const float brickTop = brickBB.position.y;
    const float brickBottom = brickBB.position.y + brickBB.size.y;
    const float brickLeft = brickBB.position.x;
    const float brickRight = brickBB.position.x + brickBB.size.x;

    // If the ball was completely above or below the brick in the previous frame,
    // we treat it as a vertical collision.
    if (previousBottom <= brickTop || previousTop >= brickBottom)
    {
        result.m_bBounceY = true;
    }

    // If the ball was completely left or right of the brick in the previous frame,
    // we treat it as a horizontal collision.
    if (previousRight <= brickLeft || previousLeft >= brickRight)
    {
        result.m_bBounceX = true;
    }

    // Corner case:
    // If neither axis was classified clearly, we fall back to the overlap test.
    // This can happen when the ball moves into a corner region in a single frame.
    if (!result.m_bBounceX && !result.m_bBounceY)
    {
        const float overlapLeft = (ballBB.position.x + ballBB.size.x) - brickBB.position.x;
        const float overlapRight = (brickBB.position.x + brickBB.size.x) - ballBB.position.x;
        const float overlapTop = (ballBB.position.y + ballBB.size.y) - brickBB.position.y;
        const float overlapBottom = (brickBB.position.y + brickBB.size.y) - ballBB.position.y;

        const float minOverlapX = std::min(overlapLeft, overlapRight);
        const float minOverlapY = std::min(overlapTop, overlapBottom);

        if (minOverlapX < minOverlapY)
        {
            result.m_bBounceX = true;
        }
        else
        {
            result.m_bBounceY = true;
        }
    }

    return result;
}

bool handle_collision(Ball& ball, Paddle& paddle)
{
    const auto ballBB = ball.get_bounding_box();
    const auto paddleBB = paddle.get_bounding_box();

    if (!ballBB.findIntersection(paddleBB).has_value())
    {
        return false;
    }

    const sf::Vector2f previousPos = ball.getPreviousPosition();

    const float previousTop = previousPos.y - ballBB.size.y * 0.5f;
    const float previousBottom = previousTop + ballBB.size.y;
    const float previousLeft = previousPos.x - ballBB.size.x * 0.5f;
    const float previousRight = previousLeft + ballBB.size.x;

    const float paddleTop = paddleBB.position.y;
    const float paddleBottom = paddleBB.position.y + paddleBB.size.y;
    const float paddleLeft = paddleBB.position.x;
    const float paddleRight = paddleBB.position.x + paddleBB.size.x;

    if (previousBottom <= paddleTop)
    {
        const float ballCenterX = ballBB.position.x + ballBB.size.x * 0.5f;
        const float paddleCenterX = paddleBB.position.x + paddleBB.size.x * 0.5f;
        const float deltaX = ballCenterX - paddleCenterX;

        if (std::abs(deltaX) <= 5.0f)
        {
            ball.bounceFromPaddle(0.0f);
            return true;
        }

        const float halfPaddleWidth = paddleBB.size.x * 0.5f;
        const float relativeHitX = deltaX / halfPaddleWidth;

        ball.bounceFromPaddle(relativeHitX);
        return true;
    }

    if (previousRight <= paddleLeft || previousLeft >= paddleRight)
    {
        ball.bounceHorizontal();
        return true;
    }

    return false;
}