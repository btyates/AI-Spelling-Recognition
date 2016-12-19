#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iterator>
#include <iostream>
#include <fstream>
#include <conio.h>	
#include <string>
#include <vector>
#include <cmath>
#include <stdio.h>
#include <math.h>
#include <sstream>
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

	if ( _getch() == 'y' )
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

double prKbHit = 0.6;
//The probability that you want to type a character and you do successfully make it
//In the setting of the original keyboard model in the project,
//we make it 0.9, but you can try different values to see the effects.

double prKbMiss = 0.4;
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

	if ( _getch() == 'y' )
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
//  Programming #1 
//
//  List below are function prototypes of functions required 
//		to be implemented in Programming #1.
//	Replace the implementation of the following functions with 
//		your own code from Programmin #1A and #1B.
//
/************************************************************************/

/************************************************************************/
//Calculate and return the probability of charGenerated actually typed
//given charOfTheState as the underlying cognitive state. 
/************************************************************************/
double prCharGivenCharOfState(char charGenerated, char charOfTheState)
{   // KEYBOARD STATE
    // CharGenerated = What we actually touched (typed)
    // CharOfTheState = What we want to type in our mind (our cognitive state)
	int deg = int(kbDegenerateDistancePower);
	// Calculate the total ratio of all the miss components for a 26 character keyboard
	int totalRatio = int(2 * (pow(deg, 12) + pow(deg, 11) + pow(deg, 10) + pow(deg, 9) + pow(deg, 8) + pow(deg, 7) + pow(deg, 6) + pow(deg, 5) + pow(deg, 4) + pow(deg, 3) + pow(deg, 2) + pow(deg, 1)) + 1);

	// Calculate the probability of the final component (probability of generating character 13 distance from the state character
	double scalingVar = prKbMiss / totalRatio;

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
		prob = scalingVar;
	}
	else
	{
		// otherwise, the probability can be calculated using ratios 
		prob = scalingVar * pow(deg, (13 - distance));
	}

	return prob;
}


/************************************************************************/
//Calculate for each cognitive state excluding the special states I and F,
//     the probability of that cognitive state being the first cognitive state
//     after the transition out of the special state I.
//Note that the value of the parameter sizeOfTable should be
//     exactly the number characters in the word.
//Store these prbabilities in the prTable array.
/************************************************************************/
void getPrTableForPossibleInitialStates(double prTable[], int sizeOfTable)
{
	//It is a word of sizeOfTable characters:
	//     i.e. the number of character states is sizeOfTable.
	//     let's index these characters from 0 to sizeOfTable-1.
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
//     (excluding the special I state),
//     the probability of that cognitive state being the next cognitive state
//     given that currentState is the index of the current state.
//Note that the value of the parameter sizeOfTable should be
//     1 + the number characters in the word,
//Store these probabilities in the transitionPrTable[] array.
/************************************************************************/
void getPrTableForPossibleNextStates
(double transitionPrTable[], int sizeOfTable, int currentState)
{
	//We are working on a word of sizeOfTable-1 characters:
	//     i.e. the number of character states is sizeOfTable-1.
	//Index these character states from 0 to sizeOfTable-2 respectively, while
	//     the index of the special final state F is sizeOfTable-1.
	//currentState is the index of the current state in the word

	//First calculate the sum of ratios of probabilities of
	//     going from the current state into the other down-stream states down in word
	//     including all the down-stream character states and the
	//     special F final state.
	//This allows you to calculate the scaling factor to determine the probabilities.

	// Declare variables for the sum ratio and probability of the final
	double sumRatio = 0;
	double probF = 0;

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
	probF = prSpMoveOn / sumRatio;

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
			transitionPrTable[i] = prSpRepeat;
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




/************************************************************************/
//  Programming #2 
//
//  List below are function prototypes of functions given  or required 
//		to be implemented in Programming #2 
//
/************************************************************************/

/************************************************************************/
//Given the probabilities (of sizeOfTable elements) stored in prTable,
//	try to randomly take a sample out of sizeOfTable elements
//	according to the probabilities of sizeOfTable elements;
//Return the index of the non-deterministically sampled element.
/************************************************************************/
int take1SampleFrom1PrSpace(double prTable[], int sizeOfTable)
{ int i;
  double prSum=0;
  for (i=0; i<sizeOfTable; i++)
    prSum += prTable[i];
  if (prSum < 0.999 || prSum > 1.001)
		cout << "Something is wrong with a random sampling" << endl
		<< "The sum of all probabilities in the table is " << prSum  << endl;

   //Calculate the probability intervals of the characters
  double prAccumulated = 0;
  double * prIntervals;
  prIntervals = new double[sizeOfTable];
  for (i=0; i<sizeOfTable; i++)
  { prAccumulated = prAccumulated + prTable[i];
    prIntervals[i] = prAccumulated;
  }

  /*
  cout << endl;
  for (i=0; i<sizeOfTable; i++)
  { if (i>0) 
		cout << "state " << i << " lower bounded by " << prIntervals[i-1]<< endl;
    cout << "state " << i << " upper bounded by " << prIntervals[i]<< endl;
  }
  */

  // Generate a random number in [0,1]
  i = rand()% 1001; 
  double temp = i / 1000.0;
  //cout << "The random number pair generated is " << i << endl << temp << endl;

  bool sampleTaken = false;
  for (i=0; i<sizeOfTable && !sampleTaken; i++)
	if (temp <= prIntervals[i])
	{   delete [] prIntervals;
		//cout << "The random interval id is " << i << endl;
		return i;
	}
  return sizeOfTable-1;
}





/************************************************************************/
//
//Given the character to type (charToType) 
//	(assuming that the 1D keyboard of 26 keys is used),
//	(assuming that prTable[] for storing 26 probabilities),
//	calculate pr(charGenerated = 'a' | charToType),
//	calculate pr(charGenerated = 'b' | charToType), 
//	...
//	calculate pr(charGenerated = 'z' | charToType), and
//	store these probabilities in prTable.
/************************************************************************/
void getKeyboardProbabilityTable(char charToType, double prTable[], char map[])
{
	// Assign asciiMap characters into an array
	// Calculate the probabilities
	for (int i = 0; i < 26; i++)
	{
		prTable[i] = prCharGivenCharOfState(map[i], charToType);
	}

	return;

}


/************************************************************************/
//Simulate the keyboard model:
//Given the charToType, simulate what character may actually
//	be typed and return it as the result.
/************************************************************************/
char typeOneChar(char charToType)
{
	//Remove and write your own code
	double prTable[26];
	char asciiMap[] = "abcdefghijklmnopqrstuvwxyz";
	getKeyboardProbabilityTable(charToType, prTable, asciiMap);

	int x = take1SampleFrom1PrSpace(prTable, strlen(asciiMap));

	return asciiMap[x];
}



/************************************************************************/
//Simulate the combination of the spelling model and the keyboard model:
//Given a word stored in the word array, simulate what may actually
//	be typed and store the result in the output array.
//maxOutput specifies the capacity limit of the output array, by default it is 100.
//When traceON is true (by default it is false), extra outputs are provided as traces.
/************************************************************************/
void typeOneWord(char word[], char output[], bool traceON, int maxOutput)
{ 
	// PATTERN: FILL A PROBABILITY TABLE THEN SIMULATE

	// Assign variables
	int i, j = 0; // Index of word - tracks current state
	char asciiChar;

	int wordSize = strlen(word); // Find the size of the word
	double * iniTable; // Create an probability table corresponding to the word 
	iniTable = new double[wordSize];
	
	// DEBUG
	//cout << "Word size is: " << wordSize << endl << endl;

	// Calculate the probabilities for the possible initial states and fill the table
	getPrTableForPossibleInitialStates(iniTable, wordSize);

	// Simulate the initial state using the table probabilities 
	i = take1SampleFrom1PrSpace(iniTable, wordSize); // Returns the state chosen from simulation
	
	// DEBUG
	//cout << "Initial state chosen - position: " << i << endl << endl;
	//cout << "Character state : " << word[i] << endl << endl;
	
	// Simulate typing the character 
	asciiChar = typeOneChar(word[i]);
	
	if (traceON)
	{
		cout << "'" << asciiChar << "' pressed, Current state: " << word[i] << ", " << i << " ";
	}

	//cout << "Character typed : " << asciiChar << endl << endl;
	
	// Assign the character to the output stream
	output[j] = asciiChar; 
	
	// For transitional states, the table needs to be one size larger than the word to simulate the final state
	wordSize++;
	double * nexTable;
	nexTable = new double[wordSize];

	// Iterate through the rest of the characters in the word
	// Stop looping once the word is iterated through or the max output is reached
	// i tracks the word state position and j tracks the output position
	for (j = 1; i < wordSize - 1 && j < maxOutput; j++)
	{
		// Get the probabilities for the next possible states, following the current position i
		getPrTableForPossibleNextStates(nexTable, wordSize, i);

		// Simulate the next state transition using the table probabilities
		i = take1SampleFrom1PrSpace(nexTable, wordSize); // Returns the state chosen from simulation
		
		if (traceON)
		{
			if (i == wordSize - 1)
			{
				cout << "The next state: The final state" << endl;
			}
			else 
			{
				cout << "The next state: " << word[i] << ", " << i << endl;
			}	
		}

		if (i < wordSize - 1)
		{
			// Simulate typing the character
			asciiChar = typeOneChar(word[i]);
			//cout << "Character types : " << asciiChar << endl << endl;
			output[j] = asciiChar;

			if (traceON)
			{
				cout << "'" << asciiChar << "' pressed, Current state: " << word[i] << ", " << i << " ";
			}

		}
		else
		{
			output[j] = '\0';
		}
	}
	return;
}// end of the function
/*******************************************************************/

/* Programming #2C */

void typeOneArticle(char * corruptedMessageFile, char * sourceArticle, bool trace)
{
	/*
	Function overview
	corruptedMesageFile and sourceArticle are two character arrays to store the names of two files,
	trace is a boolean flag, output traces to the screen for your debuging purpose when it is true; otherwise keep silent
	*/
	// Variables
	string line; // temp string placeholder
	//char word[21]; // input word
	char output[101]; // output word
	char word[21];
	int maxOutput = 100; // output limit
	int size = 0;

	// Assign File handlers
	ifstream inputFile;
	ofstream outputFile;

	// Get filename stored in sourceArticle and corruptedMessageFile
	// Open the files, sourceArticle for input and corruptedMessage for output
	inputFile.open(sourceArticle);
	outputFile.open(corruptedMessageFile);

	// check the file is open
	if (inputFile.is_open())
	{
		// Read the lines (words) one by one from the file sourceArticle:
		while (getline(inputFile, line))
		{
			// Get the size of the input string
			size = line.size();
			//cout << "Line size is " << size << endl;

			// Clear the word array by assigning null characters
			for (int i = 0; i < 21; i++)
			{
				word[i] = '\0';
			}

			// Convert the string into a character array
			for (int i = 0; i < size; i++)
			{
				// Account for the null character at the end of the string
				if (i == size - 1)
				{
					word[i] = '\0';
				}
				// Otherwise copy the character at the given index
				else
				{
					word[i] = line[i];
				}
			}

			if (trace)
			{
				cout << "Simulate the typing of this word: " << word << endl;
			}
			// For each word/iteration:
			//i) simulate how the word may be typed accordin to the spelling model and the keyboard model, and then
			typeOneWord(word, output, trace, maxOutput);
			//ii) store the resulting wod into the file corruptedMessageFile
			if (outputFile.is_open())
			{
				outputFile << output << "\n";
			}
			else
			{
				if (trace)
				{
					cout << "Unable to open output file" << endl;
				}
			}
		}
	}
	else
	{
		if (trace)
		{
			cout << "Unable to open source file" << endl;
		}
	}
	// Close the files
	outputFile.close();
	inputFile.close();

	return;

}

double prOf1CharSeriesWhenTyping1Word(string observedString, string wordString)
{
	/*
	The function should calculate and return the probability of getting a 
	series of characters stored in the string object observedString
	when you want to type the word stored in the string object wordString
	*/

	double probability = 0; // final answer probability
	int wordSize = wordString.size();
	int observedSize = observedString.size();

	// Note: the length of the string is the exact same as the input, there are no garbage characters

	// get table of possible initial states
	double * iniTable; // Create an probability table corresponding to the word 
	iniTable = new double[wordSize];
	getPrTableForPossibleInitialStates(iniTable, wordSize);
	
	// get table of possible next states
	int transSize = wordSize + 1;
	double * transTable; // Create an probability table corresponding to the word 
	transTable = new double[transSize];
	//currentState = 0;
	//prSum = 0;

	//create the complete transitional probability table + initial state table
	vector<vector<double>> transitionPrTable(transSize);

	//add the initial state probabilities 
	for (int i = 0; i < wordSize; i++)
	{
		transitionPrTable[0].push_back(iniTable[i]);
	}
	// add the final zero to the initial row
	transitionPrTable[0].push_back(0);

	for (int currentState = 1; currentState < transSize; currentState++)
	{
		getPrTableForPossibleNextStates
		(transTable, transSize, currentState - 1);

		for (int i = 0; i < transSize; i++)
		{
			transitionPrTable[currentState].push_back(transTable[i]);
		}
	}

	// table for storing the state probability table
	vector < vector <double> > viterbiTable(wordSize);

	// initialize the vector with zeros
	for (int row = 0; row < wordSize; row++)
	{
		for (int sizer = 0; sizer < observedSize; sizer++)
		{
			viterbiTable[row].push_back(0);
		}
	}

	int sum = 0;

	// Now the primary iteration of the forward backward algorithm
	for (int col = 0; col < observedSize; col++)
	{
		for (int row = 0; row < wordSize; row++)
		{
			if (col == 0)
			{
				viterbiTable[row][col] = transitionPrTable[col][row] * prCharGivenCharOfState(observedString[col], wordString[row]);
			}
			else
			{
				for (int transIndex = 0; transIndex < row + 1  ; transIndex++)
				{
					viterbiTable[row][col] = viterbiTable[row][col] + transitionPrTable[transIndex + 1][row] * viterbiTable[transIndex][col - 1];
				}
				viterbiTable[row][col] = viterbiTable[row][col] * prCharGivenCharOfState(observedString[col], wordString[row]);
			}
		}
	}

	/*for (int row = 0; row < wordSize; row++)
	{
		for (int col = 0; col < observedSize; col++)
		{
			cout << viterbiTable[row][col] << " ";
		}
		cout << endl;
	}*/

	// calculate the final probability
	int row = wordSize;
	int col = observedSize;

	for (int transIndex = 0; transIndex < row; transIndex++)
	{
		probability = probability + transitionPrTable[transIndex + 1][row] * viterbiTable[transIndex][col - 1];
	}

	return probability;

}

double logPrOfGettingDocument1WhenTypingDocument2(string document1, string document2, string select, bool debug)
{
	// Determine for each document d who is the most likely person who has generated the document d when trying to type the Biola vision statement. 

	// For each document, we should for each person determine the probabilty that the document is the resulting text when that person tried to type 
	// the Biola vision statement. We can then compare these probabilities and the most likely person is simply the person p with the highest probability for the document

	// Let document 1 be name of d the typed vision statement
	// Let document 2 be name of p the biola vision statement

	// Pr(d|p) = Pr(d1|w1,p)*Pr(d2|w2,p)*...*Pr(dn|wn,p)

	//char word[21];
	double sumLog = 0;
	double sumLog10 = 0;
	double temp = 0;
	string line1;
	string line2;

	// Assign File handlers
	ifstream file1(document1);
	ifstream file2(document2);

	//file1.open(document1);
	//file2.open(document2);
		
	// check the files is open
	if (file1.is_open() && file2.is_open())
	{
		// Read the lines (words) one by one from the files
		while (getline(file1, line1) && getline(file2, line2))
		{
			// use logs to retain numerical precision
			// for each pair di and wi, calcuate Pr(di|wi,p) and logPr(di|wi,p)
			if (debug)
			{
				cout << "Add log Pr(" << line1 << "|" << line2 << ")" << endl;
			}
			sumLog = sumLog + log(prOf1CharSeriesWhenTyping1Word(line1, line2));
			sumLog10 = sumLog10 + log10(prOf1CharSeriesWhenTyping1Word(line1, line2));
		}
	}
	else
	{
		cout << "Unable to open source file" << endl;
	}
	// Close the files
	file1.close();
	file2.close();

	if (select == "e")
	{
		return sumLog;
	}
	else
	{
		return sumLog10;
	}
}

void learnParameters(string corruptedText, string originalText)
{
	double parameterProbability = -1000;
	double candidateParameterProb = 0;
	double hitParam = 0;
	double moveParam = 0;

	// vary Pr(repeat) and Pr(hit) on 0 - 1 with .01 increments

	// set the parameters
	for (double i = 0; i < 1.01; i += 0.1)
	{
		// set hit parameter
		prKbHit = i;
		prKbMiss = 1 - prKbHit;
		for (double j = 0; j < 1.01; j += 0.1)
		{
			// set move parameter
			prSpMoveOn = j;
			prSpRepeat = 1 - prSpMoveOn;

			// calculate the probability of generating the corrupted message given the parameters
			candidateParameterProb = logPrOfGettingDocument1WhenTypingDocument2(corruptedText, originalText, "e", false);

			// check the probability
			if (candidateParameterProb > parameterProbability)
			{
				parameterProbability = candidateParameterProb;
				hitParam = prKbHit;
				moveParam = prSpMoveOn;
			}
		}
	}

	// Assign the final parameters based on the highest probability
	prKbHit = hitParam;
	prKbMiss = 1 - prKbHit;
	prSpMoveOn = moveParam;
	prSpRepeat = 1 - prSpMoveOn;

	cout << "The most likely parameters for typing the corrupted message given the original message are: " << endl
		<< "prKbHit = " << prKbHit << endl
		<< "prKbMiss = " << prKbMiss << endl
		<< "prSpMoveOn = " << prSpMoveOn << endl
		<< "prSpRepeat = " << prSpRepeat << endl;
}

void tryToRecoverMessageFromCorruptedMessage(string corruptedText, string vocabularyText, string recoveredText)
{
	//the words in message X are contained in the vocabulary.txt.
	// option R should automatically recover message X from the corrupted result in corruptedMessage1.txt

	// placeholder varialbes
	string line1; // for reading in lines from corruptedMessage
	string line2; // for reading in lines from vocabulary
	string tempCandidateWord; // for storing vocabulary words
	float tempCandidatePr; // for storing the probabilities associated with each vocabulary word
	
	// Assign File handlers (and open files)
	ifstream message(corruptedText);
	ifstream vocab(vocabularyText);
	ofstream output(recoveredText);
	//close the vocab file
	vocab.close();

	// save the candidate vocabulary words and corresponding probabiilties for each word 
	vector <string> candidateWord;
	vector <float> candidatePr;

	// IMPLEMENTATION
	// For each corrupted string s in corruptedMessage1.txt,

	// check the files is open
	if (message.is_open())
	{
		//cout << "Files opened" << endl;
		// check for each word w in the vocabulary.txt the probability Pr ( s | w,X) that Mr. X would generate the corrupted string s when Mr. X types the word w=

		// Read the lines (words) one by one from the corrupted file
		while (getline(message, line1))
		{
			//cout << "text line is " << line1 << endl; 
			// check the vocabulary file line by line
			vocab.open(vocabularyText);
			while (getline(vocab, line2))
			{
				//cout << "vocab word is " << line2 << endl;
				// use logs to retain numerical precision
				// for each pair di and wi, calcuate Pr(di|wi,p) and logPr(di|wi,p)
				// store the candidate word and probability
				candidateWord.push_back(line2);
				tempCandidatePr = log(prOf1CharSeriesWhenTyping1Word(line1, line2));
				candidatePr.push_back(tempCandidatePr);
			}
			vocab.close();

			// find the top 4 candidate words with the highest Pr(s | w,X)
				// sort the vectors in descending order from the highest probability words
			for (int counter = 0; counter < candidateWord.size(); counter++)
			{
				for (int i = 0; i < candidateWord.size() - 1; i++)
				{
					if (candidatePr[i] < candidatePr[i + 1])
					{
						// sort the probability vector
						tempCandidatePr = candidatePr[i + 1];
						candidatePr[i + 1] = candidatePr[i];
						candidatePr[i] = tempCandidatePr;

						// sort the word vector correspondingly
						tempCandidateWord = candidateWord[i + 1];
						candidateWord[i + 1] = candidateWord[i];
						candidateWord[i] = tempCandidateWord;
					}
				}
			}

			//cout << "Candidazte word size is " << candidateWord.size() << endl << endl;

			// save these 4 words on a separate line in the output file recoveredMessage_V1.txt to indicate they are the most likely candidates 
			// regarding the corresponding actual word in message X based on the evicdence of the ocrrupted string s
				// output the first four words to represent the four words with the highest probability
			if (output.is_open())
			{
				//cout << "Output file is open" << endl;
				for (int i = 0; i < 4; i++)
				{
					output << candidateWord[i] << " ";
				}
				output << "\n";
			}

			// now clear the vectors
			candidateWord.clear();
			candidatePr.clear();
		}
		// Close the files
		message.close();
		output.close();
	}
	else
	{
		cout << "Unable to open source file" << endl;
	}

}

void tryToRecoverMessageFrom2CorruptedMessage(string corruptedText1, string corruptedText2, string vocabularyText, string recoveredText)
{
	//the words in message X are contained in the vocabulary.txt.
	// option R should automatically recover message X from the corrupted result in corruptedMessage1.txt

	// placeholder varialbes
	string messageLine1; // for reading in lines from corruptedMessage
	string messageLine2; // for reading in lines from vocabulary
	string vocabLine;
	string tempCandidateWord;
	float tempCandidatePr;
	float tempCandidatePr1; // for storing the probabilities associated with each vocabulary word
	float tempCandidatePr2;

	// Assign File handlers (and open files)
	ifstream message1(corruptedText1);
	ifstream message2(corruptedText2);
	ifstream vocab(vocabularyText);
	ofstream output(recoveredText);
	//close the vocab file
	vocab.close();

	// save the candidate vocabulary words and corresponding probabiilties for each word 
	vector <string> candidateWord;
	vector <float> candidatePr;

	// check the files is open
	if (message1.is_open() && message2.is_open())
	{
		//cout << "Files opened" << endl;
		// check for each word w in the vocabulary.txt the probability Pr ( s | w,X) that Mr. X would generate the corrupted string s when Mr. X types the word w=

		// Read the lines (words) one by one from the corrupted file
		while (getline(message1, messageLine1) && getline(message2, messageLine2))
		{
			//cout << "text line is " << line1 << endl; 
			// check the vocabulary file line by line
			vocab.open(vocabularyText);
			while (getline(vocab, vocabLine))
			{
				//cout << "vocab word is " << line2 << endl;
				// use logs to retain numerical precision
				// for each pair di and wi, calcuate Pr(di|wi,p) and logPr(di|wi,p)
				// store the candidate word and probability
				candidateWord.push_back(vocabLine);
				tempCandidatePr1 = log(prOf1CharSeriesWhenTyping1Word(messageLine1, vocabLine));
				tempCandidatePr2 = log(prOf1CharSeriesWhenTyping1Word(messageLine2, vocabLine));
				tempCandidatePr = tempCandidatePr1 + tempCandidatePr2;
				candidatePr.push_back(tempCandidatePr);
			}
			vocab.close();

			// find the top 4 candidate words with the highest Pr(s | w,X)
			// sort the vectors in descending order from the highest probability words
			for (int counter = 0; counter < candidateWord.size(); counter++)
			{
				for (int i = 0; i < candidateWord.size() - 1; i++)
				{
					if (candidatePr[i] < candidatePr[i + 1])
					{
						// sort the probability vector
						tempCandidatePr = candidatePr[i + 1];
						candidatePr[i + 1] = candidatePr[i];
						candidatePr[i] = tempCandidatePr;

						// sort the word vector correspondingly
						tempCandidateWord = candidateWord[i + 1];
						candidateWord[i + 1] = candidateWord[i];
						candidateWord[i] = tempCandidateWord;
					}
				}
			}

			// save these 4 words on a separate line in the output file recoveredMessage_V1.txt to indicate they are the most likely candidates 
			// regarding the corresponding actual word in message X based on the evicdence of the ocrrupted string s
			// output the first four words to represent the four words with the highest probability
			if (output.is_open())
			{
				//cout << "Output file is open" << endl;
				for (int i = 0; i < 4; i++)
				{
					output << candidateWord[i] << " ";
				}
				output << "\n";
			}

			// now clear the vectors
			candidateWord.clear();
			candidatePr.clear();
		}
		// Close the files
		message1.close();
		message2.close();
		output.close();
	}
	else
	{
		cout << "Unable to open source file" << endl;
	}

}

void createQualityReport(string recoveredFile1, string recoveredFile2, string masterFile, string qualityReportFile)
{
	// Assign File handlers (and open files)
	ifstream list1(recoveredFile1);
	ifstream list2(recoveredFile2);
	ifstream master(masterFile);
	ofstream output(qualityReportFile);

	string tempString;
	string buf;
	vector <vector <string> > topWordsFile1;
	vector <vector <string> > topWordsFile2;
	vector <string> masterString;

	// load all the words from the master line
	while (getline(master, tempString))
	{
		masterString.push_back(tempString);
	}

	// get the size of the master document
	int size = masterString.size();

	// load all the candidate words from the 1st list
	int i = 0; // use i as an index for the outer vector
	topWordsFile1.resize(size);
	while (getline(list1, tempString))
	{
		stringstream ss(tempString); //Insert string into a stream
		while (ss >> buf)
			topWordsFile1[i].push_back(buf);
		i++;
	}

	// load all the candidate words from the 1st list 
	i = 0; // use i as an index for the outer vector
	topWordsFile2.resize(size);
	while (getline(list2, tempString))
	{
		stringstream ss(tempString); //Insert string into a stream
		while (ss >> buf)
		{
			topWordsFile2[i].push_back(buf);
		}
		i++;
	}

	int list1top1 = 0;
	int list1top2 = 0;
	int list1top3 = 0;
	int list1top4 = 0;

	// compare strings
	for (int j = 0; j < size; j++)
	{
		if (masterString[j] == topWordsFile1[j][0])
		{
			list1top1++;
		}
		else if (masterString[j] == topWordsFile1[j][1])
		{
			//cout << topWordsFile1[j][1] << endl;
			list1top2++;
		}
		else if (masterString[j] == topWordsFile1[j][2])
		{
			//cout << topWordsFile1[j][2] << endl;
			list1top3++;
		}
		else if (masterString[j] == topWordsFile1[j][3])
		{
			//cout << topWordsFile1[j][3] << endl;
			list1top4++;
		}
	}

	// catch the results from the previous list;
	list1top2 = list1top1 + list1top2;
	list1top3 = list1top2 + list1top3;
	list1top4 = list1top3 + list1top4;

	int list2top1 = 0;
	int list2top2 = 0;
	int list2top3 = 0;
	int list2top4 = 0;

	// compare strings
	for (int j = 0; j < size; j++)
	{
		if (masterString[j] == topWordsFile2[j][0])
		{
			list2top1++;
		}
		else if (masterString[j] == topWordsFile2[j][1])
		{
			list2top2++;
		}
		else if (masterString[j] == topWordsFile2[j][2])
		{
			list2top3++;
		}
		else if (masterString[j] == topWordsFile2[j][3])
		{
			list2top4++;
		}
	}

	// catch the results from the previous list;
	list2top2 = list2top1 + list2top2;
	list2top3 = list2top2 + list2top3;
	list2top4 = list2top3 + list2top4;

	double top1 = double(list1top1);
	double top2 = double(list1top2);
	double top3 = double(list1top3);
	double top4 = double(list1top4);

	double newSize = double(size);

	double percent1 = top1 / size;
	double percent2 = top2 / size;
	double percent3 = top3 / size;
	double percent4 = top4 / size;

	cout << "Regarding the quality of recoveredMessage1.txt:" << endl;
	cout << "Out of the " << size << " word in the original message" << endl;
	cout << list1top1 << " words recognized in the top 1 list , " << percent1 << "%" << endl;
	cout << list1top2 << " words recognized in the top 2 list , " << percent2 << "%" << endl;
	cout << list1top3 << " words recognized in the top 3 list , " << percent3 << "%" << endl;
	cout << list1top4 << " words recognized in the top 4 list , " << percent4 << "%" << endl;

	output << "Regarding the quality of recoveredMessage1.txt:" << '\n' << endl;
	output << "Out of the " << size << " word in the original message" << '\n' << endl;
	output << list1top1 << " words recognized in the top 1 list , " << percent1 << "%" << '\n' << endl;
	output << list1top2 << " words recognized in the top 2 list , " << percent2 << "%" << '\n' << endl;
	output << list1top3 << " words recognized in the top 3 list , " << percent3 << "%" << '\n' << endl;
	output << list1top4 << " words recognized in the top 4 list , " << percent4 << "%" << '\n' << endl;

	top1 = double(list2top1);
	top2 = double(list2top2);
	top3 = double(list2top3);
	top4 = double(list2top4);

	percent1 = top1 / newSize;
	percent2 = top2 / newSize;
	percent3 = top3 / newSize;
	percent4 = top4 / newSize;

	cout << "Regarding the quality of recoveredMessage2.txt:" << endl;
	cout << "Out of the " << size << " word in the original message" << endl;
	cout << list2top1 << " words recognized in the top 1 list , " << percent1 << "%" << endl;
	cout << list2top2 << " words recognized in the top 2 list , " << percent2 << "%" << endl;
	cout << list2top3 << " words recognized in the top 3 list , " << percent3 << "%" << endl;
	cout << list2top4 << " words recognized in the top 4 list , " << percent4 << "%" << endl;


	output << "Regarding the quality of recoveredMessage2.txt:" << '\n' << endl;
	output << "Out of the " << size << " word in the original message" << '\n' << endl;
	output << list2top1 << " words recognized in the top 1 list , " << percent1 << "%" << '\n' << endl;
	output << list2top2 << " words recognized in the top 2 list , " << percent2 << "%" << '\n' << endl;
	output << list2top3 << " words recognized in the top 3 list , " << percent3 << "%" << '\n' << endl;
	output << list2top4 << " words recognized in the top 4 list , " << percent4 << "%" << '\n' << endl;

}
