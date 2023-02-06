# Game for CompSCI
<sub>*A GrahamSH project*</sub>

## How to run

SFML is fairly hard to install. I recommend using the my precompiled binary of this game [here](https://raw.githubusercontent.com/GrahamSH-LLK/game/main/GAME).

If you want to install it yourself and compile, you can follow these instructions. 


1. Install brew (if you don't have it already) (https://brew.sh/). It might take a little bit to install. Make sure to run the command it gives you at the end of the installation.
2. Install SFML by running `brew install sfml pkg-config`
3. Clone this repository by running `git clone https://github.com/GrahamSH-LLK/game.git`
4. Go into the directory by running `cd game`
5. Compile the game by running `g++ $(pkg-config --cflags --libs sfml-graphics) main.cpp -o GAME && ./GAME`

