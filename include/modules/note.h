#define c 16.35;
#define C 17.32;
#define d 18.35;
#define D 19.45;
#define e 20.6;
#define f 21.83;
#define F 23.12;
#define g 24.5;
#define G 25.96;
#define a 27.5;
#define A 29.14;
#define b 30.87;

#ifndef NOTE_HEADER
#define NOTE_HEADER
typedef enum {
    SIXTEENTH, EIGHT, QUARTER, HALF, FULL
} Duration;

typedef struct note {
    int octave;
    char key;
    float phase;
    float freq;
    Duration duration; 
    struct note *origin;
} Note;
#endif

float freq(int octave, char key);
float duration(Duration duration);
