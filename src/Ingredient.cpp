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
unsigned int Ingredient::sNextId = 1; // 0 is reserved for invalids.

//------------------------------------------------------------------------------
// Default constructor - initializes with invalid id. Only used by containers.
Ingredient::Ingredient() :
id(0), effects(sMaxEffects)
{
}

//------------------------------------------------------------------------------
// Private constructor - assign all constant members.
Ingredient::Ingredient(const unsigned int id,
					   const vector<StatusEffect>& effects) :
id(id), effects(effects)
{
}

//------------------------------------------------------------------------------
// Assignment operator
Ingredient& Ingredient::operator=(const Ingredient& rhs)
{
	id = rhs.id;
	for (int i = 0; i < sMaxEffects; i++)
		this->effects[i] = rhs.effects[i];
	return *this;
}

//------------------------------------------------------------------------------
// Subscript operator - StatusEffects are readonly
const StatusEffect& Ingredient::operator[](const int i) const
{
	return this->effects[i];
}

//------------------------------------------------------------------------------
// Iterators - enabling c++11 range-based loops
vector<StatusEffect>::const_iterator Ingredient::begin() const
{
	return this->effects.begin();
}
	
vector<StatusEffect>::const_iterator  Ingredient::end() const
{
	return this->effects.end();
}

//------------------------------------------------------------------------------
// Calculate rarity - takes the average rarity of it's status effects
double Ingredient::calculateRarity() const
{
	double average = 0.0;
	for (int i = 0; i < sMaxEffects; i++)
		average += this->effects[i].getRarity();
	return average / sMaxEffects;
}

//------------------------------------------------------------------------------
// Comparison operator - determined by Ingredients' ids
bool Ingredient::operator==(const Ingredient & rhs) const
{
	return this->id == rhs.id;
}

//------------------------------------------------------------------------------
// Relational operator - determined by Ingredients' ids.
bool Ingredient::operator<(const Ingredient& rhs) const
{
	return this->id < rhs.id;
}

//------------------------------------------------------------------------------
// Static constructor - makes sure status effects and id are unique.
Ingredient Ingredient::newIngredient()
{
	// Can only make a new ingredient if at least 4 status effects exist
	if (StatusEffect::total() < sMaxEffects)
		throw logic_error("Cannot discover new ingredient because less "
			"than sMaxEffects status effects exist to choose from");
	
	// Copy the status effects stack and pop the required number
	WeightedRandomizedStack<StatusEffect> existingEffects = 
		StatusEffect::getExistingEffectsStack();
	
	vector<StatusEffect> effects;
	for (int i = 0; i < sMaxEffects; i++)
		effects.push_back(existingEffects.pop());
	
	// Create a new ingredient from these effects and increment the next id.
	return Ingredient(sNextId++, effects);
}