#include"enigma.h" // also contains global constants
#include"errors.h"
#include<iostream>
#include<fstream>

using namespace std; 

int main(int argc, char** argv)
{
  // program ends if this value ever changes
  int error_code = NO_ERROR;

  // check if enough arguments
  if(argc < MIN_INPUTS)
    {
      cerr << "usage: enigma plugboard-file reflector-file (<rotor-file>* rotor-positions)?\n"; 
      error_code = INSUFFICIENT_NUMBER_OF_PARAMETERS; 
      return error_code;
    }

  Enigma* enigma_ptr; 
  try
    {
      // create enigma from config files
      enigma_ptr = new Enigma(argc, argv); 
    }
  catch(int error_code)
    {
      if(error_code != NO_ERROR)
	return error_code; 
    } 

  // encode stream of characters read from standard input
  char n;
  while(cin >> ws >>  n)
    {
      // check if valid char (A-Z)
      error_code = enigma_ptr->checkInput(n);
      if(error_code != NO_ERROR)
  	{
	  cerr << "'" << n << "' is an invalid input (only capitals A-Z encoded)\n";
  	  return error_code; 
  	}

      // encode
      n = enigma_ptr->encode(n);
      cout << n; 
    }
  
  return NO_ERROR; 
}
