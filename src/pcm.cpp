/*
This file handles encoding to and decoding from the PCM format.
*/


#include <iostream>
#include <fstream> // file rw, TEMP IMPORT

#include <vector>
#include <queue>

#include "config.h"



// 8-bit signal -- otherwise too annoying to deal with endianness
void fillNoise (std::vector<uint8_t>& signalPCM,
                const int& startPos,
                const int& stopPos,
                const int& channelNb) {

    // Add values
    for (int sampleId = startPos; sampleId < stopPos; sampleId++) {

        for (int ch = 0; ch < channelNb; ch++) {
            uint8_t data = rand() % 256; // Random from 0 to 255
            signalPCM[sampleId] = data; //.push_back(data);
        };
    };
};




// 8-bit signal -- otherwise too annoying to deal with endianness
void fillFreqSquare (std::vector<uint8_t>& signalPCM,
                     const int& startPos,
                     const int& stopPos,
                     const int& frequency,
                     const int& sampleRate, // must be > 2 * frequency (Nyquist-Shannon)
                     const int& amplitude, // 255 for 8-bit signal
                     const int& channelNb) {


    // square signal: one 1 every n 0
    int sampleDelta = sampleRate / frequency;

    // Add values
    for (int sampleId = startPos; sampleId < stopPos; sampleId++) {

        uint8_t data;

        if (sampleId % sampleDelta == 0) { // or (sampleId - startPos) % sampleDelta, but should be the same for large intervals
            data = amplitude;
        }
        else {
            data = 0;
        };

        for (int ch = 0; ch < channelNb; ch++) {
            signalPCM[sampleId] = data;
        };
    };
};



// chunk of frequency when true, chunk of nothing when false
// one channel, amplitude is 255
// requirement (Nyquist-Shannon): frequency * 2 < sampleRate
void fillFromQueue (std::vector<uint8_t>& signalPCM, // of the right size
                    std::queue<bool>& signalQueue, // passage par référence: the queue is emptied
                    const int& frequency, // 440Hz
                    const int& sampleRate) {

    // Define constants
    const int amplitude = 255; // 8-bit signal
    const int channelNb = CHANNEL_NB; // 1

    // Calculate number of samples per unit
    const int samplesPerUnit = sampleRate * TIME_UNIT / 1000;

    // Initialise sampleId
    int sampleId = 0;

    // Iterate through queue
    while (!signalQueue.empty()) {

        // Get first item of signal queue
        bool signalBool = signalQueue.front();

        // Generate tone
        if (signalBool == true) {
            
            // Fill unit with frequency
            fillFreqSquare(signalPCM, sampleId, sampleId + samplesPerUnit, frequency, sampleRate, amplitude, channelNb);

            // Increment sampleId
            sampleId += samplesPerUnit;
        }
        else {
            
            // Fill unit with 0
            for (int i=0; i<samplesPerUnit; i++) {
                signalPCM[sampleId + i] = 0;
            };

            // Increment sampleId
            sampleId += samplesPerUnit;
        };


        // Remove first item from signal queue
        signalQueue.pop();

        
    };
};
