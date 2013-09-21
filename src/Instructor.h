/*******************************************************************************
 * Project:     Potions
 * File:        Instructor.h
 * Author:      Jocelyn Clifford-Frith
 * Date:        10th September 2013
 * Standard:    C++11 (auto types and range-based loops)
 *
 * The instructor contains static methods which can be used in combination to
 * develop techniques for efficiently instructing an alchemist how to mix 
 * potions from its ingredient store.
 ******************************************************************************/
 
#pragma once
#include "Alchemist.h"

class Instructor
{
public:
	// Simply combines ingredients at random until it runs out of stock.
	static void randomlyCombineRemainingPairs(Alchemist& alchemist);
	
	// Combines ingredient pairs known to have a common effect. Upon
	// discovering a new effect, it will check for new combinations. The
	// remaining ingredients are combined at random like ApproachA
	static void combineAllPairsWithMatchingEffects(Alchemist& alchemist);
};