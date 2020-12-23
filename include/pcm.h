#include <vector>

void generateFreqSquare (std::vector<uint8_t>& signalPCM,
                         const int& startPos,
                         const int& stopPos,
                         const int& frequency,
                         const int& sampleRate, // must be > 2 * frequency (Nyquist-Shannon)
                         const int& amplitude, // 255 for 8-bit signal
                         const int& channelNb);