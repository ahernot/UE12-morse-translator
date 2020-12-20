/*
g++ -std=c++11 file-tests.cpp -o t2 && ./t2
*/


#include <iostream>
#include <sstream>
#include <fstream>
#include <codecvt>


std::wstring readFile (const char* filename) {

    std::locale empty_locale (""); // empty locale

    std::wifstream wif(filename);
    wif.imbue( std::locale(empty_locale, new std::codecvt_utf8<wchar_t>) );

    std::wstringstream wss;
    wss << wif.rdbuf();

    return wss.str();
};


int main () {

    std::wstring ws = readFile("resources/preconversion.txt");
    //std::wcout << ws << std::endl;
    std::cout << ws.size() << std::endl;

    for (std::wstring::iterator it = ws.begin(); it != ws.end(); it++) {
        std::wcout << *it << std::endl;
    };

    return 0;
};