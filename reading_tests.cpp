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

    std::string inFilePath = "output/hello-world.wav";

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
    int signalSizeBytes = inSizeBytes - 44
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

    /*
    for (int i=0; i<inSizeBytes; i++) {        

        // Read byte
        inFile.read(reinterpret_cast<char*>(&data), 1);//sizeof(uint8_t)); //reinterpret_cast<char *>(&data)

        std::cout << data << std::endl;


        if (i >= 44) { // skip header
            // Write byte to vector
            signalPCM[i] = data;
        };
    };
    */
    

    inFile.close();


    //inFile.read(reinterpret_cast<char *>(&data), sizeof(uint8_t));
    //std::cout << data << std::endl;

    //std::cout << inFile.eof() << std::endl; // not EOF :(

    
    for (std::vector<uint8_t>::iterator it = signalPCM.begin(); it<signalPCM.end(); it++) {
        //std::cout << *it << std::endl;
    };

};