#include "Card.hpp"
#include <utility>

Card::Card(std::string name, CardType type)
    : cardName(std::move(name)), cardType(type) {}

std::string Card::getCardName() const 
{
    return cardName;
}


CardType Card::getCardType() const 
{
    return cardType;
}