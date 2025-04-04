# 🎵 AI Mood Music Generator

A C++ program that creates original melodies based on user mood — **happy**, **sad**, or **energetic** — using procedural generation and real-time MIDI playback via **RtMidi**.

---

## 📌 Features

- 🎹 Generates melodies using a **Markov Chain** and **mood-based scales**
- 🎶 Real-time audio playback using **RtMidi**
- 🎼 Different instruments and tempos for each mood
- 💡 Great for indie game developers, musicians, or AI/ML music enthusiasts

---

## 🚀 How It Works

1. You enter your mood.
2. It selects a matching **scale**, **instrument**, and **tempo**.
3. A melody is generated procedurally.
4. The program plays it back live via MIDI.

---

## 🛠️ How to Build in VS Code


### 🧰 Requirements

- [MinGW-w64](https://www.mingw-w64.org/) (or g++ compiler)
- RtMidi (included in `rtmidi/`)
- VS Code with C++ extension

---

### ▶️ Build Instructions

1. Open folder in **VS Code**
2. Open terminal and type:  

   ```bash
   g++ mood_music_realtime.cpp rtmidi/RtMidi.cpp -o moodplayer.exe -D__WINDOWS_MM__ -lwinmm

OR press Ctrl + Shift + B (uses .vscode/tasks.json)
