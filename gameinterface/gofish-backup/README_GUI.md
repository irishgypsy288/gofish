# Go Fish Game - Graphical User Interface

A graphical implementation of the classic Go Fish card game featuring two AI players competing against each other, built with X11/Xlib for Linux/Ubuntu systems.

## Features

- **Graphical User Interface**: Full X11-based GUI with smooth animations
- **Welcome Screen**: Clean interface with "New Game" and "Exit" buttons
- **Dynamic Gameplay**: Watch AI1 and AI2 play in real-time
- **Card Animations**: Smooth card movements and transitions
- **Game Log**: Real-time display of game actions and decisions
- **Audio Feedback**: Celebratory sounds when the game ends
- **End Game Screen**: Shows winner and offers replay options

## Screenshots

```
Welcome Screen:
┌─────────────────────────────────────┐
│         GO FISH GAME                │
│         AI vs AI                    │
│      ┌─────────────────┐            │
│      │   NEW GAME      │            │
│      └─────────────────┘            │
│      ┌─────────────────┐            │
│      │     EXIT        │            │
│      └─────────────────┘            │
└─────────────────────────────────────┘

Gameplay Screen:
- AI2's hand displayed at top (cards face down)
- Game log in the center showing actions
- Draw pile in the middle
- AI1's hand at bottom (cards face up)
- Books count for each player
```

## Requirements

### System Requirements
- Ubuntu/Debian Linux (or any Linux with X Window System)
- X11 display server
- C++ compiler with C++11 support

### Dependencies
```bash
# Install required packages
sudo apt-get update
sudo apt-get install build-essential libx11-dev alsa-utils
```

**Required packages:**
- `build-essential` - C++ compiler and build tools
- `libx11-dev` - X11 development libraries
- `alsa-utils` - Audio playback utilities (optional, for sound effects)

## Building the Game

### Quick Start
```bash
# Clone the repository
git clone https://github.com/irishgypsy288/gofish.git
cd gofish

# Check dependencies
make check-deps

# Build the graphical version
make

# Run the game
./gofish-gui
```

### Build Options

```bash
# Build graphical version (default)
make

# Build console version (original)
make console

# Build with debug symbols
make debug

# Build and run immediately
make run

# Build and run console version
make run-console

# Clean build artifacts
make clean

# Show all available commands
make help
```

## Installation

### System-wide Installation
```bash
# Install to /usr/local/bin
sudo make install

# Now you can run from anywhere
gofish-gui

# Uninstall
sudo make uninstall
```

### Local Installation
Simply run the executable from the project directory:
```bash
./gofish-gui
```

## Usage

### Starting the Game
1. Launch the application: `./gofish-gui`
2. Click "NEW GAME" button or press Enter
3. Watch the AI players compete
4. When the game ends, choose "NEW GAME" to play again or "EXIT" to quit

### Keyboard Shortcuts
- **Enter/Space**: Start new game (on welcome/end screens)
- **Escape**: Exit the game

### Mouse Controls
- Click buttons to interact with the UI
- Buttons highlight when hovered

## Game Rules

The game follows standard Go Fish rules:

1. **Setup**: Each player starts with 7 cards from a standard 52-card deck
2. **Objective**: Form the most "books" (sets of 4 cards of the same rank)
3. **Gameplay**:
   - Players take turns asking opponents for specific ranks
   - If opponent has the rank, they give all matching cards
   - If not, the asking player draws from the deck ("Go Fish!")
   - When a player collects 4 cards of the same rank, they form a book
4. **Winning**: The player with the most books when the game ends wins

## Audio Setup

The game supports audio feedback for enhanced experience.

### Adding Sound Files

1. Navigate to the assets directory:
```bash
cd assets/sounds
```

2. Add two WAV files:
   - `victory.wav` - Played when game ends (2-3 seconds)
   - `card_move.wav` - Played when cards transfer (0.3 seconds)

### Generating Simple Sounds

If you have `sox` installed:
```bash
# Install sox
sudo apt-get install sox

# Generate victory sound
sox -n assets/sounds/victory.wav synth 0.3 sine 440 0.3 sine 554 0.3 sine 659 0.3 sine 880

# Generate card move sound
sox -n assets/sounds/card_move.wav synth 0.2 whitenoise lowpass 2000 fade 0 0.2 0.1
```

### Disabling Audio

The game works perfectly without sound files. Audio features are automatically disabled if:
- Sound files are not found
- `aplay` is not available on the system

## Project Structure

```
gofish/
├── src/
│   ├── main.cpp           # Application entry point
│   ├── GameEngine.cpp/h   # Core game logic
│   ├── UIManager.cpp/h    # X11 UI management
│   └── AudioManager.cpp/h # Sound playback
├── assets/
│   └── sounds/            # Audio files
├── build/                 # Build artifacts (generated)
├── gofish.cpp             # Original console version
├── Makefile               # Build configuration
├── MASTER_PLAN.md         # Project specifications
└── README_GUI.md          # This file
```

## Troubleshooting

### "Cannot open X display" Error
**Problem**: X11 display not available
**Solution**: 
- Ensure you're running in a graphical environment
- Check `echo $DISPLAY` returns a value (usually `:0`)
- Try: `export DISPLAY=:0`

### "X11 development libraries not found"
**Problem**: Missing libx11-dev package
**Solution**: 
```bash
sudo apt-get install libx11-dev
```

### No Sound
**Problem**: Audio not playing
**Solution**:
- Check if `aplay` is installed: `which aplay`
- Install ALSA utilities: `sudo apt-get install alsa-utils`
- Verify sound files exist in `assets/sounds/`
- Test audio: `aplay assets/sounds/victory.wav`

### Build Errors
**Problem**: Compilation fails
**Solution**:
- Ensure g++ is installed: `g++ --version`
- Check C++11 support: `g++ -std=c++11 --version`
- Install build essentials: `sudo apt-get install build-essential`
- Run: `make clean && make`

### Window Not Appearing
**Problem**: Game starts but no window shows
**Solution**:
- Check if X server is running: `ps aux | grep X`
- Verify display: `xdpyinfo | grep "name of display"`
- Try running with: `DISPLAY=:0 ./gofish-gui`

## Development

### Building from Source

1. Clone the repository
2. Install dependencies
3. Build with make
4. Run tests (if available)

### Debug Build

For development and debugging:
```bash
make debug
gdb ./gofish-gui
```

### Code Structure

- **GameEngine**: Manages game state and rules (independent of UI)
- **UIManager**: Handles X11 window, rendering, and user input
- **AudioManager**: Manages sound playback
- **main.cpp**: Coordinates components and runs game loop

## Contributing

Contributions are welcome! Areas for improvement:
- Enhanced animations
- Better card graphics
- Additional sound effects
- Human vs AI mode
- Network multiplayer
- Save/load game state

## License

This project is open source. See the repository for license details.

## Credits

- Original console version: [irishgypsy288](https://github.com/irishgypsy288)
- GUI implementation: Built with X11/Xlib
- Game design: Classic Go Fish card game

## Version History

- **v2.0** - Graphical UI with X11, animations, and audio
- **v1.0** - Console-based AI vs AI implementation

## Support

For issues, questions, or suggestions:
- Open an issue on GitHub
- Check the troubleshooting section above
- Review the MASTER_PLAN.md for technical details

---

**Enjoy the game!** 🎴🐟