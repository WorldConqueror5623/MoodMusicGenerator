#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <fstream>
#include "rtmidi/RtMidi.h"
#include "midifile/include/MidiFile.h"

using namespace std;
using namespace smf; // for midifile

// 🎵 Markov Chain for Melody Generation
map<int, vector<int>> mood_transitions = {
    {60, {62, 64, 67}}, {62, {64, 60, 65}}, {64, {65, 67, 60}},
    {65, {67, 69, 60}}, {67, {69, 71, 60}}, {69, {71, 72, 60}},
    {71, {72, 74, 60}}
};

// 🎼 Scales for Different Moods
map<string, vector<int>> mood_scales = {
    {"joyful", {60, 62, 64, 65, 67, 69, 71, 72}},
    {"melancholy", {60, 62, 63, 65, 67, 68, 70, 72}},
    {"powerful", {60, 62, 65, 67, 70, 72}},
    {"calm", {60, 62, 64, 65, 67, 69, 71}},
    {"tense", {60, 61, 64, 65, 68, 69, 72}},
    {"dreamy", {60, 62, 64, 66, 69, 71}}
};

// 🥁 Rhythm Patterns (durations in milliseconds)
vector<int> rhythm_pattern = {400, 200, 600, 300, 500};

// 🎚️ Tempo Mapping
map<string, int> mood_tempo = {
    {"joyful", 120}, {"melancholy", 60}, {"powerful", 150},
    {"calm", 80}, {"tense", 100}, {"dreamy", 90}
};

// 🎹 Instrument Mapping
map<string, int> mood_instruments = {
    {"joyful", 0}, {"melancholy", 40}, {"powerful", 81},
    {"calm", 14}, {"tense", 48}, {"dreamy", 89}
};

// 🎶 Generate Melody
vector<int> generateMelody(string mood, int length) {
    vector<int> melody;
    vector<int> scale = mood_scales[mood];
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

// 💾 Export as Text
void exportToMidiText(const vector<int>& melody, const string& filename) {
    ofstream out(filename);
    for (int note : melody) {
        out << "Note: " << note << "\n";
    }
    out.close();
    cout << "Melody exported to " << filename << endl;
}

// 🎼 Export to MIDI using Midifile
void saveAsMIDI(const vector<int>& melody, const string& filename, string mood) {
    MidiFile midi;
    midi.addTrack(1);
    int tpq = 480;
    midi.setTicksPerQuarterNote(tpq);

    int track = 0;
    int instrument = mood_instruments[mood];
    int tempo = mood_tempo[mood];
    int tick = 0;

    // Set tempo
    midi.addTempo(track, tick, tempo);

    // Set instrument
    midi.addTimbre(track, tick, 0, instrument);

    // Add notes
    for (size_t i = 0; i < melody.size(); i++) {
        int note = melody[i];
        int duration_ms = rhythm_pattern[i % rhythm_pattern.size()];
        int duration_ticks = (duration_ms * tpq) / (60000 / tempo);

        midi.addNoteOn(track, tick, 0, note, 100);
        midi.addNoteOff(track, tick + duration_ticks, 0, note);
        tick += duration_ticks;
    }

    midi.sortTracks();
    midi.write(filename);
    cout << "MIDI file saved as " << filename << endl;
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
    message.push_back(0xC0);
    message.push_back(mood_instruments[mood]);
    midiOut.sendMessage(&message);

    int tempo = mood_tempo[mood];
    for (size_t i = 0; i < melody.size(); i++) {
        int note = melody[i];
        int velocity = 100;

        message = {0x90, static_cast<unsigned char>(note), static_cast<unsigned char>(velocity)};
        midiOut.sendMessage(&message);

        this_thread::sleep_for(chrono::milliseconds(rhythm_pattern[i % rhythm_pattern.size()]));

        message = {0x80, static_cast<unsigned char>(note), 0};
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
    exportToMidiText(melody, "output_melody.txt");
    saveAsMIDI(melody, "output.mid", mood);

    return 0;
}
