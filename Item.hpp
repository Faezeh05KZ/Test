#ifndef ITEM_HPP
#define ITEM_HPP

#include <iostream>
#include <string>

enum class ItemColor
{
    RED,
    BLUE,
    YELLOW
};

inline std::string itemColorToString(ItemColor color)
{
    switch (color)
    {
        case ItemColor::RED: return "Red";
        case ItemColor::BLUE: return "Blue";
        case ItemColor::YELLOW: return "Yellow";
    }

    return "Unknown";
}

class Item
{
    public:
    
        Item(std::string name, ItemColor color, int strength, std::string originalLocation);
        
        std::string getName() const;
        ItemColor getColor() const;
        int getStrength() const;
        std::string getOriginalLocation() const;

    private:

        std::string name;
        ItemColor color;
        int strength;
        std::string originalLocation;
};

#endif