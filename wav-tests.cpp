/*
g++ -std=c++11 wav-tests.cpp -o wt
./wt
*/

// http://www.topherlee.com/software/pcm-tut-wavformat.html
// https://gist.github.com/csukuangfj/c1d1d769606260d436f8674c30662450 -> HOW TO WRITE ON POSITION, HANDLE POINTER...
// https://gist.github.com/grogy/5137278
// https://medium.com/@zekumorudoragonhatto/how-to-work-with-binary-files-in-c-520a852ee04a meh

#include <iostream>
#include <fstream> // file rw







class WAVContainer {
    private:

        void init_header () {


        };


    public:

        void write () {}; // from a std::vector
        void save () {};

};









int main () {

    std::cout << "Running" << std::endl;

    /*
    // Create binary output file stream
    std::ofstream file_bin_out;
    
    // Write to file in BINARY mode
    file_bin_out.open("output.bin", std::ios::binary);
    file_bin_out.write((char*) &"Hello World", sizeof("Hello World"));
    file_bin_out.close();
    */

    /*IMPORTANT
    int num = 0x12037F; // will write 7F 03 12
    file_wav.write(reinterpret_cast<const char*>(&num), sizeof(num));*/
    //std::cout << 0x37F << std::endl; std::cout << 0b010 << std::endl;

    // WAV file attributes
    int bitSize = 16; // 0x00000010
    int channels = 1; // 0x0001
    int sampleRate = 44100; // 0xAC44
    int bitsPerSample = 16;

    // Filename
    const char* wav_filename = "test.bin";

    // Create output file stream
    std::ofstream file_wav;
    
    // Open file in binary mode
    file_wav.open(wav_filename, std::ios::binary);
    


    /*
    // -- BEGIN WAV HEADER -- //

    // [0-3] "RIFF" header
    file_wav.write((char*)&"RIFF", 4); //sizeof("RIFF"));

    // [4-7] File size
    int filesize = 0x00000000;
    file_wav.write(reinterpret_cast<const char*>(&filesize), sizeof(filesize));

    // [8-11] "WAVE" header
    file_wav.write((char*)&"WAVE", 4);

    // [12-15] "fmt " header (format chunk marker)
    file_wav.write((char*)&"fmt ", 4);

    // [16-19] Bit size
    file_wav.write(reinterpret_cast<const char*>(&bitSize), sizeof(bitSize));

    // [20-21] Format type (1 = PCM)
    int format_type = 1; // 0x0001
    file_wav.write(reinterpret_cast<const char*>(&format_type), 2);

    // [22-23] Number of channels
    file_wav.write(reinterpret_cast<const char*>(&channels), 2);

    // [24-27] Sample rate
    file_wav.write(reinterpret_cast<const char*>(&sampleRate), sizeof(sampleRate));

    // [28-31] Sample Rate * BitsPerSample * Channels / 8
    int val1 = sampleRate * bitsPerSample * channels / 8;
    file_wav.write(reinterpret_cast<const char*>(&val1), 4);

    // [32-33] BitsPerSample * Channels / 8
    int val2 = bitsPerSample * channels / 8;
    file_wav.write(reinterpret_cast<const char*>(&val2), 2);

    // [34-35] Bits per sample (bitSize * channels)
    file_wav.write(reinterpret_cast<const char*>(&bitsPerSample), 2);

    // [36-39] "data" chunk header. Marks the beginning of the data section.
    file_wav.write((char*)&"data", 4);

    // [40-43] File size (data)
    int dataSize = 0x00000000;
    file_wav.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));

    // -- END WAV HEADER -- //
    */



    /* CLEANER APPROACH, FOR LATER
    // RIFF Chunk Descriptor
    uint8_t riffHeader[4] = {'R', 'I', 'F', 'F'}; // RIFF Header Magic header
    uint32_t riffChunkSize; // RIFF Chunk Size
    uint8_t waveHeader[4] = {'W', 'A', 'V', 'E'}; // WAVE Header

    // "fmt" sub-chunk
    uint8_t fmtHeader[4] = {'f', 'm', 't', ' '}; // FMT header
    uint32_t fmtChunkSize = 16; // Size of the fmt chunk
    uint16_t audioFormat = 1; // Audio format 1=PCM, 6=mulaw, 7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t channels = 1; // Number of channels 1=Mono 2=Sterio
    uint32_t samplesPerSec = 16000; // Sampling Frequency in Hz
    uint32_t bytesPerSec = 16000 * 2; // bytes per second
    uint16_t blockAlign = 2; // 2=16-bit mono, 4=16-bit stereo
    uint16_t bitsPerSample = 16; // Number of bits per sample

    // "data" sub-chunk
    uint8_t dataHeader[4] = {'d', 'a', 't', 'a'}; // "data"  string
    uint32_t dataChunkSize; // Sampled data length
    */
    

    // Close file
    file_wav.close();




    return 0;
};