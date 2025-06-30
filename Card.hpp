#ifndef CARD_HPP
#define CARD_HPP

#include <string>
#include "Monster.hpp" 

class GameManager; 

enum class CardType 
{
    PERK,
    MONSTER
};

class Card 
{
    public:

        virtual ~Card() = default;
        std::string getCardName() const;
        CardType getCardType() const;
        virtual void applyEffect(GameManager& engine) = 0;
        virtual std::string getDescription() const = 0;

    protected:

        Card(std::string name, CardType type);
        std::string cardName;
        CardType cardType;
};
#endif 