#include <ctime>
#include <cstring>
#include <string>
#include <iostream>
#include <conio.h>	
//For windows, include <conio.h> to use _getch().
//Instead of using _getch() and <conio.h>, 
//	you can declare a chacter variable "c" and use "cin >> c"
//  to read in a character.

using namespace std;

#include "models.h"
//to use the implementation of the models, you have to include <conio.h>


/************************************************************************/
/************************************************************************/
void demoOfKeyboardModel()
{
	cout << "Welcom Mr. Suh, congratulations on graduating..." << endl;
	cout << endl << endl 
		 << "*********************************************************" << endl
		 << "Test the correctness of the keyboard model implementation, and" << endl
		 << "show the probabilities of generating (touching) various characters" << endl 
		 << "when mentally you are in a state of trying to type something." << endl
		 << "*********************************************************" << endl; 
	cout<<endl<<"Press space to continue."<<endl;	

	_getch();
	char asciiMap[] = "abcdefghijklmnopqrstuvwxyz";
	for (int i=0; i< 26; i++)
	{double prSum;
	 prSum = 0;
	 cout << "Probability distribution regarding " << endl
		  << "all possible resulting characters when trying to type "
		  << asciiMap[i] << endl;
	 for (int j=0; j< 26; j++)
	{	
			cout << '\t' 
				 << "Pr[Char=" << asciiMap[j] << "|State=" << asciiMap[i] << "]="
				 << prCharGivenCharOfState(asciiMap[j], asciiMap[i]) << endl;
			prSum += prCharGivenCharOfState(asciiMap[j], asciiMap[i]);
	}
	 cout << "The sum of probability distribution over " << endl
		  << "all possible resulting characters when trying to type \'" << endl
		  << asciiMap[i] 
		  << "\' has been verified as " 
		  << prSum  << endl << endl;

	 cout<<endl<<"Press space to continue."<<endl;		
	 _getch();

	}
	cout << endl << endl ;
}



/************************************************************************/
/************************************************************************/
void demoInitialProbaility()
{
	
	cout << endl << endl 
		 << "*********************************************************" << endl
		 << "Consider the 3-character word \"his\" "<< endl
		 << "Demonstrate the probability distribution over the possible cognitive states" << endl
		 << "after the transition from the special initial state I."  << endl
		 << "*********************************************************" << endl; 
	cout<<endl<<"Press space to continue."<<endl;		
	_getch();

	int sizeOfTable = strlen("his");
	int currentState;
	double prSum = 0;
	double initialPrTable[3];
	getPrTableForPossibleInitialStates(initialPrTable,3);

	string word = "his";
	for (currentState=0; currentState < sizeOfTable; currentState++)
	{	cout << '\t' 
				 << "Pr[State=" << word[currentState]  << "]="
				 << initialPrTable[currentState] << endl;
			prSum += initialPrTable[currentState];
	}

	cout << "The sum of probability distribution over " << endl
		 << "all possible states after the first transition" << endl
		 << "has been verified as " << prSum  << endl << endl;
	cout<<endl<<"Press space to continue." << endl;		
	_getch();

}


/************************************************************************/
/************************************************************************/
void demoTransitionProbability()
{

	cout << endl << endl 
		 << "*********************************************************" << endl
		 << "Consider the 3-character word \"his\" "<< endl
		 << "Show the transition probabilities when spelling the word." << endl
		 << "*********************************************************" << endl
		 << "Demonstrate transition probabilities from current state to the next state."  << endl
		 << "We use special character \'_\' to represent the special final state F."  << endl
		 << "*********************************************************" << endl;  
	cout<<endl<<"Press space to continue."<<endl;		
	_getch();

	//One more example
	int sizeOfSpace = strlen("his")+1;
	string word="his_";
	for (int currentState=0; currentState< sizeOfSpace-1; currentState++)
	{	double prSum, transitionPrTable[4];
		cout << "Probability distribution over " << endl
			<< "all possible next states given that the current state is "
			<< word[currentState] << endl;

		prSum = 0;
		//nextStatePrTableGivenCurrentState(currentState,transitionPrTable,6);
		getPrTableForPossibleNextStates
		(transitionPrTable, 4, currentState);

		for (int nextState=0; nextState< sizeOfSpace; nextState++)
		{	cout << '\t' 
				 << "Pr[NextState=" << word[nextState] << "|CurrentState=" << word[currentState] << "]="
				 << transitionPrTable[nextState] << endl;
			prSum += transitionPrTable[nextState];
		}

		cout << "The sum of probabilities over " << endl
		  << "all possible next states given that CurrentState="
		  << word[currentState] << endl
		  << "is verified as " 
		  << prSum  << endl << endl;

		cout<<endl<<"Press space to continue."<<endl;		
		_getch();
	}


	//One more example
	cout << endl << endl 
		 << "*********************************************************" << endl
		 << "Consider the 5-character word \"right\" "<< endl
		 << "Show the transition probabilities when spelling the word." << endl
		 << "*********************************************************" << endl
		 << "Demonstrate transition probabilities from current state to the next state."  << endl
		 << "We use special character \'_\' to represent the special final state F."  << endl
		 << "*********************************************************" << endl;  
	cout<<endl<<"Press space to continue."<<endl;		
	_getch();
	sizeOfSpace = strlen("right")+1;
	word="right_";
	for (int currentState=0; currentState< sizeOfSpace-1; currentState++)
	{	double prSum, transitionPrTable[6];
		cout << "Probability distribution over " << endl
			<< "all possible next states given that the current state is "
			<< word[currentState] << endl;

		prSum = 0;
		//nextStatePrTableGivenCurrentState(currentState,transitionPrTable,6);
		getPrTableForPossibleNextStates
		(transitionPrTable, 6, currentState);

		for (int nextState=0; nextState< sizeOfSpace; nextState++)
		{	cout << '\t' 
				 << "Pr[NextState=" << word[nextState] << "|CurrentState=" << word[currentState] << "]="
				 << transitionPrTable[nextState] << endl;
			prSum += transitionPrTable[nextState];
		}

		cout << "The sum of probabilities over " << endl
		  << "all possible next states given that CurrentState="
		  << word[currentState] << endl
		  << "is verified as " 
		  << prSum  << endl << endl;

		cout<<endl<<"Press space to continue."<<endl;		
		_getch();
	}

}

void endOfService(const string service)
{
	// displayProgram();
	cout<<endl << service << " done." << endl << "Press any key to continue.";
	_getch();
	cout << endl;
	cout << "****************************************************" <<endl<< endl;

}

/************************************************************************/
/************************************************************************/
void main()
{
	srand( (unsigned)time( NULL ) );

	// "Then I suppose now is the time for me to say something profound....
	// Nothing comes to mind. Let's do it."

	bool inMenu = true;	// while the user is using the program this var is set to true and the menu keeps looping.
						// when the user wants to quit, it is set to false and the loop ends.
	char keyPressed;	// This var stores which menu selection the user picks.

	while ( inMenu )
	{
		cout<< endl << endl << endl;
		cout << "****************************************************" << endl;
		cout  <<"**  MENU:(press a character to select an option)  **" << endl;
 		cout << "****************************************************" << endl;
		cout<<"Q. [QUIT]     Quit."<<endl;
		cout << endl;

 		cout << "*************	Demos of the Models	*********" << endl;
		cout<<"K. Keyboard model demo"<<endl;
		cout<<"S. Spelling model demo"<<endl;
		cout<<"A. Simulate typing of a character and a word"<<endl;
		cout << "B. Simulate typing of Biola vision statements" << endl;
		cout<<"P. prOf1CharSeriesObservedWhenTyping1Word(string observed, string word)"<<endl;
		cout << "D. logPrOfGettingDocument2WhenTyping1Document2(document1, document2)" << endl;

		cout << "L. learnParametersFromDocument(corruptedText, originalText)" << endl;
		cout<<"R. Recover an original message from 1 corrupted version"<<endl;
		cout<<"T. Recover an original message from 2 corrupted versions" << endl << endl;
		
		cout << "U. Check the quality of the recovered messages" << endl;

 		cout << "*************	Setting Parameters	*********" << endl;
		cout<<"X. Reset the parameters of the spelling model" << endl;
		cout<<"Y. Reset the parameters of the keyboard model" << endl;
		cout<<"Z. Display the current parameter values" << endl;
		cout << "****************************************************" <<endl<< endl;


		cout<<"Your choice is: ";
		keyPressed = _getch();
		cout<<keyPressed << endl;

		switch (keyPressed) {
		case 'Q': case 'q'://Quit
			cout << "[QUIT]:" << endl;
			inMenu = false;
			break;

		case 'K': case 'k':// 
			displayParametersKbModel();
			cout << "[Keyboard model demo]:" << endl;
			demoOfKeyboardModel();
			endOfService("[Keyboard model demo]");
			break;

		case 'S': case 's':// 
			displayParametersSpellingModel();
			cout << "[Spelling model demo, PART I]:" << endl;
			demoTransitionProbability();
			endOfService("[Spelling model demo: state-transition probabilities]");
			cout << "[Spelling model demo, PART II]:" << endl;
			demoInitialProbaility();
			endOfService("[Spelling model demo: initial-state probabilities]");
			break;

		case 'A': case 'a':// 
		{
			displayParametersKbModel();
			displayParametersSpellingModel();

			char charToType;
			cout << endl << "Now give me a character to type for 10 times:";
			cin >> charToType;

			for (int i = 0; i < 10; i++)
			{
				cout << endl << "[" << i + 1 << "]: get \"" << typeOneChar(charToType)
					<< "\" while trying to type \"" << charToType << "\"" << endl << endl << endl;
			}

			char word[21];
			char output[101];

			cout << endl << "Now give me a word (upto 20 characters) to type (10 times):";
			cin >> word;

			for (int i = 0; i < 10; i++)
			{
				typeOneWord(word, output, true, 100);
				cout << endl << "[" << i + 1 << "]: get \"" << output
					<< "\" while trying to type \"" << word << "\"" << endl << endl << endl;
			}

		}
		endOfService("[Simulation of typing behavior.]");
		break;

		case 'B': case 'b'://
			cout << endl << "[Now typing the whole Biola vision article into]:" << endl;
			cout << "[_1_CorruptedBiolaVision1.txt]" << endl;
			typeOneArticle("1_CorruptedBiolaVision.txt", "BiolaVision.txt");

			cout << endl << "[Now typing the whole Biola vision article into]:" << endl;
			cout << "[_2_CorruptedBiolaVision1.txt]" << endl;
			typeOneArticle("2_CorruptedBiolaVision.txt", "BiolaVision.txt", true);

			endOfService("[Test typing the whole Biola vision article twice]");
			break;

		case 'R': case 'r'://
		{
			cout << "[Recover original message X from 1 corrupted version]:" << endl << endl;
			cout << "***************************************************************" << endl;
			cout << "Trying to recover the corrupted message, make sure" << endl;
			cout << "the file containing the corrupted message and" << endl;
			cout << "the file containing the vocabulary do exist." << endl;
			cout << "***************************************************************" << endl;
			cout << "Be default, we'll look for" << endl;
			cout << "corruptedMessage1.text as the file containing the corrupted message and" << endl;
			cout << "vocabulary.txt as the file contianing the vocabulary words." << endl;
			cout << "****************************************************************" << endl;
			cout << "Each word in corruptedMessage1.txt" << endl;
			cout << "is examined to determine the top 4" << endl;
			cout << "most likely matches in vocabulary.txt" << endl;
			cout << "with the recovered message sotred in recoveryMessage_V1.txt" << endl;
			cout << "*****************************************************************" << endl << endl;
			//cout << "Press space to continue" << endl << endl;
			//_getch();
			cout << "Do you want to change the default names of the files (y or n)";
			char inputChoice;
			string corruptFile;
			string vocabFile;
			string recoveredFile;
			cin >> inputChoice;
			if (inputChoice == 'y' || inputChoice == 'Y')
			{
				cout << "Corrupted message in:" << endl;
				cin >> corruptFile;
				cout << "Vocabulary in:" << endl;
				cin >> vocabFile;
				cout << "Store the recovered message in:" << endl;
				cin >> recoveredFile;
				cout << "Thinking..." << endl << endl;
				tryToRecoverMessageFromCorruptedMessage(corruptFile, vocabFile, recoveredFile);
			}
			else
			{
				cout << "Using default filenames" << endl;
				cout << "Thinking..." << endl << endl;
				tryToRecoverMessageFromCorruptedMessage("corruptedMessage1.txt", "vocabulary.txt", "recoveredMessage_V1.txt");
			}

			cout << "Finished! See the results in the recoveredMessage_V1.txt" << endl << endl;

			endOfService("[Recover original message X from 1 corrupted version]:");
		}
		break;

		case 'T': case 't'://
		{
			cout << "[Recover original message from 2 corrupted versions]:" << endl << endl;

			cout << "*****************************************************" << endl;
			cout << "Trying to recover the corrupted message, make sure" << endl;
			cout << "the file containing the 1st corrupted version and" << endl;
			cout << "the file containing the 2nd corrupted version and" << endl;
			cout << "the file containing the vocabulary do exist." << endl;
			cout << "******************************************************" << endl;
			cout << "By default, we will look for" << endl;
			cout << "corruptedMessag1.txt as the file containing the 1st corrupted version" << endl;
			cout << "corruptedMessage2.txt as the file containing the 2nd corrupted version" << endl;
			cout << "vocabulary.txt as the file containing the vocabulary words." << endl;
			cout << "************************************************************" << endl;
			cout << "By default, each word in corruptedMessage1.txt and each word in" << endl;
			cout << "corruptedMesasage2.txt are paired together to determine the top 4" << endl;
			cout << "most likely matches in vocabulary.txt with the recovered message" << endl;
			cout << "stored in the recoveredMessage_V2.txt" << endl;
			cout << "***********************************************************" << endl << endl;
			cout << "Do you want to change the default names of the files (y or n)";
			char inputChoice;
			string corruptFile1;
			string corruptFile2;
			string vocabFile;
			string recoveredFile;
			cin >> inputChoice;
			if (inputChoice == 'y' || inputChoice == 'Y')
			{
				cout << "Corrupted message 1 in:" << endl;
				cin >> corruptFile1;
				cout << "Corrupted message 2 in:" << endl;
				cin >> corruptFile2;
				cout << "Vocabulary in:" << endl;
				cin >> vocabFile;
				cout << "Store the recovered message in:" << endl;
				cin >> recoveredFile;
				cout << "Thinking..." << endl << endl;
				tryToRecoverMessageFrom2CorruptedMessage(corruptFile1, corruptFile2, vocabFile, recoveredFile);
			}
			else
			{
				cout << "Using default filenames" << endl;
				cout << "Thinking..." << endl << endl;
				tryToRecoverMessageFrom2CorruptedMessage("corruptedMessage1.txt", "corruptedMessage2.txt", "vocabulary.txt", "recoveredMessage_V2.txt");
			}

			cout << "Finished! See the results in the recoveredMessage_V2.txt" << endl << endl;

			endOfService("[Recover original message X from 2 corrupted versions]:");
		}
			break;

		case 'P': case 'p'://
			displayParametersSpellingModel();
			displayParametersKbModel();

			cout << "[Probaility(observedString, actualWord)]:" << endl;
			{	string observedString, actualWord;


			cout << "What is the observed string you see? (Enter . to quit)";
			cin >> observedString;
			while (observedString != ".")
			{
				cout << "What is the actual string you intent to type?";
				cin >> actualWord;
				cout << "Probaility(" << observedString << " | " << actualWord << ")="
					<< prOf1CharSeriesWhenTyping1Word(observedString, actualWord)
					<< endl << endl;

				cout << "What is the observed string you see? (Enter . to quit)";
				cin >> observedString;
			}
			}
			endOfService("[Probaility(observedString, actualWord)]");
			break;

		case 'D': case 'd'://
			{
			displayParametersSpellingModel();
			displayParametersKbModel();
			string observedDocument, actualDocument;
			string selectLog = "e";
			string selectLog10 = "10";
			double sumLog, sumLog10 = 0;
			cout << "[log Probability(observedDocument, actualDocument)]:" << endl;
			cout << "What is the actual document to type?" << endl;
			cin >> actualDocument;
			cout << "What is the observed corrupted document? (Enter . to quit)" << endl;
			cin >> observedDocument;
			while (observedDocument != ".")
			{
				sumLog = logPrOfGettingDocument1WhenTypingDocument2(observedDocument, actualDocument, selectLog, true);
				sumLog10 = logPrOfGettingDocument1WhenTypingDocument2(observedDocument, actualDocument, selectLog10, true);
				cout << "log Probability(" << observedDocument << " | " << actualDocument << ") is:" << endl;
				cout << sumLog << " using natural logarithm base e, or equivalently" << endl;
				cout << sumLog10 << " using logarithm base 10" << endl;

				cout << "What is the observed corrupted document? (Enter . to quit)" << endl;
				cin >> observedDocument;
			}
			endOfService("[log Probability(observedDocument, actualDocument)]:");
			break;
			}
		case 'L': case 'l':
		{
			displayParametersSpellingModel();
			displayParametersKbModel();
			string observedDocument, actualDocument;
			cout << "[learn parameters based on brute force probability]:" << endl;
			cout << "What is the actual document to type?" << endl;
			cin >> actualDocument;
			cout << "What is the observed corrupted document? (Enter . to quit)" << endl;
			cin >> observedDocument;
			while (observedDocument != ".")
			{
				learnParameters(observedDocument, actualDocument);
				cout << "What is the observed corrupted document? (Enter . to quit)" << endl;
				cin >> observedDocument;
			}

			endOfService("[learn parameters]:");
			break;
		}
		case 'X': case 'x'://
			setParametersSpellingModel();
			endOfService("Reset parameters of the spelling model");
			break;

		case 'Y': case 'y'://
			setParametersKbModel();
			endOfService("Reset parameters of the keyboard model");
			break;

		case 'Z': case 'z'://
			displayParametersKbModel();
			displayParametersSpellingModel();
			endOfService("Display of parameter values");
			break;

		case 'U': case 'u'://
		{
			char inputChoice;
			string reportFile;
			cout << "[Check the quality of the recovered message(s)]:" << endl;
			cout << "Please enter the file name for storing the quality report." << endl;
			cin >> reportFile;
			cout << endl;
			cout << "Are the corrupted versions already recovered through options L, R, T? (y or n)" << endl << endl;
			cin >> inputChoice;
			if (inputChoice != 'y')
			{
				cout << "Please run options L, R, and T first." << endl;
				cout << "Press space to continue" << endl;
				_getch();
				break;
			}

			// assign file names
			string recoveredFile1 = "recoveredMessage_V1.txt"; // for reading in lines from vocabulary
			string recoveredFile2 = "recoveredMessage_V2.txt";
			string masterFile = "messageX.txt";
			string qualityReportFile = reportFile;

			cout << "Check the quality of the recovered message vs the original message" << endl << endl;

			cout << "Be default, we assume 2 versions of the recovered message are in" << endl;
			cout << "recoveredMessages_V1.txt" << endl;
			cout << "recoveredMessage_V2.txt" << endl << endl;

			cout << "By default, we assume the original message is in messageX.txt" << endl << endl;

			cout << "Press space to continue" << endl;
			_getch();

			cout << "Creating quality report" << endl << endl;
			createQualityReport(recoveredFile1, recoveredFile2, masterFile, qualityReportFile);

			cout << "Press space to continue" << endl;
			_getch();
		}
		break;

		default:
			cout << "[?? UNKNOWN COMMAND]:" << endl;
			// Unrecognized key, do nothing
			break;
		}

	} // End of while loop.	typeOneArticle("corruptedBiolaVision1.txt", "BiolaVision.txt");



}

