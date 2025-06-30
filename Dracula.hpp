#ifndef DRACULA_HPP 
#define DRACULA_HPP 

#include <string>
#include <vector>
#include <set>
#include "Monster.hpp"

class Dracula : public Monster
{
    public:

        Dracula(const std::string& startingLocation);
        
        void usePower(GameManager& gameManager) override;
        void attack(GameManager& gameManager) override;
        bool canBeDefeated(const GameManager& gameManager) const override;
        
        bool destroyCoffin(const std::string& location);
        bool allCoffinsDestroyed() const;
        int getDestroyedCoffinsCount() const { return destroyedCoffins.size(); }
        const std::vector<std::string>& getCoffinLocations() const; // اضافه شد

        int getTaskProgress() const override { return getDestroyedCoffinsCount(); }
        int getRequiredTaskProgress() const override { return 4; }

    private:
    
        std::vector<std::string> coffinLocations;
        std::set<std::string> destroyedCoffins;
};

#endif
