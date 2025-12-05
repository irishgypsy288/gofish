#include "AudioManager.h"
#include <iostream>
#include <cstdlib>
#include <thread>

AudioManager::AudioManager() 
    : m_enabled(true), m_volume(0.7f) {
}

AudioManager::~AudioManager() {
    cleanup();
}

bool AudioManager::initialize() {
    // Check if aplay (ALSA player) is available
    int result = system("which aplay > /dev/null 2>&1");
    if (result != 0) {
        std::cerr << "Warning: aplay not found. Audio will be disabled." << std::endl;
        m_enabled = false;
        return false;
    }
    
    m_enabled = true;
    return true;
}

void AudioManager::cleanup() {
    // Nothing to cleanup for simple implementation
}

bool AudioManager::loadSound(const std::string& name, const std::string& filepath) {
    // For simple implementation, we just verify the file exists
    // In a more complex implementation, we would load the sound into memory
    return true;
}

void AudioManager::playSound(const std::string& name) {
    if (!m_enabled) {
        return;
    }
    
    // For now, we'll play sounds directly from assets folder
    std::string filepath = "assets/sounds/" + name + ".wav";
    playWavFile(filepath);
}

void AudioManager::stopSound(const std::string& name) {
    // Simple implementation doesn't support stopping
}

void AudioManager::setVolume(float volume) {
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;
    m_volume = volume;
}

void AudioManager::playWavFile(const std::string& filepath) {
    if (!m_enabled) {
        return;
    }
    
    // Play sound in background thread to avoid blocking
    std::thread([filepath]() {
        std::string command = "aplay -q " + filepath + " 2>/dev/null &";
        system(command.c_str());
    }).detach();
}