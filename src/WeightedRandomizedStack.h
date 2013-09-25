/*******************************************************************************
 * Project:     Potions
 * File:        WeightedRandomizedStack.h
 * Author:      Jocelyn Clifford-Frith
 * Date:        11th September 2013
 * Standard:    C++11 (auto type)
 *
 * This template class maintains a collection of generic objects, which can be
 * retrieved with a likelihood determined by the weighting used at the time 
 * adding the item to the collection. An  item is retrieved by choosing a random
 * number between 0 and the total weighting, and traverses the cumulative
 * weightings until one is found that is greater than the random value.
 ******************************************************************************/

#pragma once
#include <vector>
#include <stdexcept>
#include <random>
#include <ctime>


template<class T> class WeightedRandomizedStack
{
	struct Choice
	{
		T value;
		double weighting;
		
		Choice(const T& v, const double w) : value(v), weighting(w) {}
	};
	
	// Holds the stack's values and weightings
	std::vector<Choice> choices;
	
	// The sum of all elements' weightings
	double probabilitySpaceSize;
	
	// The set's random number generator and distribution
	std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution;
	
public:
	
	// Initializes an empty WeightedRandomizedStack.
	WeightedRandomizedStack<T>();
	
	// Return true if the set is empty.
	bool isEmpty() const;
	
	// Returns is size of the set.
	int size() const;
	
	// Adds the item to the set and records it's weighting.
	void push(const T & item, const double weighting);
	
	// Retrieves an item with a probability according to it's weighting.
	// Throws logic_error if the set is empty.
	const T& pop();
	
	// As above, but without removing it from the stack.
	const T& peak();
	
};

//------------------------------------------------------------------------------
// Default constructor
template <class T>
WeightedRandomizedStack<T>::WeightedRandomizedStack() :
probabilitySpaceSize(0.0)
{
	// Build the probability distribution
	generator = std::default_random_engine(time(0));
    distribution = std::uniform_real_distribution<double>(0.0, 1.0);
}

//------------------------------------------------------------------------------
// Check whether set is empty.
template <class T>
bool WeightedRandomizedStack<T>::isEmpty() const
{
	return this->choices.empty();
}

//------------------------------------------------------------------------------
template <class T>
int WeightedRandomizedStack<T>::size() const
{
	return this->choices.size();
}

//------------------------------------------------------------------------------
template <class T>
void WeightedRandomizedStack<T>::push(const T & item, const double weighting)
{	
	// Store the value and weighting together as a Choice struct
	this->choices.push_back(Choice(item, weighting));
	
	// Increase the size of probability space
	this->probabilitySpaceSize += weighting;
}

//------------------------------------------------------------------------------
// Returns an item with a probability according to its recorded weighting.
// non-const because of random mechanism
template <class T>
const T& WeightedRandomizedStack<T>::pop()
{
	// Check there's an item to return.
	if (isEmpty())
		throw std::logic_error("Attempted to retrieve an item from an empty"
			"WeightedRandomizedStack");
			
	// Select a random number within probability space
	const double randSample = this->distribution(this->generator)
							  * this->probabilitySpaceSize;
	
	// Find the index of the first element greater than this value in
	// cumulative probability space.
	double cumulativeProbabiltySpace = 0.0;
	for (auto it = choices.begin(); it != choices.end(); ++it)
	{
		cumulativeProbabiltySpace += it->weighting;
		if (randSample < cumulativeProbabiltySpace)
		{
			T value = it->value;
			
			// Move the last choice in place of the popped choice and shrink
			// the choices vector.
			*it = choices.pop_back();
			
			return value;
		}
	}
	
	throw std::length_error("tracked proabiltySpaceSize is larger than actual "
		"value in WeightedRandomizedStack.");
}

//------------------------------------------------------------------------------
// Returns an item with a probability according to its recorded weighting.
// non-const because of random mechanism
template <class T>
const T& WeightedRandomizedStack<T>::peak()
{
	// Check there's an item to return.
	if (isEmpty())
		throw std::logic_error("Attempted to retrieve an item from an empty"
			"WeightedRandomizedStack");
			
	// Select a random number within probability space
	const double randSample = this->distribution(this->generator)
							  * this->probabilitySpaceSize;
	
	// Find the index of the first element greater than this value in
	// cumulative probability space.
	double cumulativeProbabiltySpace = 0.0;
	for (auto it = choices.begin(); it != choices.end(); ++it)
	{
		cumulativeProbabiltySpace += it->weighting;
		if (randSample < cumulativeProbabiltySpace)
			return it->value;
	}
	
	throw std::length_error("tracked proabiltySpaceSize is larger than actual "
		"value in WeightedRandomizedStack.");
}