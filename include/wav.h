#include <string>
#include <vector>

#include "config.h"

float calcPseudoFrequencySquare (uint8_t dataUnit [],
                                 const int& unitSize,
                                 const int& sampleRate,
                                 const int& low,
                                 const int& high);

void generateWAV (std::vector<uint8_t>& signalPCM,
                  const std::string& outFilePath);

void readWAV (const std::string& inFilePath,
              std::vector<bool>& signalVector,
              const int& frequency = FREQUENCY,
              const int& sampleRate = SAMPLE_RATE,
              const int& timeUnit = TIME_UNIT,
              const int& headerSizeBytes = 44);
