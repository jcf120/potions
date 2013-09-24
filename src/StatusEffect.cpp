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
unsigned int StatusEffect::sNextId = 1; // 0 is reserved for invalid effects.
WeightedRandomizedStack<StatusEffect> StatusEffect::sExistingEffects =
	WeightedRandomizedStack<StatusEffect>();

//------------------------------------------------------------------------------
// Default constructor - only used by containers and represents an invalid.
StatusEffect::StatusEffect() :
id(0), rarity(0)
{
}

//------------------------------------------------------------------------------
// Constructor - private, only used by static newStatusEffect method.
StatusEffect::StatusEffect(const unsigned int id, const double rarity) :
	id(id), rarity(rarity)
{
}

//------------------------------------------------------------------------------
StatusEffect::StatusEffect(const StatusEffect& rhs) :
	id(rhs.id), rarity(rhs.rarity)
{
}

//------------------------------------------------------------------------------
StatusEffect& StatusEffect::operator=(const StatusEffect& rhs)
{
	this->id = rhs.id;
	this->rarity = rhs.rarity;
	return *this;
}

//------------------------------------------------------------------------------
double StatusEffect::getRarity() const
{
	return this->rarity;
}

//------------------------------------------------------------------------------
bool StatusEffect::operator==(const StatusEffect& rhs) const
{
	return this->id == rhs.id;
}

bool StatusEffect::operator!=(const StatusEffect& rhs) const
{
	return !(*this == rhs);
}

//------------------------------------------------------------------------------
// Relational operator
bool StatusEffect::operator<(const StatusEffect& rhs) const
{
	return this->id < rhs.id;
}

////////////////////////////////////////////////////////////////////////////////
//
//                              Static methods
//
////////////////////////////////////////////////////////////////////////////////	

//------------------------------------------------------------------------------
// Static Method - build and store a new unique StatusEffect
StatusEffect StatusEffect::newStatusEffect(const double rarity)
{
	// Initialize with a unique id
	const StatusEffect statusEffect = StatusEffect(sNextId++, rarity);
	
	// Store it in the weighted set, with rarity's reciprocal as probability.
	sExistingEffects.push(statusEffect, 1.0 / rarity);
	
	// Return the new effect
	return statusEffect;
}

//------------------------------------------------------------------------------
// Static method - returns an existing StatusEffect depending on rarity.
const StatusEffect& StatusEffect::randomStatusEffect()
{
	// Check any StatusEffect actually exist.
	if (sExistingEffects.isEmpty())
		logic_error("No StatusEffects exist from which to select.");
	
	return sExistingEffects.peak();
}

//------------------------------------------------------------------------------
// Static method - total StatusEffects in existence
int StatusEffect::total()
{
	return sExistingEffects.size();
}