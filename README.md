# Real Time Music Visualization

This project is currently work in progress.

## Overview

The is a rendition project of one of my previous works [Music Visualization](https://github.com/BenWeiTang/Music-Visualization). The idea is to conduct realtime pitch detection, and utilize the data to generation images based on [the circle of fifths](https://en.wikipedia.org/wiki/Circle_of_fifths) and the [color wheel](https://en.wikipedia.org/wiki/Color_wheel). What is different this time, is that I decided to use the SFML library and C++.

## Dev Notes

### Oct 27, 2022

The issue with mono audio file is tmeporarily solved by halving the buffer size. Stereo audio files will still result in snappier animation, but those resulting from mono audio files are visually good enough for now.

The following is a work in progress of drawing the notes. Each note is an invisible point, and when two points are close enough, a line is drawn between the two. The points are emitted from the middle and from left to right. The point of emmision indicates the point in time of the audio file when the note is picked up. In other words, if a note is emitted at the center of the window, the note is played in this middle of the duration of the audio file. 

https://user-images.githubusercontent.com/78770681/198431304-8af4a80e-77d3-4fc7-9781-a24c8ff78e29.mp4

### Oct 22, 2022

Currently, it workds fine with wav files with 44100Hz sample rate and 2 audio channels. For some reason, the processing of 44100 Hz, mono audio file is slower.
