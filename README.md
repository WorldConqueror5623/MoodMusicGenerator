# 🎵 AI Music Generator

A real-time mood-based music generator using MIDI and AI-inspired logic (Markov chains, mood-aware dynamics, and instrument selection).

---

## ✨ Features

- Mood-driven melody generation  
- AI-powered transitions using Markov chains  
- Real-time MIDI playback  
- Different instruments, tempo, and scales based on selected mood  

---

## 🎧 Supported Moods

- `joyful` – Bright and upbeat (C Major, Piano)  
- `melancholy` – Sad and soft (C Minor, Violin)  
- `powerful` – Energetic and bold (Pentatonic, Synth Lead)  
- `calm` – Peaceful tones (Major, Bells)  
- `tense` – Dissonant and suspenseful (Modal scale, Strings)  
- `dreamy` – Ethereal vibe (Whole Tone, Pad)  

---

## 🏗️ Building and Running

### 🪟 Building on Windows

1. **Install MinGW**:  
   Download from [https://www.mingw-w64.org/](https://www.mingw-w64.org/)

2. **Install RtMidi**:  
   Clone or download the RtMidi library and place it in the project folder.

3. **Build using VS Code Task**:  
   Press `Ctrl+Shift+B` and select:  
   `Build Mood Music`

4. **Or build manually using terminal**:  
   ```bash
   g++ mood_music_realtime.cpp rtmidi/RtMidi.cpp -o moodplayer.exe -D__WINDOWS_MM__ -lwinmm
   ```

---

### 🍎 Building on macOS

1. **Install dependencies**:  
   ```bash
   brew install pkg-config portmidi
   ```

2. **Clone the repository**:  
   ```bash
   git clone https://github.com/WorldConqueror5623/MoodMusicGenerator  
   cd ai-music-generator
   ```

3. **Build using VS Code Task**:  
   Press `Cmd+Shift+B` and select:  
   `Build Mood Music (macOS)`

4. **Or compile manually via terminal**:  
   ```bash
   g++ mood_music_realtime.cpp rtmidi/RtMidi.cpp -o moodplayer -framework CoreMIDI -framework CoreAudio -framework AudioToolbox
   ```

5. **Run the program**:  
   ```bash
   ./moodplayer
   ```

---