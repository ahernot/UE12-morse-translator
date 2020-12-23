
#include <fstream>

#include <string>
#include <map>




/**
 * Fill a string->string conversion map with data from a TXT file
 *
 * @param conversion_map Conversion map to fill
 * @param fill_filepath Path of file containing map information
 */
void fillConversionMap (std::map<std::string, std::string>& conversion_map,
                        const std::string& fill_filepath) {

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
