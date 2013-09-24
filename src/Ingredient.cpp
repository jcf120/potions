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
id(0)
{
}

//------------------------------------------------------------------------------
// Private constructor - assign all constant members.
Ingredient::Ingredient(const unsigned int id,
					   const StatusEffect& se0,
					   const StatusEffect& se1,
					   const StatusEffect& se2,
					   const StatusEffect& se3) :
id(id), effects{se0, se1, se2, se3}
{
}

//------------------------------------------------------------------------------
// Assignment operator
Ingredient& Ingredient::operator=(const Ingredient& rhs)
{
	id = rhs.id;
	for (int i = 0; i < INGREDIENT_EFFECTS_COUNT; i++)
		this->effects[i] = rhs.effects[i];
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
	return this->effects[i];
}

//------------------------------------------------------------------------------
// Iterators - enabling c++11 range-based loops
const StatusEffect* Ingredient::begin() const
{
	return this->effects;
}
	
const StatusEffect* Ingredient::end() const
{
	return this->effects + INGREDIENT_EFFECTS_COUNT;
}

//------------------------------------------------------------------------------
// Calculate rarity - takes the average rarity of it's status effects
double Ingredient::calculateRarity() const
{
	double average = 0.0;
	for (int i = 0; i < INGREDIENT_EFFECTS_COUNT; i++)
		average += this->effects[3].getRarity();
	return average / INGREDIENT_EFFECTS_COUNT;
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
	return Ingredient(sNextId++, e1, e2, e3, e4);
}