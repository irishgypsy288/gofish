# Go Fish Game - Quick Start Guide

## Installation (One-Time Setup)

### Step 1: Install Dependencies
```bash
sudo apt-get update
sudo apt-get install build-essential libx11-dev alsa-utils
```

### Step 2: Navigate to Project
```bash
cd gofish
```

### Step 3: Build the Game
```bash
make
```

## Running the Game

### Start the Game
```bash
./gofish-gui
```

Or use:
```bash
make run
```

## How to Play

### Welcome Screen
1. You'll see the welcome screen with "GO FISH GAME" title
2. Click **"NEW GAME"** button or press **Enter** to start
3. Click **"EXIT"** button or press **Escape** to quit

### During Gameplay
- Watch AI1 and AI2 play automatically
- AI2's cards are shown face down at the top
- AI1's cards are shown face up at the bottom
- Game log in the center shows all actions
- Books count is displayed for each player
- The game progresses automatically

### End Game Screen
1. When the game ends, you'll see the winner
2. Click **"NEW GAME"** to play again
3. Click **"EXIT"** to quit

## Keyboard Shortcuts

- **Enter/Space**: Start new game (on welcome/end screens)
- **Escape**: Exit the game

## Troubleshooting

### "Cannot open X display"
Make sure you're running in a graphical environment:
```bash
echo $DISPLAY
# Should show something like :0
```

### Build Errors
Clean and rebuild:
```bash
make clean
make
```

### No Sound
Audio is optional. The game works without sound. To enable audio:
```bash
# Check if aplay is installed
which aplay

# Install if missing
sudo apt-get install alsa-utils
```

## Additional Commands

```bash
# Build console version (original)
make console
./gofish-console

# Build with debug symbols
make debug

# Clean build files
make clean

# Show all available commands
make help

# Check dependencies
make check-deps
```

## Need More Help?

- See **README_GUI.md** for detailed documentation
- See **MASTER_PLAN.md** for technical specifications
- See **PROJECT_SUMMARY.md** for project overview

---

**Enjoy the game!** 🎴🐟