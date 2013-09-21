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
#include "WeightedRandomizedSet.h"


class StatusEffect
{
public:
	
	// Default construct initializes with invalid id 0.
	StatusEffect();
	static const StatusEffect nullValue;
	
	// Copy constructor
	StatusEffect(const StatusEffect& rhs);
	
	// Assignment operator
	StatusEffect& operator= (const StatusEffect& rhs);
	
	// Rarity accessor - readonly.
	double rarity() const;

	// Returns a StatusEffect with a unique id
	static StatusEffect newStatusEffect(const double rarity);

	// Returns an existing effect at random based on it's rarity.
	// Throws logic_error if no StatusEffects exist.
	static const StatusEffect& randomStatusEffect();
	
	// Returns the current number of StatusEffects in existence.
	static int total();
	
	// Removes all existing StatusEffects
	static void clearAll();
	
	// Comparison operator using StatusEffects' ids
	bool operator==(const StatusEffect& rhs) const;
	bool operator!=(const StatusEffect& rhs) const;
	
	// Relational operator using StatusEffects' ids
	bool operator<(const StatusEffect& rhs) const;

private:
	StatusEffect(const unsigned int id, const double effectRarity);
	
	// Used for sorting and checking that an effect is unique.
	unsigned int _id;
	
	// Determines resulting potion's value and the frequency at which the
	// status effect occurs in ingredients
	double _rarity;
	
	// Incremented for assigning unique ids
	static unsigned int s_nextId;
	
	// All existing status effect in a weighted set.
	static WeightedRandomizedSet<StatusEffect> s_existingEffects;
};

