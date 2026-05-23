#include "savepath1.h"
#include "manual1.h"               
#include "../HAL/motor1.h"         
#include "../HAL/bluetooth1.h"     
PathState pathState = PATH_IDLE;
PathStep  recordedPath[MAX_STEPS];
int       recordedCount = 0;

static char          _lastCmd      = 'S';
static unsigned long _cmdStartTime = 0;
static int           _playIndex    = 0;
static unsigned long _stepStartTime = 0;

void startRecording() {
  recordedCount  = 0;
  _lastCmd       = 'S';
  _cmdStartTime  = millis();
  pathState      = PATH_RECORDING;
  Bluetooth_Send("PATH:REC_START");
}

void savepath_onCommand(char cmd) {
  if (pathState != PATH_RECORDING) return;

  unsigned long now      = millis();
  unsigned long held_for = now - _cmdStartTime;

  if (held_for > 30 && recordedCount < MAX_STEPS) {
    recordedPath[recordedCount].command     = _lastCmd;
    recordedPath[recordedCount].duration_ms = held_for;
    recordedCount++;
  }

  _lastCmd      = cmd;
  _cmdStartTime = now;
}

void stopRecording() {
  if (pathState != PATH_RECORDING) return;

  unsigned long held_for = millis() - _cmdStartTime;
  if (held_for > 30 && recordedCount < MAX_STEPS) {
    recordedPath[recordedCount].command     = _lastCmd;
    recordedPath[recordedCount].duration_ms = held_for;
    recordedCount++;
  }

  stopMotors();
  pathState = PATH_IDLE;

  if (recordedCount == 0) {
    Bluetooth_Send("PATH:EMPTY");
  } else {
    Bluetooth_Send("PATH:REC_DONE");
  }
}

void startPlayback() {
  if (recordedCount == 0) {
    Bluetooth_Send("PATH:EMPTY");
    return;
  }

  _playIndex     = 0;
  _stepStartTime = millis();
  pathState      = PATH_PLAYING;

  Bluetooth_Send("PATH:PLAYING");
  manualmode(recordedPath[0].command);
}

void stopPlayback() {
  pathState = PATH_IDLE;
  stopMotors();
  Bluetooth_Send("PATH:STOPPED");
}

void runSavePath() {
  if (pathState != PATH_PLAYING) return;

  unsigned long elapsed = millis() - _stepStartTime;

  if (elapsed >= recordedPath[_playIndex].duration_ms) {
    _playIndex++;

    if (_playIndex >= recordedCount) {
      stopPlayback();
      return;
    }

    _stepStartTime = millis();
    manualmode(recordedPath[_playIndex].command);
  }
}
