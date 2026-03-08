# Simple-JUCE-AudioPlayer
🎵 Simple JUCE Audio Player
A lightweight, high-performance standalone audio application built with C++ and the JUCE Framework. This tool was developed as a utility to test VST3 plugins and practice robust multi-threaded DSP implementation.

✨ Features
Multi-Format Support: Native playback for .wav, .mp3, and .aiff files using juce::AudioFormatManager.

Thread-Safe Transport: Robust Play/Stop functionality with state-dependent button locking to prevent audio thread crashes.

Interactive UI:

Smart Position Slider: Real-time song progress tracking with manual seeking support.

Linear Gain Control: Smooth volume adjustment via a dedicated slider.

Dynamic Metadata: Displays the currently loaded filename directly in the interface.

High-Tick UI Sync: Utilizes juce::Timer to synchronize the GUI with the audio thread at precise 100ms intervals.

🛠️ Technical Stack
Language: C++17

Framework: JUCE 8 (using AudioAppComponent)

Build Tool: Projucer

IDE: Visual Studio 2026

📥 Requirements & Setup
1. Pre-requisites
Before building, you must have the following installed:

Visual Studio 2026: Ensure the "Desktop development with C++" workload is selected.

JUCE 8: Download the latest version from juce.com.

2. Building from Source
Clone the repository:

Bash
git clone https://github.com/zNymu/Simple-JUCE-AudioPlayer.git
Open SimpleAudioPlayer.jucer in the Projucer.

Ensure the Global Paths in Projucer point to your local JUCE modules folder.

Click "Save and Open in IDE" to generate the Visual Studio solution.

In Visual Studio, set the build configuration to Release / x64 and press F5.

🚀 How to Use
Launch the application.

Click Open File to browse your computer for an audio track.

Use the Position slider to jump to any part of the song.

Adjust the Volume slider to your preferred level.
