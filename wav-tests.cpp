/*
g++ -std=c++11 wav-tests.cpp -o wt
./wt
*/

// http://www.topherlee.com/software/pcm-tut-wavformat.html
// https://gist.github.com/csukuangfj/c1d1d769606260d436f8674c30662450

#include <iostream>
#include <fstream> // file rw



class WAVContainer {
    private:

    public:
        void write () {};
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
    int samplingRate = 44100; // 0xAC44
    int bitsPerSample = 16;

    // Filename
    const char* wav_filename = "test.bin";

    // Create output file stream
    std::ofstream file_wav;
    
    // Open file in binary mode
    file_wav.open(wav_filename, std::ios::binary);
    

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

    // [24-27] Sampling rate
    file_wav.write(reinterpret_cast<const char*>(&samplingRate), sizeof(samplingRate));

    // [28-31] Sample Rate * BitsPerSample * Channels / 8
    int val1 = samplingRate * bitsPerSample * channels / 8;
    file_wav.write(reinterpret_cast<const char*>(&val1), 4);

    // [32-33] BitsPerSample * Channels / 8
    int val2 = bitsPerSample * channels / 8;
    file_wav.write(reinterpret_cast<const char*>(&val2), 2);

    // [34-35] Bits per sample
    file_wav.write(reinterpret_cast<const char*>(&bitsPerSample), 2);

    // [36-39] "data" chunk header. Marks the beginning of the data section.
    file_wav.write((char*)&"data", 4);

    // [40-43] File size (data)
    int dataSize = 0x00000000;
    file_wav.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));

    // -- END WAV HEADER -- //
    

    // Close file
    file_wav.close();




    return 0;
};