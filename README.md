# pong-game-sdl2
2 player ping-pong game.

Notes:
------

<i>(tested only on linux, and it is buggy)</i>

SDL2 libraries must be installed.

<code>sudo apt-get install libsdl2-dev libsdl2-image-dev</code>

given binary file is for linux.

Instructions:
-------------
To compile the file:

<code>g++ pong.cpp -o pong -lSDL2 -lSDL2_image</code>

While executing the game, make sure image files (dot.bmp and plank.bmp) are in same directory as executable file.

<code>./pong</code>

Screenshot:
-----------

![pong](https://user-images.githubusercontent.com/12862695/48730443-60367f80-ec60-11e8-9acd-8c06f837e141.png)

Default game controls:
---------------------

<code>R</code> - Reset ball position

Player 1 (Left):

<code>W</code> - Up

<code>S</code> - Down

Player 2 (Right):

<code>I</code> - Up

<code>K</code> - Down
