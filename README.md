# Real Time Music Visualization

## Overview

The is a rendition project of one of my previous works [Music Visualization](https://github.com/BenWeiTang/Music-Visualization). The idea is to perform realtime pitch detection, and utilize the data to generation images based on [the circle of fifths](https://en.wikipedia.org/wiki/Circle_of_fifths) and the [color wheel](https://en.wikipedia.org/wiki/Color_wheel). What is different this time, is that I decided to use the SFML library and C++.

## Demo

https://user-images.githubusercontent.com/78770681/202867241-61586c00-cfe4-429d-96a3-eb27eca97980.mp4

## Dev Notes

### Nov 15, 2022

Improvement:
- Implemented mechanism of saving images after finishing playing an audio file.

### Nov 5, 2022

Improvements:
- Memory usage now plateaus around 178 MB.
  - The underlying implementation is to periodically offload the vertices to the GPU using a `sf::VertexBuffer` instead of continuously adding to a `sf::VertexArray` which had been the previous approach.
  - Unlike a dynamic array, a `sf::VertexBuffer` has a fixed size. Therefore, the current implementation is to bundle enough vertices into a sub pool. And then upon drawing, the program iterates through a pool of sub pools to draw all the stored vertices.
  - For more details, see [VertexBufferPool.hpp](https://github.com/BenWeiTang/Real-Time-Music-Visualization/blob/main/Real%20Time%20Music%20Visualization/VertexBufferPool.hpp) and [VertexBufferPool.cpp](https://github.com/BenWeiTang/Real-Time-Music-Visualization/blob/main/Real%20Time%20Music%20Visualization/VertexBufferPool.cpp).

### Nov 4, 2022

Improvements this week:
- The histories of the lines are now kept track of and will be continuously drawn on the canvas.
- The interval between two notes now correlates to its respective color on the color wheel.
  - The color model in use is HSV, which is then translated to RGB to fit SFML's default color model.
  - A perfect unison is represented by a very tinted hue very close to white.
  - The hue for a perfect unison of each play session is randomly generated.
  - As the interval increases, the hue deviates from the hue of the perfect unision.
  - If the interval is up a perfect fifth, for example, the hue rotates clockwise a step. If the interval is down a perfect fifth (or up a perfect fourth), the hue rotates counterclockwise a step.
  - The saturation increases with the interval while the value decreases accordingly.
  - Memory usage is temporarily moderated by controlling the capturing rate. The program can now play an audio file of length 4 minutes with about 800 MB memory use at max.

Future goals:
- Halve the memory usage.
  - A possible solution is to use `sf::VertexBuffer` to replace a portion of entries in currently used `sf::VertexArray`.
- Allow for saving the canvas upon finishing playing the audio file by the user.

https://user-images.githubusercontent.com/78770681/200097389-bb4503a2-2cee-4b1d-bcd3-c095cc61a95f.mp4

### Oct 27, 2022

The issue with mono audio file is tmeporarily solved by halving the buffer size. Stereo audio files will still result in snappier animation, but those resulting from mono audio files are visually good enough for now.

The following is a work in progress of drawing the notes. Each note is an invisible point, and when two points are close enough, a line is drawn between the two. The points are emitted from the middle and from left to right. The point of emmision indicates the point in time of the audio file when the note is picked up. In other words, if a note is emitted at the center of the window, the note is played in this middle of the duration of the audio file. 

https://user-images.githubusercontent.com/78770681/198431304-8af4a80e-77d3-4fc7-9781-a24c8ff78e29.mp4

### Oct 22, 2022

Currently, it workds fine with wav files with 44100Hz sample rate and 2 audio channels. For some reason, the processing of 44100 Hz, mono audio file is slower.
