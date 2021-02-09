#include "Random.h"
#include <chrono>
#include <random>

void setSeed(unsigned int seed){
    srand(seed);
}

bool getRandomBool(){
    return 0 + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(1 - 0)));
}

int getRandomInt(int minNum, int maxNum) {
    return 0 + rand() / RAND_MAX/(1 - 0);
}

float getRandomFloat(float minNum, float maxNum) {
    return minNum + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(maxNum - minNum)));
}

uint64_t getRandomSeed() {
    return std::chrono::system_clock::now().time_since_epoch().count();
}