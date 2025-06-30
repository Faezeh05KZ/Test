#include "Monster.hpp"
#include "Dracula.hpp"
#include "GameManager.hpp"
#include "GameUtils.hpp"
#include "Hero.hpp"

#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <set>

using namespace std;


Dracula::Dracula(const std::string& startingLocation)
    : Monster("Dracula", MonsterType::DRACULA, startingLocation, 1) {}

    void Dracula::usePower(GameManager& gameManager) 
    {
        auto currentHero = gameManager.getCurrentHero();
        
        if (currentHero && currentHero->getCurrentLocation() != currentLocation) 
        {
            gameManager.moveHero(currentHero->getName(), currentLocation);
        }
    }
    
bool Dracula::allCoffinsDestroyed() const {
    return destroyedCoffins.size() >= 4;
}
    
bool Dracula::canBeDefeated(const GameManager& gameManager) const {
            return allCoffinsDestroyed() && 
           gameManager.getHeroAtLocation(currentLocation) != nullptr;
}
    
void Dracula::attack(GameManager& gameManager) {
    if (isDefeated()) 
    return;
        
    Monster::attack(gameManager);
    gameManager.increaseTerrorLevel(1);
}

const std::vector<std::string>& Dracula::getCoffinLocations() const {
    return coffinLocations;
}

bool Dracula::destroyCoffin(const std::string& location) 
{
    if (std::find(destroyedCoffins.begin(), destroyedCoffins.end(), location) == destroyedCoffins.end()) 
    {
        destroyedCoffins.insert(location);
        return true;
    }
    return false;
}