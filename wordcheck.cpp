#include <iostream>
#include <string>
#include <fstream>
#include <unordered_set>
#include <time.h> 
using namespace std;

unordered_set<string> dictionary; //Creates an unordered set that contains strings

struct play{ //A simple struct used to contain values and words
    int value;
    string word;
};

//Function declarations
play doStuff(string input, int factor);

int getValue(char a);


// PrintFunction can be used to print entire dictionary
void printSet(unordered_set<string> const &dict)
{
    for (auto it = dict.cbegin(); it != dict.cend(); it++) { //iterator iterates until it reaches end of set. 
        cout << *it << ' ' << endl; //prints whatever iterator is pointing at
    }
}

//Main function: Loads dictionary, selects mode, receives user inputs, checks word validity and score, and outputs
int main (){
    string inputWord, dummy, finalWord; //InputWord: userInput, dummy: used in loading of dictionary, finalWord: processed word
    play wordVal; //play is a struct with a word and a value
    int factor, userInput; //Factor: multiplication factor of word score, userInput: used in startup meno
    bool scrabbleMode, keepGoing = true; //scrabbleMode: if scrabble mode is used or not, keepGoing: toggled off when user wishes to exit program
    clock_t t; //Clock variable used to measure performance
    ifstream dictionaryFile("dictionary.txt"); //Input stream read from attached file dictionary.txt (Credit to Collins 2019;)

    cout << "Loading Dictionary" << endl;
    t = clock(); //start measuring time here

    while(!dictionaryFile.eof()){ //inputting from file stream into dummy string and then dummy string into dictionary set
        dictionaryFile >> dummy; //Loops until eof reached
        dictionary.insert(dummy);
    }


    cout << "Done Loading Dictionary: Collins Scrabble Words 2019" << endl;
    cout << "txt file available at https://boardgames.stackexchange.com/questions/38366/latest-collins-scrabble-words-list-in-text-file" << endl;
    t = clock() - t; //Done measuring
    float clks_per_ms = CLOCKS_PER_SEC/1000; //calculating clocks per millisecond (1s = 1000s)
    cout << "Dictionary loaded in " << t/clks_per_ms << " ms" << endl;

    while(userInput != 1 && userInput != 2){ //Looping until user presses correct values
    cout << "Press 1 to check spelling, 2 to get Scrabble values, 3 to Print Dictionary" << endl;
    cin >> userInput; //getting input
    if(userInput == 2)  scrabbleMode = true; //toggling scrabbleMode
    else if(userInput == 1) scrabbleMode = false;
    else if(userInput == 3){ //Printing dictionary
        printSet(dictionary);
        cout << "Note, this is a scrabble dictionary (Collins 2019) so some of the words are a bit ... weird" << endl;
    }
    else cout << "invalid value, try again" << endl; //If invalid value, outputting error message before looping
    }


    while(keepGoing){ //As long as user wishes to continue
        cout << "Enter word below:" << endl;
        if(scrabbleMode){  //Details for scrabble score calculation (double/triple letter word score squares)
            cout << "Capitalize letters that are on double letter, i.e exAmplE" << endl << 
            "Double write capitalized letters that are tripled. i.e eXXample" << endl; 
        }
        cout << "input 0 to exit program" << endl; 
        cin >> inputWord;
        if(inputWord == "0"){ //Program exit toggle
            keepGoing = false;
            break;
        }
        if(scrabbleMode){ //Checking for multiplier (double/triple word score boxes) in scrabble mode
            cout << "Enter multiplier here: " << endl;
            cin >> factor;
        }
        wordVal = doStuff(inputWord, factor); //Doing function that calculates word score and deals with capitalization
        finalWord = wordVal.word; //getting final word from resulting struct 

        for(int i = 0; i < 25; i++) cout << endl; //clearing screen


        cout << "your word is " << finalWord << endl; //Confirming final word to users
        t = clock(); //starting clock
        if(dictionary.count(finalWord)){ //for an unordered set, count returns amount of times key appears. Therefore only true if word exists
            t = clock()-t; //stopping clock
            cout << "Word " << finalWord << " is VALID" << endl;
            if(scrabbleMode) cout << "value is " << wordVal.value << endl;
            cout << "Found word in " << t/clks_per_ms << " ms" << endl;
        }
        else{
            t = clock()-t; //stopping clock
            cout << "Word " << finalWord << " is invalid" << endl;
            cout << "Did not find word in " << t/clks_per_ms << " ms" << endl;
        }
    }
    cout << "Ended program. Thanks!" << endl;
    return 0;
}


//Function doStuff: calculates word score and also gets rid of any capitals (dictionary has no capital letters). returns struct containing value and string
play doStuff(string input, int factor){
    play output; //output struct
    string out; //output string
    char dummy; //dummy char value
    int value = 0; //value counter for scrabble score
    int k=1; //dummy variable
    if(isupper(input[0])){ //isupper returns nonzero if character is capital letter
        out = tolower(input[0]); //initializing string with first output character
        dummy = tolower(input[0]); //tolower returns char that is uncapitalized version on input
        if(input[0] == input[1]){ //If user inputs double capital letters, its triple letter score 
                                //(cant be double letter * 2 cause no adjacent double letter score tiles)
            value += 3*getValue(dummy); //adding to total value
            k++;
        }
        else{
            value += 2*getValue(dummy); //if single capital, is double letter val
        }
    }
    else { //For non capital letter
        out = input[0]; //initializing string
        value += getValue(input[0]); //adding to value
    }
    for(int i=k; i< input.length(); i++){ //looping through all string chars
        if(isupper(input[i])){ //capital letter
            dummy = tolower(input[i]); //creating dummy uncapitalized char
            out = out + dummy; //concatenating to output string
            if (input[i] == input[i+1]){ //if triple score case
                value += 3*getValue(dummy); //adding value
                i++; //skipping next letter
            }
            else{ //double score case
                value += 2*getValue(dummy);
            }
        }
        else{
            out = out + input[i]; //concatenating to string
            value += getValue(input[i]); //adding score
        }
    }
    output.value = factor*value; //getting final value and multiplying by factor(double/triple word score)
    output.word = out;
    return output;
}

//getValue: returns scrabble score of a given char
int getValue(char a){
    int val;
    switch(a) {
        //Uses cascading cases to limit repetition. Assumed error free input so only scores above 1 are calculated
        case 'd':
        case 'g': val = 2; break;
        case 'b' : case 'c' : case 'm' :
        case 'p' : val = 3; break;
        case 'f' : case 'h' : case 'v' : case 'w' :
        case 'y' : val = 4; break;
        case 'k' : val = 5; break;
        case 'j' : case 'x' : val = 8;  break;
        case 'q' : case 'z' : val = 10; break;
        default: val = 1; break;
    }
    return val;
}