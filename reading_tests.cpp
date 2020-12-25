// g++ -std=c++11 reading_tests.cpp -o rt


#include <iostream>
#include <fstream>

#include <math.h>

#include <string>
#include <vector>

//#include "config.h"
#define TIME_UNIT 150
#define SAMPLE_RATE 44100
#define FREQUENCY 440

#define XCHAR_BREAK_TIME 1 // gap between impulses
#define CHAR_BREAK_TIME 3 // gap between characters
#define WORD_BREAK_TIME 7 // gap between words (space)
#define SENTENCE_BREAK_TIME 9 // full-stop

/*
01010101 000 1 000 101110101 000 101110101 000 11101110111 00000000 101110111 000 11101110111 000 1011101 000 101110101 000 1110101
*/

// Clear vector
void clear (std::vector<char> &vec) {
   std::vector<char> empty;
   std::swap(vec, empty);
};

float calcPseudoFrequencySquare (uint8_t dataUnit [],
                                 const int unitSize,
                                 const int sampleRate,
                                 const int low,
                                 const int high) {

    // This calculates the average frequency in a square signal with one high surrounded by lows

    float averageGap = 0;
    int averageId = 1;
    int tempGap = 0;

    for (int i = 0; i < unitSize; i ++) {

        // Read data
        uint8_t data = dataUnit[i];
        int dataInt = (int) data;

        //std::cout << "   data: " << dataInt << std::endl;

        if (dataInt == high) {
            //std::cout << "      is high" << std::endl;
            averageGap = (averageId * averageGap + tempGap) / (averageId + 1); // add to average
            averageId ++; // increment nb of elements averaged
            tempGap = 0; // reset gap
        }
        else {
            //std::cout << "      is low" << std::endl;
            tempGap ++;
        };

        //std::cout << averageGap << std::endl;
    };

    float pseudoFrequency = sampleRate / averageGap;

    if (isinf(pseudoFrequency)) {
        return 0;
    };

    return pseudoFrequency;

};




int main () {

    std::string inFilePath = "output/output.wav";

    // Create input file stream
    std::ifstream inFile;

    // Open file in binary mode
    inFile.open(inFilePath, std::ios::binary);

    // Get size of file in bytes
    inFile.seekg(0, std::ios::end); // place pointer at end of file
    int inSizeBytes = inFile.tellg(); // read pointer position
    std::cout << inSizeBytes << std::endl;




    /*
    Build a function to read the WAV header
    */




    // Set pointer after WAV header
    inFile.seekg(44); // else: inFile.seekg(0, std::ios::beg); // return pointer to beginning of file

    // Generate vector of the right size
    int signalSizeBytes = inSizeBytes - 44;
    std::vector<uint8_t> signalPCM (signalSizeBytes);

    // Generate data container
    //uint8_t data; // would be int16_t for 16-bit signal


    // Read file
    //while (inFile.eof() == 0) {

    inFile.seekg(44);
    

    // DEFINE
    const int sampleRate = SAMPLE_RATE;
    const int samplesPerUnit = sampleRate * TIME_UNIT / 1000;
    const int bytesPerUnit = samplesPerUnit * 1;

    const int frequency = FREQUENCY;
    const int freqError = frequency * 0.1; // 10% error
    const int freqLower = frequency - freqError;
    const int freqUpper = frequency + freqError;
    

    // Calculate number of units in signal
    int unitNb = floor(signalSizeBytes / bytesPerUnit);

    // Initialise bool vector
    std::vector<bool> signalVector (unitNb);

    for (int unitIndex = 0; unitIndex < unitNb; unitIndex ++) {
        
        // Create unit array
        uint8_t dataUnit [samplesPerUnit];
        
        // Read file
        inFile.read(reinterpret_cast<char*>(&dataUnit), samplesPerUnit);


        // Calculate pseudofrequency
        float pseudoFrequency = calcPseudoFrequencySquare(dataUnit, samplesPerUnit, sampleRate, 0, 255);
        //std::cout << pseudoFrequency << std::endl;


        if ((pseudoFrequency >= freqLower) && (pseudoFrequency <= freqUpper)) {
            //std::cout << true << std::endl;
            signalVector[unitIndex] = true;
        }
        else {
            //std::cout << false << std::endl;
            signalVector[unitIndex] = false;
        };


    };


    // Close input file
    inFile.close();



    //---------------------


    for (std::vector<bool>::iterator it = signalVector.begin(); it < signalVector.end(); it ++) {
        std::cout << *it;
    };
    std::cout << std::endl; std::cout << std::endl;


    //---------------------




    int lowSize = 0; // size of gap (false)
    int highSize = 0; // size of signal (true)

    // Initialise temporary character vector
    std::vector<std::string> messageVector;
    std::vector<char> tempChar;

    // Run through bool vector
    for (int unitIndex = 0; unitIndex < unitNb; unitIndex ++) {

        // Read data
        bool signalBool = signalVector[unitIndex];

        /*
        std::cout << "Processing bool: " << signalBool << std::endl;
        std::cout << "   Size of preceding low: " << lowSize << std::endl;
        std::cout << "   Size of preceding high " << highSize << std::endl;
        */

        // In gap
        if (signalBool == false) {

            if (highSize != 0) {

                // Save symbol to tempChar (impulses <=1 are dots; impulses >1 are dashes)
                if (highSize <= 1) {
                    tempChar.push_back('0');
                }
                else {
                    tempChar.push_back('1');
                };

                // Reset highSize
                highSize = 0;
            };

            // Increment lowSize
            lowSize ++;
        }

        // End of gap
        else {

            // Add gap
            if (lowSize != 0) {

                // Symbol space (within same character) -- do nothing
                if (lowSize == XCHAR_BREAK_TIME) {}

                // Spacing character (default: character space)
                else {

                    // Save tempChar to main vector if not empty
                    if (!tempChar.empty()) {

                        // Convert tempChar vector to string
                        std::string tempString (tempChar.begin(), tempChar.end());

                        // Add tempChar to main vector
                        messageVector.push_back(tempString);

                        // Reset tempChar
                        clear(tempChar);
                    };

                    // Process breakString
                    if (lowSize >= SENTENCE_BREAK_TIME) { // Sentence break (fullstop)
                        std::string breakString = "__fullstop__";

                        // Add breakString to main vector
                        messageVector.push_back(breakString);
                    }
                    else if (lowSize >= WORD_BREAK_TIME) { // Word space
                        std::string breakString = "__space__"; // and "__space__"
                        
                        // Add breakString to main vector
                        messageVector.push_back(breakString);
                    };  
                };

                // Reset lowSize
                lowSize = 0;

            };

            // Increment highSize
            highSize ++;
            
        };


    };


    // Process last char, if any
    if (highSize != 0) {

        // Save symbol to tempChar (impulses <=1 are dots; impulses >1 are dashes)
        if (highSize <= 1) {
            tempChar.push_back('0');
        }
        else {
            tempChar.push_back('1');
        };

        // Reset highSize
        highSize = 0;
    };

    // Save tempChar to main vector if not empty
    if (!tempChar.empty()) {

        // Convert tempChar vector to string
        std::string tempString (tempChar.begin(), tempChar.end());

        // Add tempChar to main vector
        messageVector.push_back(tempString);

        // Reset tempChar
        clear(tempChar);
    };


    //---------------------



    for (std::vector<std::string>::iterator it = messageVector.begin(); it < messageVector.end(); it ++) {
        std::cout << *it << std::endl;
    };


    

};