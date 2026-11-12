#pragma once

namespace Constants {
    // Window settings
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 600;

    // Player settings
    constexpr int PLAYER_DEFAULT_WIDTH = 40;
    constexpr int PLAYER_DEFAULT_HEIGHT = 40;
    constexpr float PLAYER_DEFAULT_SPEED = 300.0f; // Pixels per second

    // Combat & Weapon settings
    constexpr float ARROW_SIZE = 6.0f;
    constexpr float ARROW_HALF_SIZE = ARROW_SIZE / 2.0f; // 3.0f for centering
    // constexpr float SWORD_SIZE = 24.0f;
    // constexpr float SWORD_HALF_SIZE = SWORD_SIZE / 2.0f;
    

    constexpr float ATTACK_OFFSET = 30.0f;
}
