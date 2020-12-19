/*
g++ -std=c++11 tests.cpp -o t
./t
*/

#include <iostream>
#include <fstream> // file rw

#include <array>
#include <map>
#include <queue> // queue to add letters


/*
ARCHIVE MAP TO LOAD LATER (in v2)
void generate_map () {

    std::ofstream ofs("output_file");

    // create class instance
    std::map<int,std::string> mymap;

    // populate map.
    mymap[0] = "the"; mymap[1] = "they";

    // save data to archive
    {
        boost::archive::text_oarchive oa(ofs);
        // write map instance to archive
        oa << mymap;
        // archive and stream closed when destructors are called
    };
};
*/


void generate_map () {

    std::ifstream file_in;

    file_in.open("resources/conversion.txt");

    //std::string line;

    for( std::string line; getline( file_in, line ); ) {
        //std::cout << line << std::endl;

        char letter; char sequence;
        file_in >> letter >> sequence;
        std::cout << letter << " = " << sequence << std::endl;
        
    };

    std::cout << file_in.eof() << std::endl; // end of file (bool)

    file_in.close();


    /*
    // Read word by word (space as separator) - uses pointer
    file_in.open("output.txt");
    std::string word;
    file_in >> word; std::cout << word << std::endl; // word 1
    file_in >> word; std::cout << word << std::endl; // word 2
    std::cout << file_in.eof() << std::endl; // end of file (bool)
    file_in.close();*/

};





void convert_char(char& c) {

    std::cout << "Converting character '" << c << "'" << std::endl;

    // Look if found in standard map




}



void convert_from_text(std::string message) {

    // Loop through the characters in the message string
    for (std::string::iterator it = message.begin(); it != message.end(); it++) {
        // Convert individual character
        
    }

};



int main () {

    generate_map();

    //char x = 'A';
    //convert_char(x);

    return 0;
};