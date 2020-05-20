# Duels framework

Ongoing work for basic 1 vs 1 programming contests

# Requirements

The project uses Python3 with YAML and Pygame:

- `sudo apt install python3-yaml python3-pip`
- `pip3 install pygame`

# Game description files

Each game has a YAML description file, listing:

- the refresh rate of the game and the GUI
- the structure of the messages that are used for this game

The messages are:

- init: what should be sent by the game at initialization
- input: the expected player input
- feedback: the feedback from the game to a player
- display: what is required to display the state (not accessible to the player)

The GUI should be able to receive two types of messages:

- `init`: how the display should be initialized, including the player names
- `display`: how the display should be updated, including the winner of the game, if any

# Creating or designing a new game

Starting from the game folder, including its description file, run the `gen_game.py` script: 
`./gen_game.py <game_folder>`. 

It will create four files:

- the C++ header corresponding to the game description file
- the C++ project (`CMakeLists.txt` / `main.cpp` files)
- A GUI template (`<game>_gui.py`)

All you have to do then is modify the created files in order to create the game.

If the description file needs to be updated during development, only the header will be overwritten.

- The game mechanics should be done within the corresponding C++ class (automatically created).
- The `main.cpp` file is to be modified by a future user / player that wants to compare its own AI to the one in the game.
