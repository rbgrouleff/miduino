#include <MIDI.h>
#include <PinChangeInt.h>
#include <EventManager.h>

#define KICK 36
#define SNARE 38
#define FOO 40
#define LEDPIN 13

#define KEY_A 2
#define KEY_B 3
#define KEY_C 4

#define PITCHBEND_PIN 0

int rawPitchBendValue;
EventManager events;

MIDI_CREATE_DEFAULT_INSTANCE();

byte keyToNote(int key)
{
  byte note;
  switch(key)
  {
    case KEY_A:
      note = KICK;
      break;
    case KEY_B:
      note = SNARE;
      break;
    case KEY_C:
      note = FOO;
      break;
    default:
      note = 0;
      break;
  }
  return note;
}

void keyUp(int event, int key)
{
  byte note = keyToNote(key);
  if (note)
  {
    MIDI.sendNoteOff(note, 127, 1);
  }
}

void keyDown(int event, int key)
{
  byte note = keyToNote(key);
  if (note)
  {
    MIDI.sendNoteOn(note, 127, 1);
  }
}

void interruptHandler()
{
  int pinState = PCintPort::pinState;
  int key = PCintPort::arduinoPin;
  if (pinState == LOW)
  {
    events.queueEvent(EventManager::kEventKeyPress, key);
  } else {
    events.queueEvent(EventManager::kEventKeyRelease, key);
  }
}

void initDigitalInput(byte pin)
{
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}

void setup()
{
  events.addListener(EventManager::kEventKeyPress, keyDown);
  events.addListener(EventManager::kEventKeyRelease, keyUp);
  MIDI.begin();
  Serial.begin(115200);
  initDigitalInput(KEY_A);
  PCintPort::attachInterrupt(KEY_A, &interruptHandler, CHANGE);
  initDigitalInput(KEY_B);
  PCintPort::attachInterrupt(KEY_B, &interruptHandler, CHANGE);
  initDigitalInput(KEY_C);
  PCintPort::attachInterrupt(KEY_C, &interruptHandler, CHANGE);
}

void loop()
{
  unsigned short newRawPitchBendValue = analogRead(PITCHBEND_PIN);
  if (newRawPitchBendValue != rawPitchBendValue)
  {
    rawPitchBendValue = newRawPitchBendValue;
    int pitchBendValue = map(rawPitchBendValue, 0, 1023, MIDI_PITCHBEND_MAX, MIDI_PITCHBEND_MIN);
    MIDI.sendPitchBend(pitchBendValue, 1);
  }
  events.processEvent();
}
