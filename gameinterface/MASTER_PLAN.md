# Go Fish Game UI - Master Plan & Procedural Standards

## Project Overview
This project transforms the existing console-based Go Fish C++ game into a fully graphical X11 application for Ubuntu/Linux systems. The UI will provide an engaging visual experience with animated card movements, dynamic displays, and audio feedback.

---

## 1. PROJECT OBJECTIVES

### Primary Goals
1. Create an intuitive graphical user interface using X11/Xlib
2. Display AI1 and AI2 players with their hands and books
3. Animate card movements throughout gameplay
4. Provide audio feedback for game events
5. Implement a complete game flow (Welcome → Gameplay → End Screen)

### User Experience Requirements
- **Welcome Screen**: Clear "New Game" button to start
- **Gameplay Screen**: Real-time visualization of both AI players
- **Card Animations**: Smooth transitions when cards move between players
- **Audio Feedback**: Celebratory sound on game completion
- **End Screen**: Options for "New Game" or "Exit"

---

## 2. TECHNICAL ARCHITECTURE

### 2.1 Technology Stack
- **Language**: C++ (C++11 or later)
- **Graphics**: X11/Xlib for window management and rendering
- **Audio**: ALSA (Advanced Linux Sound Architecture) or simple WAV playback
- **Build System**: GNU Make
- **Platform**: Ubuntu/Linux with X Window System

### 2.2 Project Structure
```
gofish/
├── src/
│   ├── main.cpp              # Entry point with X11 event loop
│   ├── GameEngine.cpp        # Core game logic (refactored from gofish.cpp)
│   ├── GameEngine.h          # Game engine header
│   ├── UIManager.cpp         # X11 UI management
│   ├── UIManager.h           # UI manager header
│   ├── CardRenderer.cpp      # Card drawing and animation
│   ├── CardRenderer.h        # Card renderer header
│   ├── AudioManager.cpp      # Sound playback
│   └── AudioManager.h        # Audio manager header
├── assets/
│   ├── sounds/
│   │   ├── victory.wav       # Celebratory sound
│   │   └── card_move.wav     # Card movement sound
│   └── fonts/
│       └── (system fonts)
├── gofish.cpp                # Original console version (preserved)
├── Makefile                  # Build configuration
├── MASTER_PLAN.md            # This document
└── README.md                 # Updated documentation

```

### 2.3 Component Responsibilities

#### GameEngine
- Manages game state (hands, books, draw pile, turn)
- Implements game rules and logic
- Provides step-by-step game progression
- Emits events for UI updates
- Maintains game history for animations

#### UIManager
- Creates and manages X11 window
- Handles user input (mouse clicks, keyboard)
- Manages UI states (Welcome, Gameplay, EndGame)
- Coordinates rendering and animations
- Displays buttons and interactive elements

#### CardRenderer
- Renders individual cards with rank and suit
- Implements card animation system
- Manages card positions and transitions
- Handles card highlighting and selection effects

#### AudioManager
- Loads and plays sound effects
- Manages audio resources
- Provides simple playback interface

---

## 3. CODING STANDARDS & CONVENTIONS

### 3.1 Code Style
- **Naming Conventions**:
  - Classes: PascalCase (e.g., `GameEngine`, `UIManager`)
  - Functions: camelCase (e.g., `initializeGame`, `renderCard`)
  - Variables: camelCase (e.g., `currentPlayer`, `cardPosition`)
  - Constants: UPPER_SNAKE_CASE (e.g., `WINDOW_WIDTH`, `CARD_HEIGHT`)
  - Private members: prefix with `m_` (e.g., `m_gameState`, `m_window`)

- **File Organization**:
  - Header files (.h) contain class declarations and inline functions
  - Source files (.cpp) contain implementations
  - One class per file pair (header + source)
  - Include guards in all headers: `#ifndef CLASSNAME_H` / `#define CLASSNAME_H`

- **Comments**:
  - Use `//` for single-line comments
  - Use `/* */` for multi-line comments
  - Document all public methods with brief descriptions
  - Explain complex algorithms and non-obvious code

### 3.2 Memory Management
- Use RAII (Resource Acquisition Is Initialization) principles
- Prefer smart pointers (`std::unique_ptr`, `std::shared_ptr`) when appropriate
- Properly clean up X11 resources (windows, graphics contexts, pixmaps)
- Avoid memory leaks by ensuring all `new` has corresponding `delete`

### 3.3 Error Handling
- Check return values from X11 functions
- Use exceptions for exceptional conditions
- Log errors to stderr with descriptive messages
- Gracefully handle missing resources (fonts, sounds)

---

## 4. UI/UX SPECIFICATIONS

### 4.1 Window Configuration
- **Window Size**: 1024x768 pixels (resizable)
- **Title**: "Go Fish - AI vs AI"
- **Background**: Dark green (card table color: #0B5D1E)
- **Frame Rate**: 60 FPS for smooth animations

### 4.2 Welcome Screen Layout
```
┌─────────────────────────────────────┐
│                                     │
│         GO FISH GAME                │
│         AI vs AI                    │
│                                     │
│      ┌─────────────────┐            │
│      │   NEW GAME      │            │
│      └─────────────────┘            │
│                                     │
│      ┌─────────────────┐            │
│      │     EXIT        │            │
│      └─────────────────┘            │
│                                     │
└─────────────────────────────────────┘
```

### 4.3 Gameplay Screen Layout
```
┌─────────────────────────────────────────────────┐
│  AI2 (Opponent)                    Books: 3     │
│  ┌──┐ ┌──┐ ┌──┐ ┌──┐ ┌──┐                      │
│  │  │ │  │ │  │ │  │ │  │  (5 cards)           │
│  └──┘ └──┘ └──┘ └──┘ └──┘                      │
│                                                 │
│  ┌────────────────────────────┐                │
│  │  Game Log / Messages       │                │
│  │  AI1: AI2, have any Aces?  │                │
│  │  AI2: Go Fish!             │                │
│  │  AI1 draws: 5 of Hearts    │                │
│  └────────────────────────────┘                │
│                                                 │
│  Draw Pile: 24 cards                           │
│  ┌──┐                                          │
│  │??│                                          │
│  └──┘                                          │
│                                                 │
│  AI1 (Current Player)              Books: 2    │
│  ┌──┐ ┌──┐ ┌──┐ ┌──┐ ┌──┐ ┌──┐ ┌──┐          │
│  │A♠│ │3♥│ │7♦│ │7♣│ │9♠│ │Q♥│ │K♦│          │
│  └──┘ └──┘ └──┘ └──┘ └──┘ └──┘ └──┘          │
└─────────────────────────────────────────────────┘
```

### 4.4 End Game Screen Layout
```
┌─────────────────────────────────────┐
│                                     │
│         GAME OVER!                  │
│                                     │
│         Winner: AI1                 │
│         Books: 7                    │
│                                     │
│      ┌─────────────────┐            │
│      │   NEW GAME      │            │
│      └─────────────────┘            │
│                                     │
│      ┌─────────────────┐            │
│      │     EXIT        │            │
│      └─────────────────┘            │
│                                     │
└─────────────────────────────────────┘
```

### 4.5 Card Design
- **Card Size**: 80x120 pixels
- **Card Back**: Blue pattern with "?" symbol
- **Card Face**: White background with:
  - Rank in top-left and bottom-right corners
  - Large suit symbol in center
  - Red for Hearts/Diamonds, Black for Clubs/Spades

### 4.6 Animation Specifications
- **Card Movement**: 
  - Duration: 500ms
  - Easing: Ease-in-out
  - Path: Straight line from source to destination
- **Card Flip**: 
  - Duration: 300ms
  - Rotation: 180 degrees on Y-axis
- **Book Formation**: 
  - Cards stack together with slight offset
  - Duration: 400ms
  - Glow effect on completion

---

## 5. GAME FLOW STATE MACHINE

### States
1. **WELCOME**: Initial screen with New Game button
2. **GAMEPLAY**: Active game with animations
3. **END_GAME**: Victory screen with options

### State Transitions
```
WELCOME --[New Game clicked]--> GAMEPLAY
GAMEPLAY --[Game ends]--> END_GAME
END_GAME --[New Game clicked]--> GAMEPLAY
END_GAME --[Exit clicked]--> EXIT
```

### Event Handling
- **Mouse Events**: Button clicks, hover effects
- **Keyboard Events**: 
  - Enter: Start new game / Continue
  - Escape: Exit game
  - Space: Pause/Resume (optional)

---

## 6. AUDIO SPECIFICATIONS

### Sound Effects
1. **victory.wav**: Played when game ends
   - Duration: 2-3 seconds
   - Format: WAV, 44.1kHz, 16-bit
   - Volume: Moderate (adjustable)

2. **card_move.wav**: Played when cards transfer
   - Duration: 0.3 seconds
   - Format: WAV, 44.1kHz, 16-bit
   - Volume: Soft

### Audio Implementation
- Use ALSA library for Linux audio
- Fallback to simple WAV file playback if ALSA unavailable
- Non-blocking audio playback (don't freeze UI)
- Option to mute sounds

---

## 7. BUILD SYSTEM (Makefile)

### Targets
- **all**: Build the graphical version (default)
- **console**: Build the original console version
- **clean**: Remove all build artifacts
- **install**: Install to system (optional)
- **run**: Build and run the graphical version
- **debug**: Build with debug symbols

### Dependencies
- X11 development libraries: `libx11-dev`
- ALSA development libraries: `libasound2-dev`
- C++ compiler: `g++` with C++11 support

### Compiler Flags
- **Release**: `-O2 -Wall -Wextra -std=c++11`
- **Debug**: `-g -O0 -Wall -Wextra -std=c++11 -DDEBUG`

---

## 8. DEVELOPMENT PHASES

### Phase 1: Core Refactoring (Priority: HIGH)
- Extract game logic from `gofish.cpp` into `GameEngine` class
- Implement event callback system
- Create step-by-step game progression methods
- Unit test game logic

### Phase 2: Basic X11 UI (Priority: HIGH)
- Initialize X11 window and event loop
- Implement welcome screen with buttons
- Handle mouse clicks and basic input
- Test window creation and event handling

### Phase 3: Card Rendering (Priority: MEDIUM)
- Design and implement card drawing functions
- Create card back and face rendering
- Implement basic card positioning
- Test card display

### Phase 4: Animation System (Priority: MEDIUM)
- Implement animation framework
- Add card movement animations
- Create smooth transitions
- Test animation timing

### Phase 5: Game Integration (Priority: HIGH)
- Connect GameEngine to UI
- Display game state in real-time
- Show AI decisions and actions
- Update UI based on game events

### Phase 6: Audio Integration (Priority: LOW)
- Set up ALSA audio system
- Create/source sound files
- Implement audio playback
- Test sound effects

### Phase 7: Polish & Testing (Priority: MEDIUM)
- Add visual effects and polish
- Optimize performance
- Test on different Ubuntu versions
- Fix bugs and edge cases

---

## 9. TESTING STRATEGY

### Unit Testing
- Test GameEngine logic independently
- Verify card counting and book detection
- Test AI decision-making

### Integration Testing
- Test UI-GameEngine communication
- Verify animations sync with game state
- Test audio playback timing

### User Acceptance Testing
- Verify all UI elements are clickable
- Ensure animations are smooth
- Confirm audio plays correctly
- Test game flow from start to finish

---

## 10. DOCUMENTATION REQUIREMENTS

### Code Documentation
- All public methods must have comments
- Complex algorithms need explanation
- README.md must include:
  - Build instructions
  - Dependencies
  - Usage guide
  - Screenshots

### User Documentation
- How to start a new game
- Understanding the UI layout
- Interpreting game messages
- Troubleshooting common issues

---

## 11. FUTURE ENHANCEMENTS (Post-MVP)

### Potential Features
1. Human vs AI mode (user can play)
2. Difficulty levels for AI
3. Network multiplayer support
4. Save/load game state
5. Statistics and game history
6. Customizable themes and card designs
7. Sound volume controls
8. Fullscreen mode
9. Multiple language support

---

## 12. RISK MANAGEMENT

### Technical Risks
- **X11 Complexity**: Mitigation - Start with simple UI, iterate
- **Animation Performance**: Mitigation - Profile and optimize early
- **Audio Library Issues**: Mitigation - Implement fallback options
- **Cross-platform Compatibility**: Mitigation - Test on multiple Ubuntu versions

### Project Risks
- **Scope Creep**: Mitigation - Stick to MVP features first
- **Time Constraints**: Mitigation - Prioritize core functionality
- **Resource Availability**: Mitigation - Use free/open-source assets

---

## 13. SUCCESS CRITERIA

The project is considered successful when:
1. ✅ Welcome screen displays with functional "New Game" button
2. ✅ Gameplay screen shows both AI players (AI1 and AI2)
3. ✅ Cards are visible and animate during gameplay
4. ✅ Game log displays AI actions in real-time
5. ✅ Celebratory sound plays when game ends
6. ✅ End screen offers "New Game" and "Exit" options
7. ✅ Makefile builds project successfully
8. ✅ Application runs on Ubuntu without crashes
9. ✅ Code is well-documented and maintainable
10. ✅ User can play multiple games in sequence

---

## REVISION HISTORY

- **v1.0** (2024-12-04): Initial master plan created
  - Defined project structure and architecture
  - Established coding standards
  - Specified UI/UX requirements
  - Outlined development phases

---

**Document Status**: APPROVED  
**Last Updated**: 2024-12-04  
**Next Review**: After Phase 2 completion