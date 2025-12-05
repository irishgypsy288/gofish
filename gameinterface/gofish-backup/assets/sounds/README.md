# Audio Assets

This directory contains sound files for the Go Fish game.

## Required Sound Files

1. **victory.wav** - Played when the game ends
   - Duration: 2-3 seconds
   - Format: WAV, 44.1kHz, 16-bit
   - A celebratory, upbeat sound

2. **card_move.wav** - Played when cards are transferred between players
   - Duration: 0.3 seconds
   - Format: WAV, 44.1kHz, 16-bit
   - A soft swoosh or shuffle sound

## How to Add Sounds

### Option 1: Use Free Sound Libraries
Download free sound effects from:
- https://freesound.org/
- https://www.zapsplat.com/
- https://mixkit.co/free-sound-effects/

### Option 2: Generate Simple Sounds
You can generate simple beep sounds using the `sox` tool:

```bash
# Install sox
sudo apt-get install sox

# Generate victory sound (ascending notes)
sox -n victory.wav synth 0.3 sine 440 0.3 sine 554 0.3 sine 659 0.3 sine 880

# Generate card move sound (short swoosh)
sox -n card_move.wav synth 0.2 whitenoise lowpass 2000 fade 0 0.2 0.1
```

### Option 3: Create Silent Placeholders
If you don't want sound, create silent WAV files:

```bash
# Create 1-second silent files
sox -n -r 44100 -c 2 victory.wav trim 0.0 1.0
sox -n -r 44100 -c 2 card_move.wav trim 0.0 0.3
```

## Current Status

The game will work without sound files, but audio features will be disabled.
Add the WAV files to this directory to enable audio.