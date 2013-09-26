/*******************************************************************************
 * Project:     Potions
 * File:        Ingredient.h
 * Author:      Jocelyn Clifford-Frith
 * Date:        10th September 2013
 * Standard:    C++11 (Uses array initializer list)
 *
 * Ingredient instances are created via the static method newIngredient(). Each
 * returned instance has a unique id and 4 randomly assigned StatusEffects, all
 * of which are immutable. An ingredient's rarity - used to determine how likely
 * it is to be foraged by an alchemist - is calculated as the average rarity of
 * its status effects.
 ******************************************************************************/

#pragma once
#include "StatusEffect.h"

class Ingredient
{
	// Used for sorting in containers
	unsigned int id;
	
	// The potential status effects of the ingredient when combined with others.
	static const int sMaxEffects = 4;
	std::vector<StatusEffect> effects;
	
	// Only used by static method - newIngredient()
	Ingredient(const unsigned int id,
			   const std::vector<StatusEffect>& effects);

public:
	// Default constructor - creates invalid id, only used by containers.
	Ingredient();
	static const Ingredient nullValue;
	
	Ingredient& operator=(const Ingredient& rhs);
	
	// Status effects are readonly, and accessed via the subscript operator.
	const StatusEffect& operator[](const int i) const;
	
	// Also accessible via iterators (to allow range-based loops)
	std::vector<StatusEffect>::const_iterator begin() const;
	std::vector<StatusEffect>::const_iterator end() const;
	
	// Returns the average rarity of the ingredient's status effects.
	double calculateRarity() const;
	
	// Comparison operator used for list and map sorting via ids.
	bool operator==(const Ingredient& rhs) const;
	
	// Relational operator used for list and map sorting via ids.
	bool operator<(const Ingredient& rhs) const;
	
//------------------------------------------------------------------------------
//                                  Statics
//------------------------------------------------------------------------------

	// Construct via static method so each Ingredient has a unique id.
	// Status effects are assigned randomly according to rarity.
	static Ingredient newIngredient();
	
private:
	// Used for assigning unique ids.
	static unsigned int sNextId;
};

