# nickelulzian d00m renderer

This is a doom-style raycasting project designed to do a few things.
I want it to be able to fully load and render a live map with a corner
minimap, save player and map data to SQL, and load fresh maps from
specially-designed map files (the format of which will likely be stored
either in JSON, plaintext, or another format, depending on what seems
right at the time).

This project is based off of [Lode's Computer Graphics Tutorial on
Raycasting](https://lodev.org/cgtutor/raycasting.html), using SDL.
Special thanks, as well, to [this timing tutorial](https://thenumb.at/cpp-course/sdl2/08/08.html).
  
### BUILD INSTRUCTIONS (Development):

1. Install SDL, SDL-image, and SDL-ttf system-wide (Debian-Based):

```sh
$ sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

2. Install the full repository recursively (some libraries are included locally, like CGLM, ini.c, and log.c)

```sh
$ git clone https://github.com/nickelulz/n00m.git --recursive
```

3. Enter the directory and run:

```sh
$ make run
```
