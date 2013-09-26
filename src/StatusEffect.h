/*******************************************************************************
 * Project:     Potions
 * File:        StausEffect.h
 * Author:      Jocelyn Clifford-Frith
 * Date:        10th September 2013
 * Standard:    C++98
 *
 * StatusEffect instances are created via the static method newIngredient(). 
 * Each returned instance has a unique id and a specified rarity, all of which 
 * are immutable. The class maintains a static collection of all existing 
 * StatusEffects. The static method randomStatusEffect() returns one of the
 * stored StatusEffects with a probability inversely proportional to its rarity.
 ******************************************************************************/

#pragma once
#include <vector>
#include "WeightedRandomizedStack.h"


class StatusEffect
{
	// Used for sorting and checking that an effect is unique.
	unsigned int id;
	
	// Determines resulting potion's value and the frequency at which the
	// status effect occurs in ingredients
	double rarity;
	
	// Only used by static method - newStatusEffect(const double rarity)
	StatusEffect(const unsigned int id, const double effectRarity);
	
public:
	StatusEffect(); // Only used by containers - uses invalid id 0.
	
	StatusEffect(const StatusEffect& rhs);
	StatusEffect& operator= (const StatusEffect& rhs);
	
	double getRarity() const;
	
	// Comparison operator using StatusEffects' ids
	bool operator==(const StatusEffect& rhs) const;
	bool operator!=(const StatusEffect& rhs) const;
	
	// Relational operator using StatusEffects' ids
	bool operator<(const StatusEffect& rhs) const;

//------------------------------------------------------------------------------
//                              Static methods
//------------------------------------------------------------------------------

	// Returns a StatusEffect with a unique id
	static StatusEffect newStatusEffect(const double rarity);

	// Returns an existing effect at random based on it's rarity.
	// Throws logic_error if no StatusEffects exist.
	static const StatusEffect& randomStatusEffect();
	
	static WeightedRandomizedStack<StatusEffect>
		getExistingEffectsStack();
	
	// Returns the current number of StatusEffects in existence.
	static int total();
	
	// Removes all existing StatusEffects
	static void clearAll();
	
//------------------------------------------------------------------------------
//                               Static members
//------------------------------------------------------------------------------
	
	static const StatusEffect nullValue; // id = 0
	
private:
	// Incremented for assigning unique ids
	static unsigned int sNextId;
	
	// All existing status effect in a weighted set.
	static WeightedRandomizedStack<StatusEffect> sExistingEffects;

};

