#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <random>
#include <thread>
#include <chrono>
#include "rtmidi/RtMidi.h"

using namespace std;

// 🎼 Mood Scales
map<string, vector<int>> mood_scales = {
    {"happy", {60, 62, 64, 65, 67, 69, 71, 72}},  // C Major
    {"sad", {60, 62, 63, 65, 67, 68, 70, 72}},    // C Minor
    {"energetic", {60, 62, 65, 67, 70, 72}}       // C Pentatonic
};

// 🎚️ Tempo Mapping
map<string, int> mood_tempo = {
    {"happy", 120},
    {"sad", 60},
    {"energetic", 150}
};

// 🎹 Instrument Mapping
map<string, int> mood_instruments = {
    {"happy", 0},      // Piano
    {"sad", 40},       // Violin
    {"energetic", 81}  // Synth Lead
};

// 🎯 Weighted Transitions for Markov Chain
map<int, vector<pair<int, double>>> weighted_transitions = {
    {60, {{62, 0.5}, {64, 0.3}, {67, 0.2}}},
    {62, {{64, 0.4}, {60, 0.4}, {65, 0.2}}},
    {64, {{65, 0.4}, {67, 0.4}, {60, 0.2}}},
    {65, {{67, 0.4}, {69, 0.4}, {60, 0.2}}},
    {67, {{69, 0.4}, {71, 0.4}, {60, 0.2}}},
    {69, {{71, 0.4}, {72, 0.4}, {60, 0.2}}},
    {71, {{72, 0.4}, {74, 0.4}, {60, 0.2}}},
    {72, {{74, 0.4}, {76, 0.4}, {60, 0.2}}}
};

// 🎲 Weighted Note Selector
int getWeightedNextNote(int current) {
    static random_device rd;
    static mt19937 gen(rd());

    vector<pair<int, double>> options = weighted_transitions[current];
    vector<int> notes;
    vector<double> weights;
    for (auto& p : options) {
        notes.push_back(p.first);
        weights.push_back(p.second);
    }

    discrete_distribution<> dist(weights.begin(), weights.end());
    return notes[dist(gen)];
}

// 🎶 Melody Generator with Rhythm
vector<pair<int, int>> generateMelody(string mood, int length) {
    vector<pair<int, int>> melody;
    vector<int> scale = mood_scales[mood];
    int note = scale[rand() % scale.size()];

    for (int i = 0; i < length; i++) {
        int duration = (rand() % 2 == 0) ? 1 : 2; // quarter or eighth
        melody.push_back({note, duration});
        note = getWeightedNextNote(note);
    }
    return melody;
}

// 🎵 Harmony (basic third)
vector<int> getHarmony(int note, string mood) {
    if (mood == "happy") return {note, note + 4}; // major third
    if (mood == "sad") return {note, note + 3};   // minor third
    return {note}; // energetic = solo
}

// 🎧 Playback
void playMelody(vector<pair<int, int>> melody, string mood) {
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
    for (auto [note, duration] : melody) {
        vector<int> chord = getHarmony(note, mood);
        int velocity = 80 + rand() % 30;
        int swing = (rand() % 11) - 5;

        // Note ON
        for (int n : chord) {
            message.clear();
            message.push_back(0x90);
            message.push_back(n);
            message.push_back(velocity);
            midiOut.sendMessage(&message);
        }

        this_thread::sleep_for(chrono::milliseconds((60000 / tempo / duration) + swing));

        // Note OFF
        for (int n : chord) {
            message.clear();
            message.push_back(0x80);
            message.push_back(n);
            message.push_back(0);
            midiOut.sendMessage(&message);
        }
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
        cout << "Invalid mood! Choose happy, sad, or energetic." << endl;
        return 1;
    }

    auto melody = generateMelody(mood, 16);
    playMelody(melody, mood);
    return 0;
}
