#pragma once
#include "ball.h"
#include "brick.h"
#include "paddle.h"

bool is_overlapping(const Entity& e1, const Entity& e2);

bool handle_collision(Ball& ball, Brick& brick);

void handle_collision(Ball& ball, Paddle& paddle);
