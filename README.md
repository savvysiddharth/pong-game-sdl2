# pong-game-sdl2
2 player ping-pong game.

Notes:
------

<i>(tested only on linux)</i>

SDL2 libraries must be installed.

<code>sudo apt-get install libsdl2-dev libsdl2-image</code>

given binary file is for linux.

Instructions:
-------------
To compile the file:

<code>g++ pong.cpp -o pong -lSDL2 -lSDL2_image</code>

While executing the game, make sure image files (dot.bmp and plank.bmp) are in same directory.

<code>./pong</code>
