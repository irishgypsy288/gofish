#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <string>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();
    
    bool initialize();
    void cleanup();
    
    bool loadSound(const std::string& name, const std::string& filepath);
    void playSound(const std::string& name);
    void stopSound(const std::string& name);
    
    void setVolume(float volume); // 0.0 to 1.0
    float getVolume() const { return m_volume; }
    
    bool isEnabled() const { return m_enabled; }
    void setEnabled(bool enabled) { m_enabled = enabled; }
    
private:
    bool m_enabled;
    float m_volume;
    
    // Simple implementation - will use system commands for audio playback
    void playWavFile(const std::string& filepath);
};

#endif // AUDIOMANAGER_H