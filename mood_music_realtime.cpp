#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include "RtMidi.h"

using namespace std;

// 🎼 Scales for Different Moods
map<string, vector<int>> mood_scales = {
    {"happy", {60, 62, 64, 65, 67, 69, 71, 72}},      // C Major Scale
    {"sad", {60, 62, 63, 65, 67, 68, 70, 72}},        // C Minor Scale
    {"energetic", {60, 62, 65, 67, 70, 72}}           // C Pentatonic Scale
};

// 🎚️ Tempo Mapping
map<string, int> mood_tempo = {
    {"happy", 120},
    {"sad", 60},
    {"energetic", 150}
};

// 🎹 Instrument Mapping
map<string, int> mood_instruments = {
    {"happy", 0},      // Acoustic Grand Piano
    {"sad", 40},       // Violin
    {"energetic", 81}  // Synth Lead
};

// 🔄 Generate Markov transitions based on scale
map<int, vector<int>> buildTransitions(const vector<int>& scale) {
    map<int, vector<int>> transitions;
    for (size_t i = 0; i < scale.size(); ++i) {
        vector<int> next;
        if (i > 0) next.push_back(scale[i - 1]);
        if (i < scale.size() - 1) next.push_back(scale[i + 1]);
        next.push_back(scale[i]);  // Stay on the same note
        transitions[scale[i]] = next;
    }
    return transitions;
}

// 🎶 Generate Melody
vector<int> generateMelody(string mood, int length, map<int, vector<int>> transitions) {
    vector<int> melody;
    vector<int> scale = mood_scales[mood];

    int note = scale[rand() % scale.size()];
    for (int i = 0; i < length; ++i) {
        melody.push_back(note);
        vector<int> options = transitions[note];
        note = options[rand() % options.size()];
    }
    return melody;
}

// 🔊 Play Melody
void playMelody(vector<int> melody, string mood) {
    RtMidiOut midiOut;
    if (midiOut.getPortCount() == 0) {
        cout << "No MIDI output ports found!" << endl;
        return;
    }
    midiOut.openPort(0);

    vector<unsigned char> message;
    message.push_back(0xC0);
    message.push_back(mood_instruments[mood]);
    midiOut.sendMessage(&message);

    int tempo = mood_tempo[mood];

    for (int note : melody) {
        message = {0x90, (unsigned char)note, 100};
        midiOut.sendMessage(&message);
        this_thread::sleep_for(chrono::milliseconds(60000 / tempo));

        message = {0x80, (unsigned char)note, 0};
        midiOut.sendMessage(&message);
    }
    midiOut.closePort();
}

// 🎤 Main
int main() {
    srand(time(0));
    string mood;
    cout << "Enter mood (happy, sad, energetic): ";
    cin >> mood;

    if (mood_scales.find(mood) == mood_scales.end()) {
        cout << "Invalid mood!" << endl;
        return 1;
    }

    auto transitions = buildTransitions(mood_scales[mood]);
    auto melody = generateMelody(mood, 16, transitions);
    playMelody(melody, mood);

    return 0;
}
