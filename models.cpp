#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iostream>
#include <fstream>
#include <conio.h>	
#include <cmath>

#include "models.h"

using namespace std;





//******************************************************************************
//******************************************************************************
//
//  Parameter setting for the storage capacity
//
//******************************************************************************
//******************************************************************************

//The maximum capacity (maximum number of characters allowed) 
//		for the storing the vocabulary set
#define VocabularyStorageLimit 20000

//The maximum capacity (maximum number of characters allowed) 
//		for storing the corrupted words during spelling recognition
#define NoisyWordsStorageLimit 15000





//******************************************************************************
//******************************************************************************
//
//  Parameter settings for the Spelling model
//
//******************************************************************************
//******************************************************************************
double prSpRepeat = 0.2;
//The probability of repeating the current cognitive state again as the next state
//we make it 0.2 initially, but you can try different values to see the effects.

double prSpMoveOn = 0.8;
//The probability of moving from the current cognitive state to other states
//	as the next state
//we make it 0.8 initially, but you can try different values to see the effects.

//********************************************************
//Note that prSpRepeat + prSpMoveon should always equal 1
//********************************************************

double spDegenerateTransitionDistancePower = 2;
//The likelihood of moving from the cognitive state of typing some character in a word 
//to the next cognitive state is proportion to the inverse of 
//(spDegenerateTransitionDistancePower) to the 
//(the distance between the current state to the next state)th power.
//In the setting of the original spelling model in the project,
//we make it 2, but you can try different values to see the effects.

double spDegenerateInitialStateDistancePower = 2;
//The likelihood of some character in a word as the initial cognitive state
//is proportion to the inverse of 
//(spDegenerateInitialStateDistancePower) to the 
//(the position of the character in the word)th power.
//In the setting of the original spelling model in the project,
// spDegenerateInitialStateDistancePower and spDegenerateTransitionDistancePower
//have the same value, but you can make them different to see the effects
//By default, we make it 2, but you can try different values to see the effects.


void setParametersSpellingModel()
{
	cout << endl
		 << "Reset the parameters of the spelling model:" << endl << endl;

	cout << "Reset P_moveOn, the probability of moving on" << endl
		 << "   from the current cognitive state to other states:" << endl
		 << "P_moveOn = ";
	cin  >> prSpMoveOn;

	prSpRepeat = 1- prSpMoveOn;
	cout << endl 
		 << "Automatically reset P_repeat to 1-P_moveOn" << endl
		 << "P_repeat = " << prSpRepeat << endl;

	cout << endl 
		 << "Do you want to change the deg_sp? (y or n)" << endl;

	if ( getch() == 'y' )
	{
		cout << "Reset deg_sp, the power coefficient governing the probability of " << endl
			<< "   skipping from the current cognitive state to other states:" << endl
			<< "deg_sp = ";

		cin  >> spDegenerateTransitionDistancePower; 

		spDegenerateInitialStateDistancePower = spDegenerateTransitionDistancePower; 
	}
}

void displayParametersSpellingModel()
{
	cout << endl
		 << "Parameter values of the spelling model:" << endl
		 << "   P_repeat  = " << prSpRepeat << endl
		 << "   P_moveOn  = " << prSpMoveOn << endl
		 << "   deg_sp = " << spDegenerateTransitionDistancePower << endl << endl;
}

//******************************************************************************
//******************************************************************************
//
//  Parameter settings for the keyboard model
//
//******************************************************************************
//******************************************************************************

double prKbHit = 0.9;
//The probability that you want to type a character and you do successfully make it
//In the setting of the original keyboard model in the project,
//we make it 0.9, but you can try different values to see the effects.

double prKbMiss = 0.1;
//The sum of probabilities that you want to type a character but end in touching 
//a different character.
//we make it 0.1, but you can try different values to see the effects.

//*******************************************************
//Note that prKbHit + prKbMiss should always equal 1
//*******************************************************

//In the setting of the original keyboard model in the project,
//we make it 0.2, but you can try different values to see the effects.


double kbDegenerateDistancePower = 2;
//The likelihood you want to type a character but end in touching a different character
//is proportion to the inverse of 
//(kbDegenerateDistancePower) raised to the (distance between them) th power
//In the setting of the original keyboard model in the project,
//we make it 2, but you can try different constants to see the effects.


void setParametersKbModel()
{
	cout << endl
		 << "Reset the parameters of the keyboard model:" << endl << endl;

	cout << "Reset P_hit, the probability of hitting" << endl
		 << "   the right character wanted on the keyboard:" << endl
		 << "P_hit = ";
	cin  >> prKbHit;

	prKbMiss = 1- prKbHit;
	cout << endl 
		 << "Automatically reset P_miss to 1-P_hit" << endl
		 << "P_miss = " << prKbMiss << endl;

	cout << endl 
		 << "Do you want to change the deg_kb? (y or n)" << endl;

	if ( getch() == 'y' )
	{
		cout << "Reset deg_kb, the power coefficient governing the probability of " << endl
			<< "   skipping from the current cognitive state to other states:" << endl
			<< "deg_kb = ";

		cin  >> kbDegenerateDistancePower; 
	}
}

void displayParametersKbModel()
{
	cout << endl
		 << "Parameter values of the keyboard model:" << endl
		 << "   P_hit  = " << prKbHit << endl
		 << "   P_miss = " << prKbMiss << endl
		 << "   deg_kb = " << kbDegenerateDistancePower << endl << endl;
}


//******************************************************************************
//******************************************************************************
//
//  Trace flag and the alphabet table
//
//******************************************************************************
//******************************************************************************
bool traceON = true;   // output tracing messages


/************************************************************************/
//Calculate and return the probability of charGenerated actually typed
//given charOfTheState as the underlying cognitive state. 
/************************************************************************/
double prCharGivenCharOfState(char charGenerated, char charOfTheState)
{   // KEYBOARD STATE
    // CharGenerated = What we actually touched (typed)
    // CharOfTheState = What we want to type in our mind (our cognitive state)

	// The probability of miss contains all of the other 25 characters other than the charOfTheState
	// In order to calculate the common component, find the correct ratios
	// Convert the kbDegenrateDistancePower into a different variable type	
	double deg = int(kbDegenerateDistancePower);
	// Calculate the total ratio of all the miss components for a 26 character keyboard
	int totalRatio = 2 * ( pow(deg, 12) + pow(deg, 11) + pow(deg, 10) + pow(deg, 9) + pow(deg, 8) + pow(deg, 7) + pow(deg, 6) + pow(deg, 5) + pow(deg, 4) + pow(deg, 3) + pow(deg, 2) + pow(deg, 1) ) + 1;

	// Calculate the probability of the final component (probability of generating character 13 distance from the state character
	double probChar13 = prKbMiss / totalRatio;

	// Calculate the distance of the charGenerated (character typed) from the charOfTheState (character desired)
	int distance = charGenerated - charOfTheState;
	
	// DEBUG
	//cout << "The distance between " << charGenerated << " and " << charOfTheState << " is " << distance << endl;
	
	// Adjust for the circular design as well as negatives
	// if the distance is greater than 13, then it is closer going the other direction around the circle
	if (distance > 13) 
	{
		// by subtracting the distance from 26, the result will be the closest distance, less than 13
		distance = 26 - distance;
	}
	else if (distance < -13)
	{
		// this will yield a positive value less than 13, which is the closest distance
		distance = 26 + distance;
		cout << distance << endl;
	}
	else if (distance < 0)
	{
		// this will yield a positive value less than 13, which is the closest distance
		distance = distance * -1;
	}
	else
	{
		// HI ALVIN AND WILLIAM :D
	}

	//DEBUG
	//cout << "Corrected distance : " << distance << endl;

	// Calculate the probability
	double prob;
	if (distance == 0)
	{
		// if the distance is zero, then the character was hit correctly
		prob = prKbHit;
	}
	else if (distance == 13)
	{
		// if the distance is 13, then the furthest character was hit, which was calculated in an earlier step
		prob = probChar13;
	}
	else
	{
		// otherwise, the probability can be calculated using ratios 
		prob = probChar13 * pow(deg, (13 - distance));
	}

	return prob;
}


/************************************************************************/
//Calculate for each cognitive state excluding the special states I and F,
//	the probability of that cognitive state being the first cognitive state
//	after the transition out of the special state I.
//Note that the value of the parameter sizeOfTable should be
//	exactly the number characters in the word.
//Store these prbabilities in the prTable array.
/************************************************************************/
void getPrTableForPossibleInitialStates(double prTable[], int sizeOfTable)
{
	//It is a word of sizeOfTable characters: 
	//	i.e. the number of character states is sizeOfTable.
	//	let's index these chracters from 0 to sizeOfTable-1.
	//

	//First calculate the sum of ratios of probabilities of
	//	going from the I state into these character states.
	double sumRatio = 0;
	double probLast = 0;
	for (int i = 0; i < sizeOfTable; i++)
	{
		sumRatio += pow(2, i);
	}
	probLast = 1 / sumRatio;

	//Second, for each character state calculate the probability 
	//	transitioning from the special I state into the character state.
	for (int i = 0; i < sizeOfTable; i++)
	{
		prTable[i] = probLast * pow(2, sizeOfTable - 1 - i);
	}
	
}




/************************************************************************/
//Calculate for each actual cognitive state for a word 
//	(excluding the special I state), 
//	the probability of that cognitive state being the next cognitive state
//	given that currentState is the index of the current state.
//Note that the value of the parameter sizeOfTable should be
//	1 + the number characters in the word,
//Store these prbabilities in the transitionPrTable[] array.
/************************************************************************/
void getPrTableForPossibleNextStates
(double transitionPrTable[], int sizeOfTable, int currentState)
{
	//It is a word of sizeOfTable characters: 
	//	i.e. the number of character states is sizeOfTable.
	//	index them from 0 to sizeOfTable-1.
	//currentState is the index of the current state in the word

	//First calculate the sum of ratios of probabilities of
	//	going from the current state into other down-stream states down in word
	//	including all the down-stream character states and the
	//	special F final state.

	// Declare variables for the sum ratio and probability of the final
	double sumRatio = 0;
	double probF= 0;
	
	// DEBUG
	//cout << "The current state is: " << currentState << endl << endl;
	//cout << "The size of the table is : " << sizeOfTable << endl << endl;
	
	// Create a loop to iterate through the states to calculate the ratio
	// This should adjust to shrink with each iteration as the current state progresses - halt the loop based on the current state
	// The i is used as an exponentiation variable and will be incremented
	for (int i = 0, j = currentState; j < sizeOfTable - 1; i++, j++)
	{
		// The sums are based on powers of two
		sumRatio += pow(2, i);
	}

	// Calculate the probability of advancing to the Final state from the given state
	probF = .8 / sumRatio;

	// DEBUG
	//cout << "The sumRatio is " << sumRatio << endl;
	//cout << "The prob of the final is " << probF << endl;

	//Second, for each state (excluding the special I state)
	//	calculate the probability of
	//	transitioning from the current state into the character state
	//	and store the probability into the table.
	
	// Iterate through the values to assign the correct probabilities
	for (int i = 0; i < sizeOfTable; i++)
	{
		if (i == currentState)
		{
			// If i equals the current state then the probability of repeating the character is .2
			transitionPrTable[i] = .2;
		}
		else if (i > currentState)
		{
			// If i is greater, then the probability is based on the ratio
			transitionPrTable[i] = probF * pow(2, sizeOfTable - 1 - i);
		}
		else
		{
			// If i is less, then it is a character that has been passed and the probability should be zero
			transitionPrTable[i] = 0;
		}
	}

}


