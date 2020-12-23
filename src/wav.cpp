#include <iostream>
#include <fstream>

#include <vector>

#include "config.h"




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





/*


// main function just for tests
int main() {

    int sampleNb = 220500;
    int channelNb = 1;

    int vectorSize = sampleNb * channelNb;
    std::vector<uint8_t> signalPCM (vectorSize); // Initialise signal vector
    //generateNoise(signalPCM, 0, sampleNb, channelNb); // Fill signal vector with random noise
    fillFreqSquare(signalPCM, 0, sampleNb, 440, 44100, 255, 1);




    static_assert(sizeof(wav_hdr) == 44, "");
    uint32_t fsize = signalPCM.size() * 8; // size in bits | in.tellg(); // get position of pointer (last element)

    //in.seekg(0, std::ios::end);
    //fsize = (uint32_t)in.tellg() - fsize;
    //in.seekg(0, std::ios::beg);

    printf("file size: %u\n", fsize);

    wav_hdr wav;
    wav.riffChunkSize = fsize + sizeof(wav_hdr) - 8;
    wav.dataChunkSize = fsize + sizeof(wav_hdr) - 44;

    std::ofstream out("out/outpcm.wav", std::ios::binary);
    out.write(reinterpret_cast<const char *>(&wav), sizeof(wav));


    for (std::vector<uint8_t>::iterator it = signalPCM.begin(); it != signalPCM.end(); it++) {
        out.write(reinterpret_cast<const char*>(&*it), sizeof(uint8_t)); // sizeof(uint8_t) = 1 (size in bytes)
    };


    return 0;
};
*/