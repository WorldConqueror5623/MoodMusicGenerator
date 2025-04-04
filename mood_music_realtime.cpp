#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include "rtmidi/RtMidi.h"

using namespace std;

// 🎼 Scales for Different Moods
map<string, vector<int>> mood_scales = {
    {"happy", {60, 62, 64, 65, 67, 69, 71, 72}},  // C Major Scale
    {"sad", {60, 62, 63, 65, 67, 68, 70, 72}},    // C Minor Scale
    {"energetic", {60, 62, 65, 67, 70, 72}}       // C Pentatonic Scale
};

// 🎵 Markov Chain for Melody Generation
map<int, vector<int>> mood_transitions = {
    {60, {62, 64, 67}},  // C → D, E, G
    {62, {64, 65, 67}},  // D → E, F, G
    {63, {65, 67, 60}},  // Eb → F, G, C
    {64, {65, 67, 69}},  // E → F, G, A
    {65, {67, 69, 71}},  // F → G, A, B
    {67, {69, 71, 72}},  // G → A, B, C
    {68, {70, 72, 60}},  // Ab → Bb, C, C
    {69, {71, 72, 60}},  // A → B, C, C
    {70, {72, 74, 60}},  // Bb → C, D, C
    {71, {72, 74, 60}},  // B → C, D, C
    {72, {60, 62, 64}}   // C octave → C, D, E
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

// 🎶 Function to Generate Melody
vector<int> generateMelody(string mood, int length) {
    vector<int> melody;
    vector<int> scale = mood_scales[mood];

    // Start with a random note from the scale
    int note = scale[rand() % scale.size()];
    for (int i = 0; i < length; i++) {
        melody.push_back(note);

        // Transition using Markov map if available, else random from scale
        if (mood_transitions.find(note) != mood_transitions.end()) {
            vector<int> next_notes = mood_transitions[note];
            note = next_notes[rand() % next_notes.size()];
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
    message.push_back(0xC0); // Program change
    message.push_back(mood_instruments[mood]);
    midiOut.sendMessage(&message);

    // 🎹 Play Notes
    int tempo = mood_tempo[mood];
    for (int note : melody) {
        message.clear();
        message.push_back(0x90); // Note on
        message.push_back(note);
        message.push_back(100); // Velocity
        midiOut.sendMessage(&message);

        this_thread::sleep_for(chrono::milliseconds(60000 / tempo));

        // Release Note
        message.clear();
        message.push_back(0x80); // Note off
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
    cout << "Enter mood (happy, sad, energetic): ";
    cin >> mood;

    if (mood_scales.find(mood) == mood_scales.end()) {
        cout << "Invalid mood! Choose happy, sad, or energetic." << endl;
        return 1;
    }

    vector<int> melody = generateMelody(mood, 16);

    cout << "🎶 Playing melody: ";
    for (int note : melody) cout << note << " ";
    cout << endl;

    playMelody(melody, mood);

    return 0;
}
// 🎵 Enjoy your music! 🎶