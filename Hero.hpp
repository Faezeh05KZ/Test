#ifndef HERO_HPP
#define HERO_HPP

#include <string>
#include <vector>
#include <memory>
#include "Item.hpp"
#include "PerkCard.hpp" 

class PerkCard;

enum class HeroType
{
    ARCHAEOLOGIST,
    MAYOR
};

inline std::string heroTypeToString(HeroType type)
{
    switch (type)
    {
        case HeroType::ARCHAEOLOGIST: return "Archaeologist";
        case HeroType::MAYOR: return "Mayor";
    }
    return "Unknown";
}

class Hero
{
    public:

        Hero(std::string name, HeroType type, int maxActions, std::string startingLocation);
        virtual ~Hero() = default;

        std::string getName() const;
        HeroType getType() const;
        int getActionsLeft() const;
        std::string getCurrentLocation() const;
        const std::vector<std::shared_ptr<Item>>& getItems() const;
        const std::vector<std::shared_ptr<PerkCard>>& getPerkCards() const; 

        void resetActions();
        void useAction();
        void addActions(int count);
        void takeDamage();
        bool isDamaged() const;

        void addItem(std::shared_ptr<Item> item);
        void addPerkCard(std::shared_ptr<PerkCard> card);         
        std::shared_ptr<Item> removeItem(const std::string& itemName);
        std::shared_ptr<PerkCard> removePerkCard(const std::string& cardName);         
        int getTotalItemStrengthByColor(ItemColor color) const;
        void removeItemsByStrength(ItemColor color, int requiredStrength);
        bool hasItem(const std::string& itemName) const;

        void setCurrentLocation(std::string location);
        void displayInfo() const;

        virtual bool hasSpecialAction() const = 0; 

    protected:

        std::string name;
        HeroType type;
        int maxActions;
        int actionsLeft;
        std::string currentLocation;
        bool damaged;
        std::vector<std::shared_ptr<Item>> items;
        std::vector<std::shared_ptr<PerkCard>> perkCards; 
};

#endif
