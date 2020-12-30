/**
 * WAV file writing and reading
 */


//#include <iostream>
#include <fstream>

#include <vector>

#include "config.h"
#include "signal_processing.h" // calcPseudoFrequencySquare


typedef struct WAV_HEADER {

    // "RIFF" Chunk Descriptor
    uint8_t riffHeader[4] = {'R', 'I', 'F', 'F'}; // RIFF Header Magic header
    uint32_t riffChunkSize; // RIFF Chunk Size
    uint8_t waveHeader[4] = {'W', 'A', 'V', 'E'}; // WAVE Header

    // "fmt" sub-chunk
    uint8_t fmtHeader[4] = {'f', 'm', 't', ' '}; // fmt header
    uint32_t fmtChunkSize = 16; // size of the fmt chunk
    uint16_t audioFormat = 1; // audio format 1=PCM, 6=mulaw, 7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t channels = CHANNEL_NB; // number of channels 1=Mono 2=Sterio
    uint32_t samplesPerSec = SAMPLE_RATE; // sampling Frequency in Hz
    uint32_t bytesPerSec = SAMPLE_RATE * CHANNEL_NB; // bytes per second
    uint16_t blockAlign = 1; // 1=8-bit mono, 2=16-bit mono, 4=16-bit stereo
    uint16_t bitsPerSample = 8; // number of bits per sample

    // "data" sub-chunk
    uint8_t dataHeader[4] = {'d', 'a', 't', 'a'}; // "data" string
    uint32_t dataChunkSize; // sampled data length

} wav_hdr;




void generateWAV (std::vector<uint8_t>& signalPCM, // cannot put const :(
                  const std::string& outFilePath) {

    // Check header
    static_assert(sizeof(wav_hdr) == 44, "wrong WAV header size");

    // Calculate size of PCM signal in bits
    uint32_t fsize = signalPCM.size() * 8;

    // Fill missing information in WAV header
    wav_hdr wav;
    wav.riffChunkSize = fsize + sizeof(wav_hdr) - 8;
    wav.dataChunkSize = fsize + sizeof(wav_hdr) - 44;

    // Create output file stream
    std::ofstream outFile;

    // Open file in binary mode
    outFile.open(outFilePath, std::ios::binary);

    // Write WAV header
    outFile.write(reinterpret_cast<const char*>(&wav), sizeof(wav));

    // Write PCM data
    for (std::vector<uint8_t>::iterator it = signalPCM.begin(); it != signalPCM.end(); it++) {
        outFile.write(reinterpret_cast<const char*>(&*it), sizeof(uint8_t)); // sizeof(uint8_t) = 1 (size in bytes)
    };

    // Close output file
    outFile.close();

};



void readWAV (const std::string& inFilePath,
              std::vector<bool>& signalVector,
              const int& frequency,
              const int& sampleRate,
              const int& timeUnit,
              const int& headerSizeBytes) {
    
    // Create input file stream
    std::ifstream inFile;

    // Open file in binary mode
    inFile.open(inFilePath, std::ios::binary);

    // Get size of file in bytes
    inFile.seekg(0, std::ios::end); // place pointer at end of file
    int inSizeBytes = inFile.tellg(); // read pointer position
    int signalSizeBytes = inSizeBytes - headerSizeBytes; // can be used to generate signalVector of the right size


    /*
    Build a function to read the WAV header
    */    
    

    // Calculate constants
    const int samplesPerUnit = sampleRate * timeUnit / 1000;
    const int bytesPerUnit = samplesPerUnit * 1;

    // Compute frequency acceptability range
    const int freqError = frequency * 0.1; // 10% error
    const int freqLower = frequency - freqError;
    const int freqUpper = frequency + freqError;
    
    // Calculate number of units in signal
    int unitNb = floor(signalSizeBytes / bytesPerUnit);

    // Set pointer after WAV header
    inFile.seekg(headerSizeBytes); // 44



    // Iterate through bytes
    for (int unitIndex = 0; unitIndex < unitNb; unitIndex ++) {
        
        // Create unit array
        uint8_t dataUnit [samplesPerUnit];
        
        // Read file
        inFile.read(reinterpret_cast<char*>(&dataUnit), samplesPerUnit);

        // Calculate pseudofrequency
        float pseudoFrequency = calcPseudoFrequencySquare(dataUnit, samplesPerUnit, sampleRate, 0, 255);

        // Compare read frequency and add to signalVector
        if ((pseudoFrequency >= freqLower) && (pseudoFrequency <= freqUpper)) {
            signalVector.push_back(true);
        }
        else {
            signalVector.push_back(false);
        };

    };



    // Close input file
    inFile.close();
};
