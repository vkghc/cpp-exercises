#include"enigma.h"// also contains global constants
#include"errors.h"
#include<iostream>
#include<fstream>
#include<cmath>

using namespace std;

/* ~~~ HELPER FUNCTIONS ~~~ */
// these are used to make various checks while info from config files is being read
// if an error is encountered they throw an error code and output an error message (cerr)

// Check if stream 'in' sucessfully connected to file in file_path
// throw ERROR_OPENING_CONFIG_FILE if not
void checkOpen(ifstream const& in, char const* file_path)
{
  if(!in)
    {
      cerr << "File " << file_path << " does not exist or cannot be accessed.\n";
      throw ERROR_OPENING_CONFIGURATION_FILE;
    }
}

// Check if fail flat set on 'in' after attempting to extract an integer
// if fail flag set but eof flag not set, non-numeric char encountered
// throw NON_NUMERIC_CHARACTER error code
void checkNonNumeric(std::ifstream const& in, char const* file_path)
{
  if(in.fail())
    {
      cerr << "Non-numeric character in config file " << file_path << ".\n";
      throw NON_NUMERIC_CHARACTER;
    }
}

// Check if integer just extracted is b/w 0 and 25
// throw INVALID_INDEX if not
void checkValidIndex(int integer, char const* file_path)
{
  if(integer > INDEX_Z || integer < INDEX_A)
    {
      cerr << "Integer not between 0 and 25 found in  file " << file_path <<".\n";
      throw INVALID_INDEX;
    }
  
}

// given integer in position 'pos' in  'input_array' (where I store extracted integers),
// function checks if integer repeated so far, or if mapped to self for rotor/plugboard
// throw error code, which depends on which class is being checked (check_class)
void checkValidMap(int const* input_array, int pos, char const* file_path,  char check_class)
{
  switch(check_class)
    {
    case 'p':
      
      // check if any contact connected to itself
      if(pos%2 == 0 && input_array[pos] == input_array[pos-1])
	{
	  cerr << "Plugboard contact '" << input_array[pos-1] << "' cannot be mapped to itself in " << file_path << ".\n"; 
	  throw IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
	}
      
      // check if any contact repeatd
      for(int j = 0; j <= pos; j++)
	for(int k = 0; k <=pos; k++)
	  if(j != k && input_array[j] == input_array[k])
	    {
	      cerr << "Plugboard contact '" << input_array[j] << "' mapped more than once in " << file_path << ".\n"; 
	      throw IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
	    }
      break;
      
  
    case 'f':
    
      if(pos%2 == 0 && input_array[pos] == input_array[pos-1])
	{
	  cerr << "Reflector contact '" << input_array[pos-1] << "' cannot be mapped to itself in " << file_path << ".\n"; 
	  throw INVALID_REFLECTOR_MAPPING;
	}
      
      // check if any contact repeatd
      for(int j = 0; j <= pos; j++)
	for(int k = 0; k <=pos; k++)
	  if(j != k && input_array[j] == input_array[k])
	    {
	      cerr << "Reflector contact '" << input_array[j] << "' mapped more than once in " << file_path << ".\n"; 
	      throw INVALID_REFLECTOR_MAPPING;
	    }
      
      break;

    case 't':
      
      // check if any contact repeatd in first 26 positions
      for(int j = 0; j <= pos; j++)
	for(int k = 0; k <=pos; k++)
	  if(pos<ALPHABET_LENGTH && j != k && input_array[j] == input_array[k])
	    {
	      if(j > k)
		{
		  cerr << "Rotor input '" << j << "' cannot be mapped to output '" << input_array[j] << "' (output already mapped to/from '" << k << "') in " << file_path <<".\n";
		}
	      else
		{
		  {
		    cerr << "Rotor input '" << k << "' cannot be mapped to output '" << input_array[j] << "' (output already mapped to/from '" << j << "') in " << file_path <<".\n";
		  }
		}
	      
	      throw INVALID_ROTOR_MAPPING;
	      
	    }
      break;
    }
}



// Check if, so far in extraction from config file, more than 26 params specified
// for plugboard; throw INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS
void checkParamErrorPb1(int length, char const* file_path)
{
  if(length >= ALPHABET_LENGTH)
    {
      cerr << "Too many parameters in config " << file_path << ".\n"; 
      throw INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
    }
}


// Check if odd number of params specified for plugboard
// throw INCORRECT_NUMBER_OF_PARAMETERS
void checkParamErrorPb2(int length, char const* file_path)
{
    if(length%2 != 0)
    {
      cerr << "Incorrect (odd) number of parameters in config file " << file_path << ".\n"; 
      throw INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
    }
}


// Check if too many params specified for reflector
// throw INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS
void checkParamErrorRf1(int length, char const* file_path)
{
  if(length >= ALPHABET_LENGTH)
    {
      cerr << "Too many parameters in config file " << file_path << ".\n"; 
      throw INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
    }
}

// Check if too few params specified for reflector
// throw INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS
void checkParamErrorRf2(int length, char const* file_path)
{
  if(length < ALPHABET_LENGTH)
    {
      cerr << "Too few reflector parameters in config file " << file_path << ".\n"; 
      throw INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
    }
}

// Check if all inputs mapped in rotor
// throw INVALID_ROTOR_MAPPING
void checkParamErrorRotor(int length, char const* file_path)
{
  if(length < ALPHABET_LENGTH)
    {
      cerr << "Not all inputs mapped in rotor " << file_path << ".\n"; 
      throw INVALID_ROTOR_MAPPING;
    }
}

// Check if more starting positions in pos config file than rotors
// Output warning but allow program to run
void checkParamWarningPos(int length, int num_rotors, char const* file_path)
{
  if(length > num_rotors)
    {
      cerr << "Warning: More starting positions than rotors supplied in " << file_path << "\n";
      cerr << "Only first " << num_rotors << " position(s) used.\n"; 
    }
}

// Check if too few positions specified given number of rotors
// throw NO_ROTOR_STARTING_POSITION
void checkParamErrorPos(int length, int num_rotors,  char const* file_path)
{
  if(length < num_rotors)
    {
      cerr << "No starting position specified for rotor position " << length << " in "  << file_path << ".\n"; 
      throw NO_ROTOR_STARTING_POSITION;
    }
}
/* ~~~ END HELPER FUNCTIONS ~~~ */ 



/* ~~~ CREATE CLASSES TO BUILD ENIGMA ~~~*/

// This is friend function to plugboard and reflector
// It swaps positions of letters to implement encoding
void swap(int& a, int& b)
{
  int temp = a;
  a = b;
  b = temp;
}

// constructor extracts mapping from config file and checks if valid
// throws error if not
Plugboard::Plugboard(char const* file_path)
{
  // start assuming null.pb
  for(int i = 0; i < ALPHABET_LENGTH; i++)
    plugboard_letters_[i] = (char)(ASCII_A + i);
  
  // open input stream
  ifstream in;
  in.open(file_path);
  
  // no file/access fail
  checkOpen(in,file_path);
  
  // start reading in file
  int i = 0;
  int pb_input[52];
  in >> pb_input[i];
  while(!in.eof())
    {
      // before we reach eof, perform checks
      if(!in.eof())
	{
	  // too many params fail
	  checkParamErrorPb1(i, file_path);
	  
	  // non-numeric fail
	  checkNonNumeric(in, file_path); 

	  // integer not 0-25 fail
	  checkValidIndex(pb_input[i], file_path); 
	  
	  // invalid map fail 
	  checkValidMap(pb_input, i, file_path, 'p'); 
	}

      i++; 
      in >> pb_input[i];
    }
  in.close();
  
  pb_input[i] = ALPHABET_LENGTH; // 'sentinel' integer
  
  // odd number of params fail
  checkParamErrorPb2(i, file_path); 
  
  // set plugboard
  setPlugboard(pb_input);  
}


// sets up mapping given a valid config file
// uses swap
void Plugboard::setPlugboard(int const* input_array)
{
  int i = 0;
  while(input_array[i] != ALPHABET_LENGTH)
    {
      int a =  input_array[i];
      int b = input_array[i+1];
      swap(plugboard_letters_[a], plugboard_letters_[b]); 
      i +=2;
    }
}

char Plugboard::getPlugboardOutput(char input_letter)
{
  output_letter_ = plugboard_letters_[(int)(input_letter) - ASCII_A];
  return output_letter_; 
}

// constructor extracts mapping from config file and checks if valid
// throws error if not
Reflector::Reflector(char const* file_path)
{
  // set non-swapped reflector
  for(int i = 0; i < ALPHABET_LENGTH; i++)
    reflector_letters_[i] = (char)(ASCII_A + i);

  // open input stream
  ifstream in;
  in.open(file_path);
  
  // file/access fail
  checkOpen(in,file_path);
  
  // start reading in file
  int i = 0;
  int rf_input[52];
  in >> rf_input[i];
  while(!in.eof())
    {
      // before we reach eof, perform checks
      if(!in.eof())
	{
	  // too many params fail
	  checkParamErrorRf1(i, file_path);
	  
	  // non-numeric char fail
	  checkNonNumeric(in, file_path); 
	  
	  // non 0-25 integer fail
	  checkValidIndex(rf_input[i], file_path); 

	  // invalid map fail 
	  checkValidMap(rf_input, i, file_path, 'f');
	}

      i++; 
      in >> rf_input[i];
      
    }
  in.close();

  rf_input[i] = ALPHABET_LENGTH; // 'sentinel' integer
  
  // too few params fail 
  checkParamErrorRf2(i, file_path);
  
  // set reflector
  setReflector(rf_input);
}

// sets up mapping given a valid config file
// uses swap
void Reflector::setReflector(int const* input_array)
{
  // set reflector
  int i = 0;
  while(input_array[i] != ALPHABET_LENGTH)
    {
      int a =  input_array[i];
      int b = input_array[i+1];
      swap(reflector_letters_[a], reflector_letters_[b]); 
      i +=2;
    }
}


char Reflector::getReflectorOutput(char input_letter)
{
  output_letter_ = reflector_letters_[(int)(input_letter) - ASCII_A];
  return output_letter_; 
}


// constructor extracts mapping from config file and checks if valid
// throws error if not
Rotor::Rotor(char const* file_path)
{
  // start assuming everything maps to itself
  for(int i = 0; i < ALPHABET_LENGTH; i++)
    {
      static_letters_[i] = (char)(ASCII_A + i);
      shifted_letters_[i] = (char)(ASCII_A + i);
      forward_map_[i] = (char)(ASCII_A + i);
      backward_map_[i] = (char)(ASCII_A + i);
      window_ = (char)ASCII_A; 
    }
   
  // open input stream
  ifstream in;
  in.open(file_path);
  
  // file/access fail
  checkOpen(in,file_path);
  
  // start reading in file
  int i = 0;
  int rotor_input[52];
  in >> rotor_input[i];
  while(!in.eof())
    {
     // before we reach eof, perform checks
      if(!in.eof() && rotor_input[i] < 52)
	{
	  // non-numeric fail
	  checkNonNumeric(in, file_path); 
	  
	  // non 0-25 integer fail
	  checkValidIndex(rotor_input[i], file_path); 
	  
	  // invalid mapping fail
	  checkValidMap(rotor_input, i, file_path, 't');
	}
      
      i++; 
      in >> rotor_input[i];
    }
  in.close();
  
  // too few params fail
  checkParamErrorRotor(i, file_path); 
  
  rotor_input[i] = ALPHABET_LENGTH; // 'sentinel' integer

  // set rotor
  setRotorMapsNotches(rotor_input); 
}


// sets rotors mappings, and any provided notches given valid config file
void Rotor::setRotorMapsNotches(int const* input_array)
{
  
  // set rotor mappings
  for(int i = 0; i < ALPHABET_LENGTH; i++)
    forward_map_[i] = (char)(input_array[i]+ASCII_A);
  
  for(int i = 0; i < ALPHABET_LENGTH; i++)
    {
      int temp = matchChar((char)(i+ASCII_A), forward_map_);
      backward_map_[i] = (char)(temp + ASCII_A);
    }

  
  // set notches
  int i = ALPHABET_LENGTH; // starts reading after rotor mapping specified
  while(input_array[i] != ALPHABET_LENGTH)
    {
      notches_[i-ALPHABET_LENGTH] = (char)(input_array[i] + ASCII_A);
      i++; 
    }
  notches_[i] = '\0';
}


// checks if rotor positions valid and enough and sets for all rotors
// throws error otherwise
void setRotorPositions(char const* file_path, int num_rotors, Rotor** rotor_array_ptr)
{
  // open input stream
  ifstream in;
  in.open(file_path);
  
  // file/access fail
  checkOpen(in,file_path);
  
  // start reading in file
  int i = 0;
  int position_input[num_rotors];
  in >> position_input[i];
  while(!in.eof())
    {
      // before we reach eof, perform checks
      if(!in.eof())
	{ 
	  // non-numeric fail 
	  checkNonNumeric(in, file_path);
	  
	  // non 0-25 fail 
	  checkValidIndex(position_input[i], file_path);
	}
      
      i++;
      in >> position_input[i];
    }
  in.close();

  // not enough start positions fail 
  checkParamErrorPos(i, num_rotors, file_path);
  
  // too many start positions warning
  checkParamWarningPos(i, num_rotors, file_path); 
  
  // set initial position for rotors
  for(int i = 0; i < num_rotors; i++)
    {
      // set window
      rotor_array_ptr[i]->window_ = (char)(position_input[i]+ASCII_A);
      
      // set the initial position of the shifted_letters_
      int temp = rotor_array_ptr[i]->window_ - ASCII_A; 
      for(int j = 0; j <ALPHABET_LENGTH; j++)
	rotor_array_ptr[i]->shifted_letters_[j] =
	  rotor_array_ptr[i]->static_letters_[(j+temp) % ALPHABET_LENGTH];
    }
}

// finds given character in array and returns position
// returns 'sentinel' integer ALPHABET_LENGTH otherwise
int Rotor:: matchChar(char letter, char const *target_array)
{
  for(int i = 0; i < ALPHABET_LENGTH; i++)
    if(letter == target_array[i])
      return i;
  return ALPHABET_LENGTH; 
}

char Rotor::getWindow()
{
  return window_; 
}

char Rotor::getForwardOutput(char letter)
{
  forward_output_ = shifted_letters_[(int)(letter - ASCII_A)];

  forward_output_ = forward_map_[matchChar(forward_output_, static_letters_)];

  forward_output_ = static_letters_[matchChar(forward_output_, shifted_letters_)]; 

  return forward_output_; 
  
}

char Rotor::getBackwardOutput(char letter)
{
  backward_output_ = shifted_letters_[(int)(letter - ASCII_A)];

  backward_output_ = backward_map_[matchChar(backward_output_, static_letters_)];
  
  backward_output_ = static_letters_[matchChar(backward_output_, shifted_letters_)];
  
  return backward_output_; 
  
}

void Rotor::rotateRotor()
{
  char temp[ALPHABET_LENGTH]; 
  for(int i = 0; i < ALPHABET_LENGTH; i++)
    temp[i] = shifted_letters_[(i + 1) % ALPHABET_LENGTH];

  for(int i = 0; i < ALPHABET_LENGTH; i++)
    shifted_letters_[i] = temp[i]; 
  
  window_ = (abs((int)(window_ - ASCII_A + 1) % ALPHABET_LENGTH)) + ASCII_A;    
}


Enigma::Enigma(int num_config_files,  char** config_files)
{
  num_rotors_ = num_config_files-NON_ROTOR_ARGS;

  // check if single rotor w/o rotor position file (special case for 5 cmd line argument)
  try
    {
      checkSingleRotor(num_config_files); 
    }
  catch(int error_code)
    {
      if(error_code != NO_ERROR)
	throw error_code; 
    }

  // check if plugboard file well formed; yes, create plugboard
  try
    {
      pb_ptr_ = new Plugboard(config_files[1]); // delete
    }
  catch(int error_code)
    {
      if(error_code != NO_ERROR)
	throw error_code; 
    }
  
  // check and set reflector
  try
    {
      rf_ptr_ = new Reflector(config_files[2]); // delete
    }

  catch(int error_code)
    {
      if(error_code != NO_ERROR)
	throw error_code;
    }
  
  // if rotors exist, create them
  if(num_rotors_ > 0)
    {
      rotor_array_ptr_ = new Rotor* [num_rotors_];
      if(num_config_files) // leave last file for rotor position
	{
	  for(int i = 0; i < num_rotors_; i++)
	    {
	      try
		{
		  rotor_array_ptr_[i] = new Rotor(config_files[i+3]); // delete
		}
	      
	      catch(int error_code)
		{
		  if(error_code != NO_ERROR)
		    throw error_code;
		}
	    }
	}
      
      // check and load rotor initial specification file
      try
	{
	  setRotorPositions(config_files[num_config_files-1], num_rotors_, rotor_array_ptr_);
	}
      catch(int error_code)
	{
	  if(error_code != NO_ERROR)
	    throw error_code;
	}
    }
}


// if only 5 command line arguments, single rotor but no position file
// throw error
void Enigma::checkSingleRotor(int num_config_files)
{
  if(num_rotors_ == 0 && num_config_files == ONE_ROTOR_NO_POS)
    {
      cerr << "One rotor file supplied, but no rotor position file.\n";
      throw(NO_ROTOR_STARTING_POSITION);
    }
}

// this function  has access to private data member notches_ in Rotors class
// starting from the rotor before the right most rotor
// it rotates all rotors if the one to their immediate right
// has a notch letter displayed in its window
// made private because it accesses private data memebers of other classes
void Enigma::rotateIfRightAtNotch()
{
  // only does something if more than one rotor
  if(num_rotors_ > 1) 
    {
      // get the position of the right-most rotor
      int rotor_index = num_rotors_ -1;
      
      for(int i = rotor_index; i >= 0; i--)
	{
	  // get the window of the right rotor
	  char right_window = rotor_array_ptr_[i]->getWindow();
	  
	  int j = 0;
	  while(rotor_array_ptr_[i]->notches_[j] != '\0')
	    {
	      if(rotor_array_ptr_[i]->notches_[j] == right_window)
		rotor_array_ptr_[i-1]->rotateRotor();
	      j++;
	    }
	}
    }
}

// gets the output of all rotors, given initial intput
// specify direction ('f', 'b') as rotor mapping is directional
char Enigma::getRotorsOutput(char letter, char direction)
{
  char output = letter;
  
  if(direction == 'f')
    for(int i = num_rotors_-1; i >= 0; i--)
      output = rotor_array_ptr_[i]->getForwardOutput(output);
  
  else
    for(int i = 0; i <= num_rotors_-1; i++)
      output = rotor_array_ptr_[i]->getBackwardOutput(output);
  
  return output; 
}


int Enigma::checkInput(char letter)
{
  if(letter < ASCII_A || letter > ASCII_Z)
    return INVALID_INPUT_CHARACTER;

  return NO_ERROR; 
}

char Enigma::encode(char letter)
{
  char encoded =  pb_ptr_->getPlugboardOutput(letter);
  

  if(num_rotors_ > 0)
    {
      //always rotate right most rotor
      rotor_array_ptr_[num_rotors_ -1]->rotateRotor();

      // rotate all other rotors as needed
      rotateIfRightAtNotch();

      // get rotor output
      encoded = getRotorsOutput(encoded, 'f'); 

      // reflect
      encoded = rf_ptr_->getReflectorOutput(encoded);
      
      // back through rotors
      encoded = getRotorsOutput(encoded, 'b');
    }
  else
    {
      // reflect
      encoded = rf_ptr_->getReflectorOutput(encoded);
    }

  // one last time through the plugboard
  encoded =  pb_ptr_->getPlugboardOutput(encoded);
  
  return encoded; 
  
}

// destroy all dynamic objects created within enigma
Enigma::~Enigma()
{
  delete pb_ptr_;
  pb_ptr_ = nullptr;

  delete rf_ptr_;
  rf_ptr_ = nullptr;

  for(int i = 0; i < num_rotors_; i++)
    {
      delete rotor_array_ptr_[i];
      rotor_array_ptr_[i] = nullptr; 
    }

  delete [] rotor_array_ptr_;
  rotor_array_ptr_ = nullptr;
}
