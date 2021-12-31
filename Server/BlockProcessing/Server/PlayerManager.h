#pragma once

#include <Math/Vector.h>
#include <vector>

struct Player {
    const char* name;
    Vec3d position;
};

class PlayerManager {
public:
    void init();
    void addPlayer(Player& player);
    std::vector<Player> players;
    int playerCount = 0;
};