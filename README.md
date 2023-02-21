# Real Time Music Visualization

## Overview

https://user-images.githubusercontent.com/78770681/202867241-61586c00-cfe4-429d-96a3-eb27eca97980.mp4

For more details about what you're seeing, check out my [portfolio page](https://www.ben-tang.com/work/rtmv) for step-by-step explanations with animated diagrams!

## Circular Cache

One premise of drawing a line is to know where the two circles are located. Every time we want to capture all the possible lines, we have to loop through all the circles, and in each iteration, we also have to loop through all the other circles to checks whether a line can be drawn between each and every pair of circles there exist. In short, this is an O(n^2) operation that is being done multiple times per second. Not efficient at all. And it gets worse as the music continues to play and more notes get detected and added to the calculation.

Luckily, based on what we know about the drifting behavior of a note (as shown in [What About The Entire Song?](https://github.com/BenWeiTang/Real-Time-Music-Visualization#what-about-the-entire-song)), we can conclude that a note doesn't move horizontally as much as it does vertically. Therefore, two notes that are played far apart across the timeline are not likely to interact with each other. This is good news for us because it means that as time goes by, we can start to ignore the earlier notes. By ignore I mean not updating the notes' movements and not checking them for intersections.

If we only loop through a finite number of notes each time, the runtime is suddenly reduced to O(1). Better yet, if we continue to ignore earlier notes, lines on the left side will stop stretching out and we get a more balanced picture.

To do that, we want to create a cache for a collection of notes. We only want to update notes that are present in the cache. Also, only their intersections are examined.

In this system, when a new note is detected, it gets added to the cache for calculation. We can construct a mental image of this by using a circular structure. Like this:

<div align="center">
<video src="https://user-images.githubusercontent.com/78770681/219882614-e1fd7f14-d08e-4f22-8251-5efe02102357.mp4">
</div>

Internally, it is just an array with a pointer that cycles and tracks where to put the upcoming note. The pointer is represented by the red triangle rotating around the cache. Older notes fade out more than new notes in opacity to indicate that they are not as *fresh*.

If the cache is full and a new note pops up, the new note takes the place of the oldest note in the cache. Like this:

<div align="center">
<video src="https://user-images.githubusercontent.com/78770681/219884975-fa1bfad7-b136-451e-a36d-e04dad93c533.mp4">
</div>

On a more technical note (haha very funny), adding a note is done by calling [NotePool::EmplaceBack](https://github.com/BenWeiTang/Real-Time-Music-Visualization/blob/6ef2eee66cc64d24a8ff405f3a83ae3ac40c7e65/Real%20Time%20Music%20Visualization/NotePool.cpp#L35) where `NotePool` is the circular cache ADT and `EmplaceBack` indicates that move semantics are used. Because it uses the move semantics, the older data simply gets overwritten. And since the internal array stores the instances of the [Note](https://github.com/BenWeiTang/Real-Time-Music-Visualization/blob/main/Real%20Time%20Music%20Visualization/Note.hpp) class not just pointers to the instances, it means there is no memory leaks, either.

In summary, by only keeping an eye out for the newer notes, we avoid the burden of combinatorial explosion while making sure the line distribution across the canvas is balanced.

## GPU Rendering

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
