
#include <string>
#include <queue>

#include "config.h"



/**
 * Convert from morse sequence queue to signal bytestream queue
 * Will empty conversionQueue and fill signalQueue
 *
 * @param conversion_queue Morse sequence queue
 * @param signalQueue Signal queue (in units of TIME_UNIT)
 * @return
 */
void queueToSignal (std::queue<std::string>& conversionQueue,
                    std::queue<bool>& signalQueue) {

    // Define conversion constants (to move)
    const std::string dot_repr = "0";
    const std::string dash_repr = "1";
    const std::string fullstop = ".";
    const std::string space = "_";

    // Initialise gap request flag
    bool queueGap = false;

    // Initialise eof (end of field) flag
    bool eof = false;


    // Run while conversionQueue is not empty
    while (!conversionQueue.empty()) {

        // Get first item of conversion queue
        std::string convChar = conversionQueue.front();



        // Run through element's impulses in Morse code
        for (std::string::iterator it = convChar.begin(); it != convChar.end(); it++) {

            // Convert from char to string
            std::string c; std::string cc;
            c = *it;

            // Dot & Dash
            if ((c == dot_repr) || (c == dash_repr)) {

                // Add gap between characters (if no space character before/after)
                if (queueGap == true) {
                    //std::cout << ">gap" << std::endl;

                    for (int i=0; i<CHAR_BREAK_TIME; i++) {
                        signalQueue.push(false);
                    };

                    // Update flag
                    queueGap = false;
                }

                // Add gap between impulses (if no space character before/after)
                else { // if not first character
                    for (int i=0; i<XCHAR_BREAK_TIME; i++) {
                        signalQueue.push(false);
                    };
                };

                // Get impulse length
                int impulseTime;
                if (c == dot_repr) {
                    impulseTime = DOT_TIME;
                    //std::cout << "dot" << std::endl;
                }
                else if (c == dash_repr) {
                    impulseTime = DASH_TIME;
                    //std::cout << "dash" << std::endl;
                };

                // Add impulse
                for (int i=0; i<impulseTime; i++) {
                    signalQueue.push(true);
                };

                // Reset eof (end of field) flag
                eof = false;

            }



            // Space & Fullstop
            else if ( (eof == false) && ((c == space) || (c == fullstop)) ) {

                // Reset gap request flag
                queueGap = false;

                // Get gap length
                int gapTime;
                if (c == space) {
                    gapTime = WORD_BREAK_TIME;
                    //std::cout << ">>>SPACE" << std::endl;
                }
                else if (c == fullstop) {
                    gapTime = SENTENCE_BREAK_TIME;
                    //std::cout << ">>>FULLSTOP" << std::endl;
                };

                // Add gap
                for (int i=0; i<gapTime; i++) {
                    signalQueue.push(false);
                };

                // Update eof flag
                eof = true;
            };

        };

        // Remove first item from conversion queue
        conversionQueue.pop();

        // Request gap between characters (will apply if queue not empty)
        if (eof == false) {
            queueGap = true;
        };
        
    };

};
