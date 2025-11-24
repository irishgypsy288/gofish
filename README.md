This repository contains a command-line implementation of the classic card game "Go Fish," where two AI players compete against each other. Run the program to watch the AIs play a full game, making decisions on asking for cards, drawing from the deck, and forming sets.The game simulates the traditional rules of Go Fish:A standard deck of 52 cards is used, divided into 13 ranks (Ace through King) with 4 suits each.
Each player starts with a hand of cards (typically 7 for two players).
Players take turns asking the opponent for specific ranks. If the opponent has the rank, they hand over all cards of that rank. If not, the asking player draws from the deck ("Go Fish!").
The goal is to form the most "books" (sets of 4 cards of the same rank).
The game ends when the deck is empty and no more moves can be made, with the player having the most books declared the winner.

This project is written in C++ and is designed as a simple demonstration of game logic, random decision-making, and basic AI behavior.FeaturesFully automated gameplay between two AI opponents.
Console output showing each turn, hands (optionally hidden or shown), and game progress.
Random shuffling and dealing for varied games.
Simple AI logic: Randomly selects ranks to ask for based on current hand.

RequirementsC++ compiler (e.g., g++ for GCC/MinGW, cl for Visual Studio).
Standard C++ library (no external dependencies).

Building and RunningOn Linux/macOSClone the repository:

git clone https://github.com/irishgypsy288/gofish.git
cd gofish

Compile the source code (assuming single file gofish.cpp or multiple files):

g++ gofish.cpp -o gofish

If there are multiple source files (e.g., card.cpp, player.cpp), compile them together:

g++ *.cpp -o gofish

Run the executable:

./gofish

On WindowsClone the repository using Git for Windows or download as ZIP and extract.
Option 1: Using MinGW (GCC for Windows)  Install MinGW (e.g., via MSYS2 or standalone installer) to get g++.  
Open Command Prompt or PowerShell in the repository directory.  
Compile:

g++ gofish.cpp -o gofish.exe

For multiple files:

g++ *.cpp -o gofish.exe

Run:

gofish.exe

Option 2: Using Visual Studio  Install Visual Studio (Community edition is free) with C++ workload.  
Open the repository folder in Visual Studio (File > Open > Folder).  
Create a new project or use the command-line tools.  
In Developer Command Prompt for VS:
Navigate to the repository directory and compile:

cl gofish.cpp /EHsc /o gofish.exe

For multiple files:

cl *.cpp /EHsc /o gofish.exe

Run:

gofish.exe

Note: If the project uses any Windows-specific headers or has dependencies, ensure they are installed. This project appears to use standard C++11 features, so no additional libraries are required.UsageSimply run the executable, and the game will start automatically. Output will be printed to the console, showing each AI's actions and the game state.Example output snippet:

AI1 asks AI2 for Queens.
AI2: Go Fish!
AI1 draws a card.
...
AI1 forms a book of Aces!

