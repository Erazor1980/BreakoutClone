#pragma once
#include "ball.h"
#include "brick.h"
#include "paddle.h"

struct BrickCollisionResult
{
    bool m_bHit{ false };
    bool m_bBounceX{ false };
    bool m_bBounceY{ false };
};

bool is_overlapping(const Entity& e1, const Entity& e2);

BrickCollisionResult handle_collision(Ball& ball, Brick& brick);

bool handle_collision(Ball& ball, Paddle& paddle);
