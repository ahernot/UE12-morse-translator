#include <string>
#include <map>
#include <queue>


/**
 * Convert string to morse code binary representation
 *
 * @param c String (character) to convert
 * @param conversion_map Conversion map from strings to morse code binary representation in string form
 * @param preconversion_map Preconversion map to convert to accepted strings for morse code conversion
 * @return Morse code binary representation of string input (one character only)
 */
std::string charToMorse (std::string& c,
                         std::map<std::string, std::string>& conversionMap,
                         std::map<std::string, std::string>& preconversionMap) {

    // Preconversion
    const std::string cp = preconversionMap[c]; // APPARENTLY will create a new key with empty value if key not found: passage par référence ou pas, then?

    // Replace c with preconverted version if not empty
    if (!( cp.empty() )) {
        c = cp;
    };

    // Conversion
    const std::string cc = conversionMap[c];

    return cc; // Return by value
};



void stringToMorse (std::string& message,
                    std::queue<std::string>& conversionQueue,
                    std::map<std::string, std::string>& conversionMap,
                    std::map<std::string, std::string>& preconversionMap) {

    // Initialisation (needed?)
    const std::string fullstop = ".";
    const std::string space = "_";

    // Loop through the characters in the message string
    for (std::string::iterator it = message.begin(); it != message.end(); it++) {

        // Convert from char to string
        std::string c; c = *it;

        // Convert characters
        if (c == " ") {
            conversionQueue.push(space); // space (word break)
        }
        else if (c == ".") {
            conversionQueue.push(fullstop); // fullstop (sentence break)
        }
        else {
            std::string cc = charToMorse(c, conversionMap, preconversionMap);
            conversionQueue.push(cc);
        };
        
    };
};
