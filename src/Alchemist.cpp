/*******************************************************************************
 * Project:     Potions
 * File:        Alchemist.h
 * Author:      Jocelyn Clifford-Frith
 * Date:        10th September 2013
 * Standard:    C++11 (auto types and range-based loops)
 ******************************************************************************/

#include "Alchemist.h"
#include <stdexcept>
#include <algorithm>
#include "WeightedRandomizedSet.h"

using namespace std;

//------------------------------------------------------------------------------
// Returns the total worth of all brewed potions
double Alchemist::inventoryValue() const
{
	return _inventoryValue;
}

//------------------------------------------------------------------------------
// Returns that number of combinations that resulted in no gain.
int Alchemist::worthlessPotionCount()
{
	return _worthlessPotionCount;
}

//------------------------------------------------------------------------------
// Default constructor
Alchemist::Alchemist() :
_inventoryValue(0.0), _totalIngredientsRemaining(0), _worthlessPotionCount(0)
{
}

//------------------------------------------------------------------------------
// 'Discover' a new unique ingredient and make a record of it as empty in stock.
const Ingredient Alchemist::discoverNewIngredient()
{
	// Fetch a new ingredient with unique id and properties.
	const Ingredient ingredient = Ingredient::newIngredient();
	
	// Add the ingredient to the store with a stock of zero.
	_ingredientStore[ingredient] = 0;
	
	// 'Eat' the ingredient and learn it's first effect
	learnIngredientEffect(ingredient, ingredient[0]);
	
	// Return the new ingredient
	return ingredient;
}

//------------------------------------------------------------------------------
// Forage - adds a specified number of ingredients to the store. The ingredient
// varieties are determined by the rarities of those that have been 'discovered'
void Alchemist::forage(const int count)
{
	// Generate a WeightedRandomizedSet from the discovered ingredients. This
	// represents the garden from which ingredients are foraged.
	WeightedRandomizedSet<Ingredient> garden;
	
	// Fills the garden with potential ingredients and info about rarity.
	for (auto& p : _ingredientStore) // p is an Ingredient-int pair
		garden.add(p.first, 1.0 / p.first.calculateRarity());
	
	// Fetch ingredients from the garden the specified number of times.
	// Increment the stock of each ingredient retrieved.
	for (int i = 0; i < count; i++)
		_ingredientStore[garden.retrieve()]++;
	
	// Note the increase in stock
	_totalIngredientsRemaining += count;
}

//------------------------------------------------------------------------------
// Returns a vector of all known ingredients - even if stock is zero
vector<Ingredient> Alchemist::allKnownIngredients() const
{
	// State the vector's size, so it isn't returned needlessly large.
	vector<Ingredient> ingredients(_ingredientStore.size());
	
	// Copy in the ingredients
	int i = 0;
	for (auto& p : _ingredientStore) // p is a pair reference
		ingredients[i++] = p.first;
	
	// Return the ingredients
	return ingredients;
}

//------------------------------------------------------------------------------
// Check if an ingredient is currently stocked (one or more)
bool Alchemist::hasIngredient(const Ingredient& ingredient) const
{
	return countOfIngredient(ingredient) > 0;
}

//------------------------------------------------------------------------------
// Returns the stock count of the specified ingredient
int Alchemist::countOfIngredient(const Ingredient& ingredient) const
{
	// Search for the ingredient in the map.
	auto iP = _ingredientStore.find(ingredient); // iP is a pair iterator
	
	// Check a value was found.
	if (iP == _ingredientStore.end())
		return 0;
	
	// Return the pair stock value
	return iP->second;
}

//------------------------------------------------------------------------------
// Returns the total number of all ingredients remaining in stock.
int Alchemist::totalIngredientsRemaining() const
{
	return _totalIngredientsRemaining;
}


//------------------------------------------------------------------------------
// Returns the number of ingredient varieties still in stock.
int Alchemist::calculateVarietiesInStock() const
{
	int varietiesInStock = 0;
	
	// Increment for each ingredient stock more than 0;
	for (auto& p : _ingredientStore) // p is an ingredient-int pair
		if (p.second > 0) varietiesInStock++;
		
	// Return the calculated value
	return varietiesInStock;
}
//------------------------------------------------------------------------------
// Returns a vector of all StatusEffects in the effectsReference map.
vector<StatusEffect> Alchemist::allKnownEffects() const
{
	vector<StatusEffect> effects;
	
	// Add each key to the vector
	for (auto& p : _effectsReference) // p is a effect-vector pair
		effects.push_back(p.first);
	
	// Return all the vector of keys
	return effects;
}

//------------------------------------------------------------------------------
// Returns the vector of ingredients in effectsReference for the effect.
// Note: method can't be guaranteed const because map subscripting isn't.
const vector<Ingredient>& Alchemist::getIngredientsWithEffect
	(const StatusEffect& effect)
{
	return _effectsReference[effect];
}

//------------------------------------------------------------------------------
// Returns true if the ingredient is found under the effect in effectsReference.
bool Alchemist::ingredientHasEffect
	(const Ingredient& ingredient, const StatusEffect& effect) const
{
	// Has the effect been registered?
	auto iRef = _effectsReference.find(effect); // iRef is a pair iterator
	if (iRef == _effectsReference.end())
		return false;
	
	// Is the ingredient listed under the found effect?
	const vector<Ingredient>& vec = iRef->second;
	return find(vec.begin(), vec.end(), ingredient) != vec.end();
}

//------------------------------------------------------------------------------
// Notes a new status effect for an ingredient. 
void Alchemist::learnIngredientEffect
	(const Ingredient& ingredient, const StatusEffect& effect)
{
	// Check the ingredient is know to the alchemist
	auto iStore = _ingredientStore.find(ingredient); // ingr-int pair iterator
	if (iStore == _ingredientStore.end())
		throw logic_error("Alchemist attempted to learn the effect of an "
			"ingredients that it's new encountered before.");
	
	// Look for the status effect in the effectsRefence.
	auto iRef = _effectsReference.find(effect); // effect-vec pair iterator
	
	// Add the effect to the reference if it hasn't been before, along with the
	// ingredient exhibiting the effect.
	if (iRef == _effectsReference.end()) {
		_effectsReference[effect] = vector<Ingredient>({ingredient});
	}
	
	// Otherwise add the ingredient under the effect if it hasn't been already.
	else {
		vector<Ingredient>& vec = iRef->second;
		if (find(vec.begin(), vec.end(), ingredient) == vec.end())
			vec.push_back(ingredient);
		}
}

//------------------------------------------------------------------------------
// Combine ingredients - increase inventory value and record common effects
Discovery Alchemist::combine
	(const Ingredient& i1, const Ingredient& i2)
{
	// Are both ingredients in stock?
	if (!hasIngredient(i1) || !hasIngredient(i2))
		throw logic_error("Alchemist attempted to combine ingredients that "
			"weren't in stock.");
	
	// Make sure the two ingredients are different.
	if (i1 == i2)
		throw invalid_argument("Alchemist::combine() - i1 and i2 cannot share "
			"the same value");
	
	// Remove one of each ingredient from the store
	_ingredientStore[i1]--;
	_ingredientStore[i2]--;
	_totalIngredientsRemaining -= 2;
	
	// Findings from this combination will be returned with this object.
	// If nothing is learned on gained, this will be returned empty.
	Discovery discovery;
	
	// Find the rarest matching status effects in the ingredients.
	const StatusEffect* pRarestEffect = nullptr;
	
	// Loop over all possible effect combinations and check for matches.
	for (const StatusEffect& e1 : i1)
		for (const StatusEffect& e2 : i2)
			if (e1 == e2)
			{
				// Note the match if none were previously found.
				if (!pRarestEffect)
					pRarestEffect = &e1;
				// Else, note the match if the effect's rarer than the last.
				else if (pRarestEffect->rarity() < e1.rarity())
					pRarestEffect = &e1;
				
				// Check if the effects have already been discovered, and learn 
				// them if not. Note anything learned in the returned Discovery.
				if (!ingredientHasEffect(i1, e1)) {
					learnIngredientEffect(i1, e1);
					discovery.addFinding(i1, e1);
				}
				if (!ingredientHasEffect(i2, e2)) {
					learnIngredientEffect(i2, e2);
					discovery.addFinding(i2, e2);
				}
			}
			
	// If a match was found, set the potion's value as the effect's rarity.
	if (pRarestEffect) {
		discovery.potionValue = pRarestEffect->rarity();
		
		// Increase the inventory's value by that of the potion
		_inventoryValue += discovery.potionValue;
	}
	// Otherwise, note the waste of ingredients
	else {
		_worthlessPotionCount++;
	}
	
	// return everything that been learned (potentially nothing)
	return discovery;
}

//------------------------------------------------------------------------------
// Combine ingredients - same as above but for three ingredients
/*Discovery Alchemist::combine
	(const Ingredient& i1, const Ingredient& i2, const Ingredient& i3)
{	
	// Are all ingredients in stock?
	if (!hasIngredient(i1) || !hasIngredient(i2) || !hasIngredient(i3))
		throw logic_error("Alchemist attempted to combine ingredients that "
			"weren't in stock.");
	
	// Make sure the three ingredients are different.
	if (i1 == i2 || i1 == i3 || i2 == i3)
		throw invalid_argument("Alchemist::combine() - i1, i2 and i3 must all"
			"have different values");
	
	// Remove one of each ingredient from the store
	_ingredientStore[i1]--;
	_ingredientStore[i2]--;
	_ingredientStore[i3]--;
	_totalIngredientsRemaining -= 3;
	
	// Findings from this combination will be returned with this object.
	// If nothing is learned on gained, this will be returned empty.
	Discovery discovery;
	
	// Find the rarest matching status effects in the ingredients.
	const StatusEffect* pRarestEffect = nullptr;
	
	// Loop over all possible effect combinations and check for matches.
	//  ..........
			
	// If a match was found, set the potion's value as the effect's rarity.
	if (pRarestEffect) {
		discovery.potionValue = pRarestEffect->rarity();
		
		// Increase the inventory's value by that of the potion
		_inventoryValue += discovery.potionValue;
	}
	// Otherwise, note the waste of ingredients
	else {
		_worthlessPotionCount++;
	}
	
	// return everything that been learned (potentially nothing)
	return discovery;
}*/