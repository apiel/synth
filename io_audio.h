#ifndef IO_AUDIO_H_
#define IO_AUDIO_H_

#include <Arduino.h>
#include <Audio.h>

#include "io_audio_synth.h"
#include "io_audio_synth_modulation.h"
#include "io_audio_synth_storage.h"
#include "io_audio_wav.h"

AudioOutputMQS audioOut;

AudioConnection* patchCordSynth[SYNTH_COUNT];
AudioMixer4 mixer;
AudioConnection* patchCordMixer;

// there should be only one lfo, reseted only on synth 0

void initSynth(byte synthPos) {
    synth[synthPos].init();
    // saving and loading synth will be different
    // loadSynth(synthPos);
}

void audioInit() {
    AudioMemory(25);

    for (byte s = 0; s < SYNTH_COUNT; s++) {
        initSynth(s);
    }

    patchCordSynth[0] = new AudioConnection(synth[0], 0, mixer, 0);
    patchCordSynth[1] = new AudioConnection(synth[1], 0, mixer, 1);
    patchCordSynth[2] = new AudioConnection(synth[2], 0, mixer, 2);
    patchCordSynth[3] = new AudioConnection(synth[3], 0, mixer, 3);

    patchCordMixer = new AudioConnection(mixer, audioOut);
}

void synthNoteOn(byte note, byte velocity) {
    for (byte s = 0; s < SYNTH_COUNT; s++) {
        if (!synth[s].env.isActive()) {
            synth[s].noteOn(note, velocity);
            return;
        }
    }
}

void synthNoteOff(byte note) {
    for (byte s = 0; s < SYNTH_COUNT; s++) {
        if (synth[s].lastNote == note) {
            synth[s].noteOff(note);
            return;
        }
    }
}

#endif
