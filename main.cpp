#include <iostream>

// #include "stdlib.h"

#include "config.h"

#include "morse.h"
#include "signal.h"
#include "pcm.h"
#include "wav.h"







void textToMorseAudio (std::string& text, // cannot be const because of iterator
                       const std::string outFilePath,
                       std::map<std::string, std::string> conversionMap,
                       std::map<std::string, std::string> preconversionMap) {
    

    // Convert from text to morse sequence
    std::queue<std::string> conversionQueue = stringToMorse(text, conversionMap, preconversionMap);
    
    // Convert from morse sequence to signal bytestream queue
    std::queue<bool> signalQueue;
    queueToSignal(conversionQueue, signalQueue); // check that conversion_queue remains unmodified (ideally would be popped and deleted)

    // Generate PCM signal to include into WAV file
    std::vector<int> signalPCM = generateSignalPCM (signalQueue, 44100, 1);
    

    };



int main () {


    // Generate the default preconversion map (accents are treated as several characters so don't really matter..)
    std::map<std::string, std::string> preconversion_map;
    fill_conversion_map (preconversion_map, "resources/preconversion.txt");

    // Generate the conversion map
    std::map<std::string, std::string> conversion_map;
    fill_conversion_map (conversion_map, "resources/conversion.txt");



    const std::string outFilePath = "output/output.wav";




    return 0;
};