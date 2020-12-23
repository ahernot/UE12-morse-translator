/*
This file handles encoding to and decoding from the PCM format
g++ -std=c++11 pcm.cpp -o pcm
./pcm
*/


#include <iostream>
#include <fstream> // file rw, TEMP IMPORT

#include <vector>
#include <queue>

#define TIME_UNIT 10 // TEMP


// chunk of frequency when true, chunk of nothing when false
std::vector<int> signalFromQueue (std::queue<bool> signalQueue, // pas de passage en référence
                                    const int& frequency,
                                    const int& amplitude,
                                    const int& sampleRate,
                                    const int& channels) {
    /*
    sampleRate=44100Hz
    channels=1
    8-bit signal -- otherwise too annoying to deal with little endian etc
    */


    int high = 255; // max amplitude for 8-bit signal
    int low = 0;
    // int frequency = 440; // Hz (A440)
    // requirement (Nyquist-Shannon): frequency * 2 < sampleRate

    // Calculate size of output signal
    int samplesPerUnit = sampleRate * TIME_UNIT / 1000;
    int signalSize = sizeof(signalQueue) * samplesPerUnit;

    // Calculate number of null points between two impulses
    int pointDelta = sampleRate / frequency; // converts to int automatically (close enough if frequency << sampleRate)

    std::cout << "   sizeof signalQueue: " << sizeof(signalQueue) << std::endl;
    std::cout << "   sample rate: " << sampleRate << "Hz" << std::endl;
    std::cout << "   time unit: " << TIME_UNIT << "ms" << std::endl;
    std::cout << "   samplesPerUnit: " << samplesPerUnit << std::endl;
    std::cout << "   signalSize: " << signalSize << std::endl;
    std::cout << "   point delta: " << pointDelta << std::endl;

    
    // Initialise signal vector
    std::vector<int> signalPCM (signalSize);

    // Iterate through queue
    while (!signalQueue.empty()) {

        // Get first item of signal queue
        bool signalBool = signalQueue.front();

        //std::cout << "Processing " << signalBool << std::endl;

        // Generate tone
        if (signalBool == true) {

            //std::cout << "   signal" << std::endl;

            for (int i=0; i<samplesPerUnit; i++) {

                if (i % pointDelta == 0) {
                    signalPCM.push_back(high);
                }
                else {
                    signalPCM.push_back(low);
                };
            };
        }
        else {
            //std::cout << "   zeros" << std::endl;
            for (int i=0; i<samplesPerUnit; i++) {
                signalPCM.push_back(low);
            };
        };


        // Remove first item from signal queue
        signalQueue.pop();
    };

    return signalPCM;
};



// Get size of vector: vector.size()
// sizeof() function must be looking at bit size of first element


// 8-bit signal -- otherwise too annoying to deal with endianness
void generateNoise (std::vector<uint8_t>& signalPCM,
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
void generateFreqSquare (std::vector<uint8_t>& signalPCM,
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





typedef struct WAV_HEADER {
    // RIFF Chunk Descriptor
    uint8_t riffHeader[4] = {'R', 'I', 'F', 'F'}; // RIFF Header Magic header
    uint32_t riffChunkSize; // RIFF Chunk Size
    uint8_t waveHeader[4] = {'W', 'A', 'V', 'E'}; // WAVE Header

    // "fmt" sub-chunk
    uint8_t fmtHeader[4] = {'f', 'm', 't', ' '}; // FMT header
    uint32_t fmtChunkSize = 16; // Size of the fmt chunk
    uint16_t audioFormat = 1; // Audio format 1=PCM, 6=mulaw, 7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t channels = 1; // Number of channels 1=Mono 2=Sterio
    uint32_t samplesPerSec = 44100; // Sampling Frequency in Hz
    uint32_t bytesPerSec = 44100 * 1; // bytes per second
    uint16_t blockAlign = 1; // 1=8-bit mono I suppose, 2=16-bit mono, 4=16-bit stereo
    uint16_t bitsPerSample = 8; // Number of bits per sample

    // "data" sub-chunk
    uint8_t dataHeader[4] = {'d', 'a', 't', 'a'}; // "data"  string
    uint32_t dataChunkSize; // Sampled data length
} wav_hdr;







// main function just for tests
int main() {

    int sampleNb = 220500;
    int channelNb = 1;

    int vectorSize = sampleNb * channelNb;
    std::vector<uint8_t> signalPCM (vectorSize); // Initialise signal vector
    //generateNoise(signalPCM, 0, sampleNb, channelNb); // Fill signal vector with random noise
    generateFreqSquare(signalPCM, 0, sampleNb, 440, 44100, 255, 1);




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