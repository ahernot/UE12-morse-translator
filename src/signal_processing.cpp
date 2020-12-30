/**
 * Signal analysis
 */

#include <iostream>

/**
 * Calculate pseudo-frequency of square signal (binary HIGH/LOW signal)
 * 
 * 
 */
float calcPseudoFrequencySquare (uint8_t dataUnit [],
                                 const int& unitSize,
                                 const int& sampleRate,
                                 const int& low,
                                 const int& high) {

    // Initialise variables
    float averageGap = 0;
    int averageId = 1;
    int tempGap = 0;

    for (int i = 0; i < unitSize; i ++) {

        // Read data
        uint8_t data = dataUnit[i];
        int dataInt = (int) data;

        // 
        if (dataInt == high) {
            averageGap = (averageId * averageGap + tempGap) / (averageId + 1); // add to average
            averageId ++; // increment nb of elements averaged
            tempGap = 0; // reset gap
        }
        else {
            tempGap ++;
        };
    };

    float pseudoFrequency = sampleRate / averageGap;

    // Prevent infinite value
    if (isinf(pseudoFrequency)) {
        return 0;
    };

    return pseudoFrequency;

};
