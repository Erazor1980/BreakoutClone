#include <cstdlib>
#include "interactions.h"

bool is_overlapping(const Entity& e1, const Entity& e2)
{
    const auto bb_e1 = e1.get_bounding_box();
    const auto bb_e2 = e2.get_bounding_box();

    return bb_e1.findIntersection(bb_e2).has_value();
}

bool handle_collision(Ball& ball, Brick& brick)
{
    const auto ballBB = ball.get_bounding_box();
    const auto brickBB = brick.get_bounding_box();

    if (!ballBB.findIntersection(brickBB).has_value())
    {
        return false;
    }

    brick.damage();

    const sf::Vector2f previousPos = ball.getPreviousPosition();

    const float previousTop = previousPos.y - ballBB.size.y * 0.5f;
    const float previousBottom = previousTop + ballBB.size.y;

    const float brickTop = brickBB.position.y;
    const float brickBottom = brickBB.position.y + brickBB.size.y;

    if (previousBottom <= brickTop || previousTop >= brickBottom)
    {
        ball.bounceVertical();
    }
    else
    {
        ball.bounceHorizontal();
    }

    return true;
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