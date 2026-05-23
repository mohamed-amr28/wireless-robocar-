#ifndef SAVEPATH_H
#define SAVEPATH_H

#include <Arduino.h>

#define MAX_STEPS 100

typedef struct {
  char command;
  unsigned long duration_ms;
} PathStep;

typedef enum {
  PATH_IDLE,
  PATH_RECORDING,
  PATH_PLAYING
} PathState;

extern PathState pathState;
extern PathStep recordedPath[MAX_STEPS];
extern int recordedCount;

void startRecording();
void stopRecording();
void startPlayback();
void stopPlayback();
void savepath_onCommand(char cmd);
void runSavePath();

#endif
