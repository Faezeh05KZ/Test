#include "Hero.hpp"
#include <iostream>
#include <utility>
#include <numeric>
#include <algorithm>
#include <vector>

using namespace std;

Hero::Hero(string name, HeroType type, int maxActions, string startingLocation)
    : name(move(name)), type(type), maxActions(maxActions), actionsLeft(maxActions), currentLocation(move(startingLocation)), damaged(false) {}

string Hero::getName() const
{
    return name;
}

HeroType Hero::getType() const
{
    return type;
}

int Hero::getActionsLeft() const
{
    return actionsLeft;
}

string Hero::getCurrentLocation() const
{
    return currentLocation;
}

const vector<shared_ptr<Item>>& Hero::getItems() const
{
    return items;
}

const vector<shared_ptr<PerkCard>>& Hero::getPerkCards() const
{
    return perkCards;
}

bool Hero::isDamaged() const
{
    return damaged;
}

void Hero::resetActions()
{
    actionsLeft = maxActions;
}

void Hero::useAction()
{
    if(actionsLeft > 0) actionsLeft--;
}

void Hero::addActions(int count)
{
    actionsLeft += count;
}

void Hero::takeDamage()
{
    damaged = true;
}

void Hero::addItem(shared_ptr<Item> item)
{
    items.push_back(item);
}

void Hero::addPerkCard(shared_ptr<PerkCard> card) 
{
    perkCards.push_back(card);
}

shared_ptr<PerkCard> Hero::removePerkCard(const std::string& cardName) 
{
    for (auto it = perkCards.begin(); it != perkCards.end(); ++it) 
    {
        if ((*it)->getCardName() == cardName) 
        {
            auto foundCard = *it;
            perkCards.erase(it);

            return foundCard;
        }
    }

    return nullptr;
}


shared_ptr<Item> Hero::removeItem(const string& itemName)
{
    for (auto it = items.begin(); it != items.end(); ++it)
    {
        if ((*it)->getName() == itemName)
        {
            auto foundItem = *it;
            items.erase(it);
            return foundItem;
        }
    }
    return nullptr;
}

int Hero::getTotalItemStrengthByColor(ItemColor color) const
{
    int total = 0;
    for (const auto& item : items)
    {
        if (item->getColor() == color)
        {
            total += item->getStrength();
        }
    }
    return total;
}

void Hero::removeItemsByStrength(ItemColor color, int requiredStrength)
{
    vector<shared_ptr<Item>> itemsOfColor;
    for(const auto& item : items)
    {
        if(item->getColor() == color)
        {
            itemsOfColor.push_back(item);
        }
    }

    sort(itemsOfColor.begin(), itemsOfColor.end(), [](const auto& a, const auto& b)
    {
        return a->getStrength() > b->getStrength();
    });

    int currentStrength = 0;
    vector<string> itemsToRemoveNames;

    for (const auto& item : itemsOfColor) 
    {
        currentStrength += item->getStrength();
        itemsToRemoveNames.push_back(item->getName());

        if (currentStrength >= requiredStrength) 
        {
            break;
        }
    }

    if (currentStrength >= requiredStrength)
    {
        for (const auto& name : itemsToRemoveNames)
        {
            cout << "Used item: " << name << endl;
            removeItem(name);
        }
    }
}


void Hero::setCurrentLocation(string location)
{
    currentLocation = move(location);

    if(damaged) damaged = false;
}

void Hero::displayInfo() const
{
    cout << "Hero: " << name << " (" << heroTypeToString(type) << ")" << endl;
    cout << "  Location: " << currentLocation << (damaged ? " (Defeated, at Hospital)" : "") << endl;
    cout << "  Actions Left: " << actionsLeft << "/" << maxActions << endl;
    cout << "  Items: ";

    if (items.empty())
    {
        cout << "None";
    }
    else
    {
        for (const auto& item : items)
        {
            cout << item->getName() << "(" << item->getStrength() << ") ";
        }
    }
    cout << endl;
    
    cout << "  Perk Cards: ";
    if (perkCards.empty()) 
    {
        cout << "None";
    } 
    
    else 
    {
        for (const auto& card : perkCards) 
        {
            cout << "'" << card->getCardName() << "' ";
        }
    }
    cout << endl;
}


bool Hero::hasItem(const std::string& itemName) const 
{
    return any_of(items.begin(), items.end(),
        [&itemName](const auto& item) { return item->getName() == itemName; });
}
