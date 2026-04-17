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

void handle_collision(Ball& ball, Paddle& paddle)
{
    if (!is_overlapping(ball, paddle))
    {
        return;
    }

    const auto ballBB = ball.get_bounding_box();
    const auto paddleBB = paddle.get_bounding_box();

    const float overlapLeft = (ballBB.position.x + ballBB.size.x) - paddleBB.position.x;
    const float overlapRight = (paddleBB.position.x + paddleBB.size.x) - ballBB.position.x;
    const float overlapTop = (ballBB.position.y + ballBB.size.y) - paddleBB.position.y;
    const float overlapBottom = (paddleBB.position.y + paddleBB.size.y) - ballBB.position.y;

    const float minOverlapX = std::min(overlapLeft, overlapRight);
    const float minOverlapY = std::min(overlapTop, overlapBottom);

    if (minOverlapX < minOverlapY)
    {
        ball.bounceHorizontal();
        return;
    }

    if (ballBB.getCenter().y > paddleBB.position.y) // we are not bouncing up, if the ball is too low
    {
        return;
    }

    const float ballCenterX = ballBB.position.x + ballBB.size.x * 0.5f;
    const float paddleCenterX = paddleBB.position.x + paddleBB.size.x * 0.5f;
    const float deltaX = ballCenterX - paddleCenterX;

    if (std::abs(deltaX) <= 5.0f)   // bounce up if ball hits paddle center +/- 5 pixels
    {
        ball.bounceFromPaddle(0.0f);
        return;
    }

    const float halfPaddleWidth = paddleBB.size.x * 0.5f;
    const float relativeHitX = deltaX / halfPaddleWidth;

    ball.bounceFromPaddle(relativeHitX);
}