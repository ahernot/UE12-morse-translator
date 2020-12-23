# UE12 – Morse Translator (v0.3)
Project due 2021.01.24

<br><br>

## Compilation Instructions
* `cd` to the project folder
* run the `make` command in the shell
* to run, type `./morse` in the shell

<br><br>

## Changelog
* v0.1 (2020.12.19): Begin project, create text->Morse conversion functions
* v0.2 (2020.12.20): Create Morse->signal conversion function with smart support for spaces
* v0.3 (2020.12.23): Create signal->WAV conversion function, reorganise into package structure

<br><br>

## To do
* v0.4: Implement .wav file reading (WAV->PCMvector)
* v0.5: Implement backwards conversion (PCMvector->text)
* v2: Use json files or equivalent for conversion maps storage (or archive using `boost::archive::text_oarchive`)
* v2: Deal with accentuated characters better
* add `-std=$(CPP)` in makefile?


<br><br>


## About the program
This program implements a basic, uncompressed audio generation. As such, the output audio file is an 8-bit mono audio file. Given the binary nature of Morse code, a simple 2-bit file with a fixed sample rate would've been sufficient, but this program's goal is to make the Morse code generated human-understandable. As such, the tone is audible (440Hz) and lasts for significantly longer than needed (here, 150ms).

The following sources and documentation were of great help to me:
* A webpage documenting how a WAV header is constructed http://www.topherlee.com/software/pcm-tut-wavformat.html and a clean (cleaner than mine) C++ implementation https://gist.github.com/csukuangfj/c1d1d769606260d436f8674c30662450 
* A Gist explaining how to write to a binary file in C++ https://gist.github.com/grogy/5137278


<br><br>


## About the PCM data format
from https://stackoverflow.com/questions/49764773/how-is-audio-data-stored-in-raw-pcm-format
It is a RAW data storage format, and is the simplest data storage format — data is stored linearly and uncompressed.

<br>

Example for a stereo (L, R) 16-bit (2 bytes per sample) little-endian (smallest byte on the left) signal:
```
…  |    i-1    |     i     |    i+1    |  …  <- point id
…  |  L  |  R  |  L  |  R  |  L  |  R  |  …  <- data segments
…  |Ll|Lb|Rl|Rb|Ll|Lb|Rl|Rb|Ll|Lb|Rl|Rb|  …  <- in byte chunks (8 bits)
```

<br>

Example for a 3-channel (L, R, C) 32-bit (4 bytes per sample) little-endian (smallest byte on the left) signal:
```
…  |                i-1                |                 i                 |                i+1                |  …  <- point id
…  |     L     |     R     |     C     |     L     |     R     |     C     |     L     |     R     |     C     |  …  <- data segments
…  |L0|L1|L2|L3|R0|R1|R2|R3|C0|C1|C2|C3|L0|L1|L2|L3|R0|R1|R2|R3|C0|C1|C2|C3|L0|L1|L2|L3|R0|R1|R2|R3|C0|C1|C2|C3|  …  <- in byte chunks (8 bits)
```


<br><br>


## A few words on endianness

Data is stored in bytes (1 byte = 8 bits).
For 16-bit signals, each data point is spread across two bytes. Endianness refers to the order of the data.

For little-endian storage, the least significant (smallest) byte is on the left:
```
    0        1        2     <- byte id
01234567 01234567 01234567  <- byte position
•••••••• •••••••• ••••••••  <- data
```

and the number, once assembled, should read 2 $\|$ 1 $\|$ 0 (24-bit signal)


<br><br>


## Random notes

The `.size` function gets the size of a C++ object (`vector.size()` will return the number of elements in `vector`). The `sizeof()` function returns the bit size of the element at the pointer address of the object (hence, for a vector or any other 'recurrent' data structure, it is much lesser than the actual bit size of the whole object).
