/*******************************************************************************
 * Project:     Potions
 * File:        Ingredient.cpp
 * Author:      Jocelyn Clifford-Frith
 * Date:        10th September 2013
 * Standard:    C++11 (Uses array initializer list)
 ******************************************************************************/

#include "Ingredient.h"
#include <stdexcept>
#include <iostream>

using namespace std;

//------------------------------------------------------------------------------
// Initialize static members
const Ingredient Ingredient::nullValue = Ingredient();
unsigned int Ingredient::s_nextId = 1; // 0 is reserved for invalids.

//------------------------------------------------------------------------------
// Default constructor - initializes with invalid id. Only used by containers.
Ingredient::Ingredient() :
_id(0)
{
}

//------------------------------------------------------------------------------
// Private constructor - assign all constant members.
Ingredient::Ingredient(const unsigned int id,
					   const StatusEffect& se0,
					   const StatusEffect& se1,
					   const StatusEffect& se2,
					   const StatusEffect& se3) :
_id(id), _effects{se0, se1, se2, se3}
{
}

//------------------------------------------------------------------------------
// Static constructor - makes sure status effects and id are unique.
Ingredient Ingredient::newIngredient()
{
	// Can only make a new ingredient if at least 4 status effects exist
	if (StatusEffect::total() < 4)
		throw logic_error("Cannot discover new ingredient because less "
			"than 4 status effects exist to choose from");
	
	// Select random status effects and check we don't reuse the same one.
	StatusEffect e1 = StatusEffect::randomStatusEffect();
	StatusEffect e2 = StatusEffect::randomStatusEffect();
	StatusEffect e3 = StatusEffect::randomStatusEffect();
	StatusEffect e4 = StatusEffect::randomStatusEffect();
	
	// Reassign until all values are different.
	while (e2 == e1)
		e2 = StatusEffect::randomStatusEffect();
	while (e3 == e1 || e3 == e2)
		e3 = StatusEffect::randomStatusEffect();
	while (e4 == e1 || e4 == e2 || e4 == e3)
		e4 = StatusEffect::randomStatusEffect();
	
	// Create a new ingredient from these effects and increment the next id.
	return Ingredient(s_nextId++, e1, e2, e3, e4);
}

//------------------------------------------------------------------------------
// Assignment operator
Ingredient& Ingredient::operator=(const Ingredient& rhs)
{
	_id = rhs._id;
	_effects[0] = rhs._effects[0];
	_effects[1] = rhs._effects[1];
	_effects[2] = rhs._effects[2];
	_effects[3] = rhs._effects[3];
	return *this;
}

//------------------------------------------------------------------------------
// Subscript operator - StatusEffects are readonly
const StatusEffect& Ingredient::operator[](const int i) const
{
	// Check bounds
	if (i < 0 || i >= INGREDIENT_EFFECTS_COUNT)
		throw out_of_range("Attempted to access StatusEffect outside "
			"Ingredient's range.");
	return _effects[i];
}

//------------------------------------------------------------------------------
// Iterators - enabling c++11 range-based loops
const StatusEffect* Ingredient::begin() const
{    return _effects;    }
	
const StatusEffect* Ingredient::end() const
{    return _effects + INGREDIENT_EFFECTS_COUNT;    }

//------------------------------------------------------------------------------
// Calculate rarity - takes the average rarity of it's status effects
double Ingredient::calculateRarity() const
{
	return (_effects[0].rarity() + _effects[1].rarity() +
			_effects[2].rarity() + _effects[3].rarity()) / 4.0;
}

//------------------------------------------------------------------------------
// Comparison operator - determined by Ingredients' ids
bool Ingredient::operator==(const Ingredient & rhs) const
{
	return _id == rhs._id;
}

//------------------------------------------------------------------------------
// Relational operator - determined by Ingredients' ids.
bool Ingredient::operator<(const Ingredient& rhs) const
{
	return _id < rhs._id;
}