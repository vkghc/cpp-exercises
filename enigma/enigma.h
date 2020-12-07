#include<fstream>

/* ~~~ GOBAL CONSTANTS ~~~ */
// these help with indexing/search
int const ALPHABET_LENGTH = 26; // most array sizes are a multiple of this
int const ASCII_A = 65;
int const ASCII_Z = 90;
int const MIN_INPUTS = 3;
int const ONE_ROTOR_NO_POS = 4;
int const NON_ROTOR_ARGS = 4; 
int const INDEX_A = 0;
int const INDEX_Z = 25; 

/* ~~~ HELPER FUNCTIONS ~~~ */

// these are used to make various checks while info from config files is being read
// if an error is encountered they throw an error code and output an error message (cerr)

// Check if stream 'in' sucessfully connected to file in file_path
// throw ERROR_OPENING_CONFIG_FILE if not
void checkOpen(std::ifstream const& in, char const* file_path);

// Check if fail flat set on 'in' after attempting to extract an integer
// if fail flag set but eof flag not set, non-numeric char encountered
// throw NON_NUMERIC_CHARACTER error code
void checkNonNumeric(std::ifstream const& in, char const* file_path);

// Check if integer just extracted is b/w 0 and 25
// throw INVALID_INDEX if not
void checkValidIndex(int integer, char const* file_path);

// Given integer in position 'pos' in  'input_array' (where I store extracted integers),
// function checks if integer repeated so far, or if mapped to self for rotor/plugboard
// throw error code, which depends on which class is being checked (check_class)
void checkValidMap(int const* input_array, int pos, char const* file_path, char check_class);

// Check if, so far, more than 26 params specified
// for plugboard; throw INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS
void checkParamErrorPb1(int length, char const* file_path);

// Check if odd number of params specified for plugboard
// throw INCORRECT_NUMBER_OF_PARAMETERS
void checkParamErrorPb2(int length, char const* file_path);

// Check if too many params specified for reflector
// throw INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS
void checkParamErrorRf1(int length, char const* file_path);

// Check if too few params specified for reflector
// throw INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS
void checkParamErrorRf2(int length, char const* file_path);

// Check if all inputs mapped in rotor
// throw INVALID_ROTOR_MAPPING
void checkParamErrorRotor(int length, char const* file_path);


// Check if more starting positions in pos config file than rotors
// Output warning but allow program to run
void checkParamWarningPos(int length, int num_rotors, char const* file_path);


// Check if too few positions specified given number of rotors
// throw NO_ROTOR_STARTING_POSITION
void checkParamErrorPos(int length, int num_rotors, char const* file_path);

/* ~~~ END HELPER FUNCTIONS ~~~ */



/* ~~~ CREATE CLASSES TO BUILD ENIGMA ~~~*/

class Plugboard
{
 private:
  // contains mapping of letters
  char plugboard_letters_[ALPHABET_LENGTH];
 
  char output_letter_;

  // sets up mapping given a valid config file
  // uses swap
  void setPlugboard(int const* input_array);

  // swaps positions of letters to implement mapping
  friend void swap(int& a, int& b);
  
 public:
  // constructor extracts mapping from config file and checks if valid
  // throws error if not
  Plugboard(char const* file_path);
  
  char getPlugboardOutput(char input_letter);
};


class Reflector
{
 private:

  // contains mapping
  char reflector_letters_[ALPHABET_LENGTH];
  
  char output_letter_;
  
  // sets up mapping given a valid config file
  // uses swap
  void setReflector(int const* input_array);
  
  friend void swap(int& a, int& b);
  
public:
  // constructor extracts mapping from config file and checks if valid
  // throws error if not
  Reflector(char const* file_path);
  
  char getReflectorOutput(char input_letter);
};


class Rotor
{
  // enigma has access to private members but only throw private functions
  friend class Enigma;
  
 private:
  // normal alphabet
  char static_letters_[ALPHABET_LENGTH];
  
  // accounts for rotation
  char shifted_letters_[ALPHABET_LENGTH];

  // mappings are directional
  char forward_map_[ALPHABET_LENGTH];
  char backward_map_[ALPHABET_LENGTH];
  
  char window_;

  // at most 26 notches make sense
  char notches_[ALPHABET_LENGTH];

  // output from mapping is directional
  char forward_output_;
  char backward_output_;
  
  // finds given character in array and returns position
  // returns 'sentinel' integer 26 otherwise
  int matchChar(char letter, char const target_array[ALPHABET_LENGTH]);

  // sets rotors mappings, and any provided notches given valid config file
  void setRotorMapsNotches(int const* input_array);

  // checks if rotor positions valid and enough and sets for all rotors
  // throws error otherwise
  friend void setRotorPositions(char const* file_path, int num_rotors, Rotor** rotor_array_ptr); 
  
 public:
  // constructor extracts mapping from config file and checks if valid
  // throws error if not
  Rotor(char const* file_path);
  
  char getWindow();
  void rotateRotor();
  char getForwardOutput(char letter);
  char getBackwardOutput(char letter); 
};


class Enigma
{
 
 private:
  // set up pointer to objects that build enigma
  // objects will be dynamically created
  Plugboard* pb_ptr_;
  Reflector* rf_ptr_;
  Rotor** rotor_array_ptr_;
  int num_rotors_;

  // if only 5 command line arguments, single rotor but no position file
  // throw error
  void checkSingleRotor(int num_config_files); 
  
  // this function  has access to private data member notches_ in Rotors class
  // starting from the rotor before the right most rotor
  // it rotates all rotors if the one to their immediate right
  // has a notch letter displayed in its window
  // made private because it accesses private data memebers of other classes
  void rotateIfRightAtNotch();
  
  // gets the output of all rotors, given initial intput
  char getRotorsOutput(char letter, char direction); // direction = {'f', 'b'}

    
 public:
  Enigma(int num_config_files, char** config_files);

  // destroy all dynamic objects created within enigma
  ~Enigma();

  int checkInput(char letter);
  
  char encode(char letter);
};
