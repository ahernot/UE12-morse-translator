#include <iostream>

#include <string>
#include <map>
#include <queue>
#include <vector>

#include "config.h"

#include "data_loader.h"
#include "morse.h"
#include "signal_gen.h"
#include "pcm.h"
#include "wav.h"




void textToMorseAudio (std::string& text, // cannot be const because of iterator
                       const std::string outFilePath,
                       std::map<std::string, std::string> conversionMap,
                       std::map<std::string, std::string> preconversionMap) {
    
    // Convert from text to morse sequence
    std::queue<std::string> conversionQueue;
    stringToMorse(text, conversionQueue, conversionMap, preconversionMap);

    
    // Convert from morse sequence to signal bytestream queue (empties conversionQueue)
    std::queue<bool> signalQueue;
    //signalQueue.push(true); signalQueue.push(false); signalQueue.push(true); signalQueue.push(false); signalQueue.push(true); signalQueue.push(false); signalQueue.push(true); signalQueue.push(false); signalQueue.push(true); signalQueue.push(false);
    queueToSignal(conversionQueue, signalQueue);


    // Generate PCM signal to include into WAV file
    const int samplesPerUnit = SAMPLE_RATE * TIME_UNIT / 1000;
    const int unitNb = signalQueue.size();
    const int signalSize = samplesPerUnit * unitNb * CHANNEL_NB;
    std::vector<uint8_t> signalPCM (signalSize);
    fillFromQueue (signalPCM, signalQueue, FREQUENCY, SAMPLE_RATE);

    
    // Generate WAV file
    generateWAV (signalPCM, outFilePath);

};




int main () {

    // Generate the default preconversion map (accents are treated as several characters so don't really matter..)
    std::map<std::string, std::string> preconversionMap;
    fillConversionMap (preconversionMap, "resources/preconversion.txt");

    // Generate the conversion map
    std::map<std::string, std::string> conversionMap;
    fillConversionMap (conversionMap, "resources/conversion.txt");

    // Generate the conversion map
    std::map<std::string, std::string> returnMap;
    fillConversionMap (conversionMap, "resources/conversion.txt", true);





    // Convert text to Morse audio file
    std::string outMessage = "Joyeux Noel";
    const std::string outFilePath = "output/output.wav";
    textToMorseAudio(outMessage, outFilePath, conversionMap, preconversionMap);


    // Convert Morse audio file to text
    const std::string inFilePath = "output/output.wav";
    std::string inMessage;
    //morseAudioToText(inMessage, inFilePath, )

    return 0;
};