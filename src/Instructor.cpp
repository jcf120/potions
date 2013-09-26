/*******************************************************************************
 * Project:     Potions
 * File:        Instructor.h
 * Author:      Jocelyn Clifford-Frith
 * Date:        10th September 2013
 * Standard:    C++11 (<random>, auto types and range-based loops)
 ******************************************************************************/
 
#include "Instructor.h"
#include <vector>
#include <iostream>
#include <random>
#include <ctime>

using namespace std;

void Instructor::randomlyCombineRemainingPairs(Alchemist & alchemist)
{
	// Make a local copy of the known ingredients
	vector<Ingredient> ingredients = alchemist.allKnownIngredients();
	
	// Setup random distribution
	default_random_engine randGen(time(0));
	uniform_int_distribution<int> randDist(0, ingredients.size() - 1);
	
	// Combine pairs of ingredients at random until no distinct pairs remain.
	while (alchemist.calculateVarietiesInStock() > 1)
	{
		// Choose two different ingredients that are in stock
		Ingredient ingr1, ingr2;
		while (!alchemist.hasIngredient(ingr1))
			ingr1 = ingredients[randDist(randGen)];
		while (!alchemist.hasIngredient(ingr2) || ingr1 == ingr2)
			ingr2 = ingredients[randDist(randGen)];
		
		// Combine them
		alchemist.combine(ingr1, ingr2);
	}
}

void Instructor::combineAllPairsWithMatchingEffects(Alchemist & alchemist)
{
	int passes = 0;
	bool succeededLastPass = true;
	while (succeededLastPass)
	{
	passes++;
	succeededLastPass = false;
	
		// Collect the known status effects
		vector<StatusEffect> knownEffects = alchemist.allKnownEffects();
		
		// Check each effect for available matches
		for (const StatusEffect& effect : knownEffects) {
			
			// Copy vector, rather than reference, so iterators don't invalidate.
			auto ingredients = alchemist.getIngredientsWithEffect(effect);
			
			// Skip onto next effect if only one ingredient is known
			if (ingredients.size() < 2) break;
			
			// Iterate across ingredients, combining those in stock.
			auto i1 = ingredients.begin();
			auto i2 = next(i1);
			
			while (i2 != ingredients.end() && i1 != ingredients.end())
			{
				while (   i1 != ingredients.end()
					   && !alchemist.hasIngredient(*i1))
					++i1;
				while (   i2 != ingredients.end()
					   && (!alchemist.hasIngredient(*i2) || i1 == i2))
					++i2;
				
				// Check iterators are valid
				if (i1 != ingredients.end() && i2 != ingredients.end())
				{
					// Make a potion
					alchemist.combine(*i1, *i2);
					succeededLastPass = true;
				}
			}
		}
	}
}