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

// TO DEFINE IN config.h
#define DASH_TIME 3 // length of dash
#define DOT_TIME 1 // length of dot

#define HIGH_REPR 1 // yes signal
#define LOW_REPR 0 // no signal

#define XCHAR_BREAK_TIME 1 // gap between impulses
#define CHAR_BREAK_TIME 3 // gap between characters
#define WORD_BREAK_TIME 7 // gap between words (space)
#define SENTENCE_BREAK_TIME 9 // full-stop





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

    const std::string fullstop = ".";
    const std::string space = "_";

    // Loop through the characters in the message string
    for (std::string::iterator it = message.begin(); it != message.end(); it++) {

        // Convert from char to string
        std::string c; std::string cc;
        c = *it;

        // Convert characters
        if (c == " ") {
            conversion_queue.push(space); // space (word break)
        }
        else if (c == ".") {
            conversion_queue.push(fullstop); // fullstop (sentence break)
        }
        else {
            std::string cc = convert_char(c, conversion_map, preconversion_map);
            conversion_queue.push(cc);
        };
        
    };

    return conversion_queue;

};





/**
 * Convert from morse sequence queue to signal bytestream queue
 *
 * @param conversion_queue Morse sequence queue
 * @return Signal bytestream queue
 */
std::queue<bool> queueToSignal (std::queue<std::string> conversion_queue) {

    // Initialise signal queue
    std::queue<bool> signal_queue;

    // Define conversion constants (to move)
    const std::string dot_repr = "0";
    const std::string dash_repr = "1";
    const std::string fullstop = ".";
    const std::string space = "_";

    // Initialise gap request flag
    bool queue_gap = false;

    // Initialise eof (end of field) flag
    bool eof = false;

    while (!conversion_queue.empty()) {

        // Get first item of conversion queue
        std::string conv_char = conversion_queue.front();

        // Run through element's impulses
        for (std::string::iterator it = conv_char.begin(); it != conv_char.end(); it++) {

            // Convert from char to string
            std::string c; std::string cc;
            c = *it;

            // Dot & Dash
            if ((c == dot_repr) || (c == dash_repr)) {

                // Add gap between characters (if no space character before/after)
                if (queue_gap == true) {
                    //std::cout << ">gap" << std::endl;

                    for (int i=0; i<CHAR_BREAK_TIME; i++) {
                        signal_queue.push(false);
                    };

                    // Update flag
                    queue_gap = false;
                }

                // Add gap between impulses (if no space character before/after)
                else { // if not first character
                    for (int i=0; i<XCHAR_BREAK_TIME; i++) {
                        signal_queue.push(false);
                    };
                };

                // Get impulse length
                int impulse_time;
                if (c == dot_repr) {
                    impulse_time = DOT_TIME;
                    //std::cout << "dot" << std::endl;
                }
                else if (c == dash_repr) {
                    impulse_time = DASH_TIME;
                    //std::cout << "dash" << std::endl;
                };

                // Add impulse
                for (int i=0; i<impulse_time; i++) {
                    signal_queue.push(true);
                };

                // Reset eof (end of field) flag
                eof = false;

            }



            // Space & Fullstop
            else if ( (eof == false) && ((c == space) || (c == fullstop)) ) {

                // Reset gap request flag
                queue_gap = false;

                // Get gap length
                int gap_time;
                if (c == space) {
                    gap_time = WORD_BREAK_TIME;
                    //std::cout << ">>>SPACE" << std::endl;
                }
                else if (c == fullstop) {
                    gap_time = SENTENCE_BREAK_TIME;
                    //std::cout << ">>>FULLSTOP" << std::endl;
                };

                // Add gap
                for (int i=0; i<gap_time; i++) {
                    signal_queue.push(false);
                };

                // Update eof flag
                eof = true;
            };

        };

        // Remove first item from conversion queue
        conversion_queue.pop();

        // Request gap between characters (will apply if queue not empty)
        if (eof == false) {
            queue_gap = true;
        };
        
    };

    
    return signal_queue;

};




int main () {

    // Generate the default preconversion map (accents are treated as several characters so don't really matter..)
    std::map<std::string, std::string> preconversion_map;
    fill_conversion_map (preconversion_map, "resources/preconversion.txt");

    // Generate the conversion map
    std::map<std::string, std::string> conversion_map;
    fill_conversion_map (conversion_map, "resources/conversion.txt");



    // Define message
    std::string message = "This is a message. Hello.";

    // Convert from text to morse sequence
    std::queue<std::string> conversion_queue = convert_from_text(message, conversion_map, preconversion_map);
    
    // Convert from morse sequence to signal bytestream queue
    std::queue<bool> signal_queue = queueToSignal(conversion_queue);



    // Print signal
    while (!signal_queue.empty()) {
        bool cbool = signal_queue.front();
        //std::cout << cbool << std::endl;
        signal_queue.pop();
    };


    
    // Save output to a WAV file

    return 0;
};