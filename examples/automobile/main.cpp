#include <iostream>

#include "auto.h"

using namespace std;


/**
 * @brief main
 * @return != 0 for any errors
 */
int main()
{
    Auto *pVW = new VW();
    if (!pVW) {
        cerr << "Error: failed to initialize automobile object!" << endl;
        return -1;
    }

    pVW->starten();
    pVW->beschleunigen();

    return 0;
}
