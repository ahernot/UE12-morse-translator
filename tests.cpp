/*
g++ -std=c++11 tests.cpp -o t
./t
*/

// in convert_char std::map can't be const for some reason...

#include <iostream>
#include <fstream> // file rw
#include <sstream>

#include <array>
#include <map>
#include <queue> // queue to add letters

// TO DEFINE IN config.h
#define TIME_UNIT 250 // 100 // 10ms 

#define DASH_TIME 3 // length of dash
#define DOT_TIME 1 // length of dot

#define HIGH_REPR 1 // yes signal
#define LOW_REPR 0 // no signal

#define XCHAR_BREAK_TIME 1 // gap between impulses
#define CHAR_BREAK_TIME 3 // gap between characters
#define WORD_BREAK_TIME 7 // gap between words (space)
#define SENTENCE_BREAK_TIME 9 // full-stop





/**
 * Fill a string->string conversion map with data from a TXT file
 *
 * @param conversion_map Conversion map to fill
 * @param fill_filepath Path of file containing map information
 */
void fill_conversion_map (std::map<std::string, std::string>& conversion_map, const std::string& fill_filepath) {

    // Open the conversion file
    std::ifstream file_in;
    file_in.open(fill_filepath);

    // Initialise the loop variables
    std::string line;
    std::string letter; std::string sequence;

    // Get the first line
    file_in >> letter >> sequence;
    //std::cout << letter << " = " << sequence << std::endl;

    // Write to the map
    conversion_map[letter] = sequence;
    
    // Loop through the file's lines
    while (std::getline(file_in, line)) {
        
        // Read the line
        file_in >> letter >> sequence;
        //std::cout << letter << " = " << sequence << std::endl;

        // Write to the map
        conversion_map[letter] = sequence;

    };

    // Close the conversion file
    file_in.close();

};



/**
 * Convert string to morse code binary representation
 *
 * @param c String (character) to convert
 * @param conversion_map Conversion map from strings to morse code binary representation in string form
 * @param preconversion_map Preconversion map to convert to accepted strings for morse code conversion
 * @return Morse code binary representation of string input (one character only)
 */
std::string convert_char (std::string& c,
                   std::map<std::string, std::string>& conversion_map,
                   std::map<std::string, std::string>& preconversion_map) {

    // Preconversion
    std::string cp = preconversion_map[c];

    // Replace c with preconverted version if not empty
    if (!( cp.empty() )) {
        c = cp; // Replace c with preconverted c
    };

    // Conversion
    std::string cc = conversion_map[c];

    return cc;
};



/**
 * Convert string to morse code binary representation
 *
 * @param c String (character) to convert
 * @param conversion_map Conversion map from strings to morse code binary representation in string form
 * @return Morse code binary representation of string input (one character only)
 */
std::string convert_char (std::string& c,
                          std::map<std::string, std::string>& conversion_map) {
    // Generate an empty map
    std::map<std::string, std::string> empty_map;
    
    // Run the conversion function
    std::string cc = convert_char(c, conversion_map, empty_map);

    return cc;
};




std::queue<std::string> convert_from_text (std::string& message,
                                           std::map<std::string, std::string>& conversion_map,
                                           std::map<std::string, std::string>& preconversion_map) {

    // Initialise queue
    std::queue<std::string> conversion_queue;

    const std::string fullstop = ".";
    const std::string space = "_";

    // Loop through the characters in the message string
    for (std::string::iterator it = message.begin(); it != message.end(); it++) {

        // Convert from char to string
        std::string c; std::string cc;
        c = *it;

        // Convert characters
        if (c == " ") {
            conversion_queue.push(space); // space (word break)
        }
        else if (c == ".") {
            conversion_queue.push(fullstop); // fullstop (sentence break)
        }
        else {
            std::string cc = convert_char(c, conversion_map, preconversion_map);
            conversion_queue.push(cc);
        };
        
    };

    return conversion_queue;

};





/**
 * Convert from morse sequence queue to signal bytestream queue
 *
 * @param conversion_queue Morse sequence queue
 * @return Signal bytestream queue
 */
std::queue<bool> queueToSignal (std::queue<std::string> conversion_queue) {

    // Initialise signal queue
    std::queue<bool> signal_queue;

    // Define conversion constants (to move)
    const std::string dot_repr = "0";
    const std::string dash_repr = "1";
    const std::string fullstop = ".";
    const std::string space = "_";

    // Initialise gap request flag
    bool queue_gap = false;

    // Initialise eof (end of field) flag
    bool eof = false;

    while (!conversion_queue.empty()) {

        // Get first item of conversion queue
        std::string conv_char = conversion_queue.front();

        // Run through element's impulses
        for (std::string::iterator it = conv_char.begin(); it != conv_char.end(); it++) {

            // Convert from char to string
            std::string c; std::string cc;
            c = *it;

            // Dot & Dash
            if ((c == dot_repr) || (c == dash_repr)) {

                // Add gap between characters (if no space character before/after)
                if (queue_gap == true) {
                    //std::cout << ">gap" << std::endl;

                    for (int i=0; i<CHAR_BREAK_TIME; i++) {
                        signal_queue.push(false);
                    };

                    // Update flag
                    queue_gap = false;
                }

                // Add gap between impulses (if no space character before/after)
                else { // if not first character
                    for (int i=0; i<XCHAR_BREAK_TIME; i++) {
                        signal_queue.push(false);
                    };
                };

                // Get impulse length
                int impulse_time;
                if (c == dot_repr) {
                    impulse_time = DOT_TIME;
                    //std::cout << "dot" << std::endl;
                }
                else if (c == dash_repr) {
                    impulse_time = DASH_TIME;
                    //std::cout << "dash" << std::endl;
                };

                // Add impulse
                for (int i=0; i<impulse_time; i++) {
                    signal_queue.push(true);
                };

                // Reset eof (end of field) flag
                eof = false;

            }



            // Space & Fullstop
            else if ( (eof == false) && ((c == space) || (c == fullstop)) ) {

                // Reset gap request flag
                queue_gap = false;

                // Get gap length
                int gap_time;
                if (c == space) {
                    gap_time = WORD_BREAK_TIME;
                    //std::cout << ">>>SPACE" << std::endl;
                }
                else if (c == fullstop) {
                    gap_time = SENTENCE_BREAK_TIME;
                    //std::cout << ">>>FULLSTOP" << std::endl;
                };

                // Add gap
                for (int i=0; i<gap_time; i++) {
                    signal_queue.push(false);
                };

                // Update eof flag
                eof = true;
            };

        };

        // Remove first item from conversion queue
        conversion_queue.pop();

        // Request gap between characters (will apply if queue not empty)
        if (eof == false) {
            queue_gap = true;
        };
        
    };

    
    return signal_queue;

};



// chunk of frequency when true, chunk of nothing when false
std::vector<int> generateSignalPCM (std::queue<bool> signalQueue, // pas de passage en référence
                                    const int& sampleRate,
                                    const int& channels) {
    /*
    sampleRate=44100Hz
    channels=1
    8-bit signal -- otherwise too annoying to deal with little endian etc
    */


    int high = 255; // max amplitude for 8-bit signal
    int low = 0;
    int frequency = 440; // Hz (A440)
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




int main () {

    // Generate the default preconversion map (accents are treated as several characters so don't really matter..)
    std::map<std::string, std::string> preconversion_map;
    fill_conversion_map (preconversion_map, "resources/preconversion.txt");

    // Generate the conversion map
    std::map<std::string, std::string> conversion_map;
    fill_conversion_map (conversion_map, "resources/conversion.txt");



    // Define message
    std::string message = "This is a message. Hello.";

    // Convert from text to morse sequence
    std::queue<std::string> conversion_queue = convert_from_text(message, conversion_map, preconversion_map);
    
    // Convert from morse sequence to signal bytestream queue
    std::queue<bool> signalQueue = queueToSignal(conversion_queue); // check that conversion_queue remains unmodified (ideally would be popped and deleted)



    /*// Print signal
    while (!signalQueue.empty()) {
        bool cbool = signalQueue.front();
        //std::cout << cbool << std::endl;
        signalQueue.pop();
    };*/


    // Generate PCM signal to include into WAV file
    std::vector<int> signalPCM = generateSignalPCM (signalQueue, 44100, 1);

    /*for (std::vector<int>::iterator it = signalPCM.begin(); it != signalPCM.end(); it++) {
        std::cout << *it << std::endl;
    };*/



    
    // ---------- Save output to a WAV file
    /* NEED TO ACTUALLY CALC FILE SIZE ETC
    // WAV file attributes
    int bitSize = 8;
    int channels = 1;
    int sampleRate = 44100;
    int bitsPerSample = 16;

    // Filename
    const char* wav_filename = "out.wav";

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
        uint32_t samplesPerSec = 16000; // Sampling Frequency in Hz
        uint32_t bytesPerSec = 16000 * 2; // bytes per second
        uint16_t blockAlign = 2; // 2=16-bit mono, 4=16-bit stereo
        uint16_t bitsPerSample = 16; // Number of bits per sample

        // "data" sub-chunk
        uint8_t dataHeader[4] = {'d', 'a', 't', 'a'}; // "data"  string
        uint32_t dataChunkSize; // Sampled data length
    } wav_hdr;


    static_assert(sizeof(wav_hdr) == 44, "");

    uint32_t fsize = sizeof(signalPCM); // in.tellg(); // get position of pointer (last element)
    //in.seekg(0, std::ios::end);
    //fsize = (uint32_t)in.tellg() - fsize;
    //in.seekg(0, std::ios::beg);

    printf("file size: %u\n", fsize);

    wav_hdr wav;
    wav.riffChunkSize = fsize + sizeof(wav_hdr) - 8;
    wav.dataChunkSize = fsize + sizeof(wav_hdr) - 44;

    std::ofstream out("out.wav", std::ios::binary);
    out.write(reinterpret_cast<const char *>(&wav), sizeof(wav));

    /*
    int16_t d;
    for (int i = 0; i < fsize; ++i) {
    // TODO: read/write in blocks
    in.read(reinterpret_cast<char *>(&d), sizeof(int16_t));
    out.write(reinterpret_cast<char *>(&d), sizeof(int16_t));
    }
    */


    for (std::vector<int>::iterator it = signalPCM.begin(); it != signalPCM.end(); it++) {
        out.write(reinterpret_cast<const char*>(&*it), 1); // file_wav.write()
    };





    return 0;
};