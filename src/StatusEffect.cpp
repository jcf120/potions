/*******************************************************************************
 * Project:     Potions
 * File:        StausEffect.cpp
 * Author:      Jocelyn Clifford-Frith
 * Date:        10th September 2013
 * Standard:    C++98
 ******************************************************************************/
 
#include "StatusEffect.h"
#include <stdexcept>

using namespace std;

//------------------------------------------------------------------------------
// Initialize static members
const StatusEffect StatusEffect::nullValue = StatusEffect();
unsigned int StatusEffect::s_nextId = 1; // 0 is reserved for invalid effects.
WeightedRandomizedSet<StatusEffect> StatusEffect::s_existingEffects =
	WeightedRandomizedSet<StatusEffect>();

//------------------------------------------------------------------------------
// Default constructor - only used by containers and represents an invalid.
StatusEffect::StatusEffect() :
_id(0), _rarity(0)
{
}

//------------------------------------------------------------------------------
// Constructor - private, only used by static newStatusEffect method.
StatusEffect::StatusEffect(const unsigned int id, const double effectRarity) :
	_id(id), _rarity(effectRarity)
{
}

//------------------------------------------------------------------------------
// Copy constructor
StatusEffect::StatusEffect(const StatusEffect& rhs) :
	_id(rhs._id), _rarity(rhs._rarity)
{
}

//------------------------------------------------------------------------------
// Assignment operator
StatusEffect& StatusEffect::operator=(const StatusEffect& rhs)
{
	_id = rhs._id;
	_rarity = rhs._rarity;
	return *this;
}

//------------------------------------------------------------------------------
// Rarity accessor
double StatusEffect::rarity() const
{
	return _rarity;
}

//------------------------------------------------------------------------------
// Comparison operators
bool StatusEffect::operator==(const StatusEffect& rhs) const
{
	return _id == rhs._id;
}

bool StatusEffect::operator!=(const StatusEffect& rhs) const
{
	return !(*this == rhs);
}

//------------------------------------------------------------------------------
// Relational operator
bool StatusEffect::operator<(const StatusEffect& rhs) const
{
	return _id < rhs._id;
}

//------------------------------------------------------------------------------
// Static Method - build and store a new unique StatusEffect
StatusEffect StatusEffect::newStatusEffect(const double rarity)
{
	// Initialize with a unique id
	const StatusEffect statusEffect = StatusEffect(s_nextId++, rarity);
	
	// Store it in the weighted set, with rarity's reciprocal as probability.
	s_existingEffects.add(statusEffect, 1.0 / rarity);
	
	// Return the new effect
	return statusEffect;
}

//------------------------------------------------------------------------------
// Static method - returns an existing StatusEffect depending on rarity.
const StatusEffect& StatusEffect::randomStatusEffect()
{
	// Check any StatusEffect actually exist.
	if (s_existingEffects.isEmpty())
		logic_error("No StatusEffects exist from which to select.");
	
	return s_existingEffects.retrieve();
}

//------------------------------------------------------------------------------
// Static method - total StatusEffects in existence
int StatusEffect::total()
{
	return s_existingEffects.size();
}