/*
g++ -std=c++11 tests.cpp -o t
./t
*/

// in convert_char std::map can't be const for some reason...

#include <iostream>
#include <fstream> // file rw
#include <sstream>

#include <array>
#include <map>
#include <queue> // queue to add letters



/**
 * Fill a string->string conversion map with data from a TXT file
 *
 * @param conversion_map Conversion map to fill
 * @param fill_filepath Path of file containing map information
 */
void fill_conversion_map (std::map<std::string, std::string>& conversion_map, const std::string& fill_filepath) {

    // Open the conversion file
    std::ifstream file_in;
    file_in.open(fill_filepath);

    // Initialise the loop variables
    std::string line;
    std::string letter; std::string sequence;

    // Get the first line
    file_in >> letter >> sequence;
    //std::cout << letter << " = " << sequence << std::endl;

    // Write to the map
    conversion_map[letter] = sequence;
    
    // Loop through the file's lines
    while (std::getline(file_in, line)) {
        
        // Read the line
        file_in >> letter >> sequence;
        //std::cout << letter << " = " << sequence << std::endl;

        // Write to the map
        conversion_map[letter] = sequence;

    };

    // Close the conversion file
    file_in.close();

};



/**
 * Convert string to morse code binary representation
 *
 * @param c String (character) to convert
 * @param conversion_map Conversion map from strings to morse code binary representation in string form
 * @param preconversion_map Preconversion map to convert to accepted strings for morse code conversion
 * @return Morse code binary representation of string input (one character only)
 */
std::string convert_char (std::string& c,
                   std::map<std::string, std::string>& conversion_map,
                   std::map<std::string, std::string>& preconversion_map) {

    // Preconversion
    std::string cp = preconversion_map[c];

    // Replace c with preconverted version if not empty
    if (!( cp.empty() )) {
        c = cp; // Replace c with preconverted c
    };

    // Conversion
    std::string cc = conversion_map[c];

    return cc;
};



/**
 * Convert string to morse code binary representation
 *
 * @param c String (character) to convert
 * @param conversion_map Conversion map from strings to morse code binary representation in string form
 * @return Morse code binary representation of string input (one character only)
 */
std::string convert_char (std::string& c,
                          std::map<std::string, std::string>& conversion_map) {
    // Generate an empty map
    std::map<std::string, std::string> empty_map;
    
    // Run the conversion function
    std::string cc = convert_char(c, conversion_map, empty_map);

    return cc;
};




std::queue<std::string> convert_from_text (std::string& message,
                                           std::map<std::string, std::string>& conversion_map,
                                           std::map<std::string, std::string>& preconversion_map) {

    // Initialise queue
    std::queue<std::string> conversion_queue;

    // Loop through the characters in the message string
    for (std::string::iterator it = message.begin(); it != message.end(); it++) {

        // Convert from char to string
        std::string c; std::string cc;
        c = *it;

        // Convert characters
        if (c == " ") { cc = "_"; }
        else if (c == ".") { cc = "."; }
        else { std::string cc = convert_char(c, conversion_map, preconversion_map); };

        // Push to queue
        conversion_queue.push(cc);
        
    };

    return conversion_queue;

};



// Function to convert queue to binary signal with 0 and 1, each character lasting for TIME_UNIT milliseconds
// Function to save output to a WAV file



int main () {

    // Generate the default preconversion map (accents are treated as several characters so don't really matter..)
    std::map<std::string, std::string> preconversion_map;
    fill_conversion_map (preconversion_map, "resources/preconversion.txt");

    // Generate the conversion map
    std::map<std::string, std::string> conversion_map;
    fill_conversion_map (conversion_map, "resources/conversion.txt");
    
    // Test
    /*
    std::string x = "à";
    std::string xc = convert_char(x, conversion_map, preconversion_map);
    std::cout << xc << std::endl;
    */


    std::string message = "This is a message. Hello.";
    std::queue<std::string> conversion_queue = convert_from_text(message, conversion_map, preconversion_map);
    

    return 0;
};