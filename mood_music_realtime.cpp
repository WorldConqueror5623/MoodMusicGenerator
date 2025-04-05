#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include "rtmidi/RtMidi.h" // Include the RtMidi library for MIDI output

using namespace std;

// 🎵 Markov Chain for Melody Generation
map<int, vector<int>> mood_transitions = {
    {60, {62, 64, 67}},  // C → D, E, G
    {62, {64, 60, 65}},  // D → E, C, F
    {64, {65, 67, 60}},  // E → F, G, C
    {65, {67, 69, 60}},  // F → G, A, C
    {67, {69, 71, 60}},  // G → A, B, C
    {69, {71, 72, 60}},  // A → B, C, C
    {71, {72, 74, 60}}   // B → C, D, C
};

// 🎼 Scales for Different Moods
map<string, vector<int>> mood_scales = {
    {"joyful", {60, 62, 64, 65, 67, 69, 71, 72}},  // C Major Scale
    {"melancholy", {60, 62, 63, 65, 67, 68, 70, 72}},  // C Minor Scale
    {"powerful", {60, 62, 65, 67, 70, 72}},  // C Pentatonic Scale
    {"calm", {60, 62, 64, 65, 67, 69, 71}},  // C Major (soft dynamics)
    {"tense", {60, 61, 64, 65, 68, 69, 72}},  // Dissonant Mode
    {"dreamy", {60, 62, 64, 66, 69, 71}}      // Whole Tone Scale
};

// 🎚️ Tempo Mapping
map<string, int> mood_tempo = {
    {"joyful", 120},
    {"melancholy", 60},
    {"powerful", 150},
    {"calm", 80},
    {"tense", 100},
    {"dreamy", 90}
};

// 🎹 Instrument Mapping
map<string, int> mood_instruments = {
    {"joyful", 0},        // Acoustic Grand Piano
    {"melancholy", 40},   // Violin
    {"powerful", 81},     // Synth Lead
    {"calm", 14},         // Tubular Bells
    {"tense", 48},        // Strings Ensemble
    {"dreamy", 89}        // New Age Pad
};

// 🎶 Function to Generate Melody
vector<int> generateMelody(string mood, int length) {
    vector<int> melody;
    vector<int> scale = mood_scales[mood];

    // Start with a random note from the scale
    int note = scale[rand() % scale.size()];
    for (int i = 0; i < length; i++) {
        melody.push_back(note);
        if (mood_transitions.count(note)) {
            note = mood_transitions[note][rand() % mood_transitions[note].size()];
        } else {
            note = scale[rand() % scale.size()];
        }
    }
    return melody;
}

// 🎛️ Play Melody using RtMidi
void playMelody(vector<int> melody, string mood) {
    RtMidiOut midiOut;
    if (midiOut.getPortCount() == 0) {
        cout << "No MIDI output ports found!" << endl;
        return;
    }
    midiOut.openPort(0);

    vector<unsigned char> message;

    // 🎼 Set Instrument
    message.push_back(0xC0);
    message.push_back(mood_instruments[mood]);
    midiOut.sendMessage(&message);

    // 🎹 Play Notes
    int tempo = mood_tempo[mood];
    for (int note : melody) {
        message.clear();
        message.push_back(0x90);
        message.push_back(note);
        message.push_back(100); // Velocity
        midiOut.sendMessage(&message);

        this_thread::sleep_for(chrono::milliseconds(60000 / tempo));

        // Release Note
        message.clear();
        message.push_back(0x80);
        message.push_back(note);
        message.push_back(0);
        midiOut.sendMessage(&message);
    }
    midiOut.closePort();
}

// 🎤 Main Function
int main() {
    srand(time(0));

    string mood;
    cout << "Enter mood (joyful, melancholy, powerful, calm, tense, dreamy): ";
    cin >> mood;

    if (mood_scales.find(mood) == mood_scales.end()) {
        cout << "Invalid mood! Choose from: joyful, melancholy, powerful, calm, tense, dreamy." << endl;
        return 1;
    }

    vector<int> melody = generateMelody(mood, 16);
    playMelody(melody, mood);

    return 0;
}
