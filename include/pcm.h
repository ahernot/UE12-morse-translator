#include <iostream>
#include <vector>

#include "config.h"


void fillNoise (std::vector<uint8_t>& signalPCM,
                const int& startPos,
                const int& stopPos,
                const int& channelNb);

void fillFreqSquare (std::vector<uint8_t>& signalPCM,
                     const int& startPos,
                     const int& stopPos,
                     const int& frequency,
                     const int& sampleRate,
                     const int& amplitude,
                     const int& channelNb);

void fillFromQueue (std::vector<uint8_t>& signalPCM,
                    std::queue<bool>& signalQueue,
                    const int& frequency = FREQUENCY,
                    const int& sampleRate = SAMPLE_RATE);

void fillFromSignal (std::vector<bool>& signalVector,
                     std::vector<std::string>& messageVector,
                     const int& unitNb);
