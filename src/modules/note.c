#include <math.h>
#include "../../include/modules/note.h"

float freq(int octave, char key) {
    float freq = 0.0;

    switch (key) {
        case 'c':
            freq = c;
            break;
        case 'C':
            freq = C;
            break;
        case 'd':
            freq = d;
            break;
        case 'D':
            freq = D;
            break;
        case 'e':
            freq = e;
            break;
        case 'f':
            freq = f;
            break;
        case 'F':
            freq = F;
            break;
        case 'g':
            freq = g;
            break;
        case 'G':
            freq = G;
            break;
        case 'a':
            freq = a;
            break;
        case 'A':
            freq = A;
            break;
        case 'b':
            freq = b;
            break;
    }

    float result = freq * pow(2, ++octave);

    return result;
}

float duration(Duration duration) {
    return pow(2, (int)(duration-2));
}
