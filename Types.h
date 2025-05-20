﻿#pragma once

enum class BlockType {
    UNBREAKABLE,
    REGULAR,
    SPEED_CHANGE,
    WITH_BONUS
};

enum class BonusType {
    PADDLE_SIZE_INCREASE,
    PADDLE_SIZE_DECREASE,
    BALL_SPEED_INCREASE,
    BALL_SPEED_DECREASE,
    STICKY_PADDLE,
    BOTTOM_SHIELD,
    RANDOM_TRAJECTORY_CHANGE
};