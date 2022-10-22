# Real Time Music Visualization

This project is currently work in progress.

## Overview

The is a rendition project of one of my previous works [Music Visualization](https://github.com/BenWeiTang/Music-Visualization). The idea is to conduct realtime pitch detection, and utilize the data to generation images based on [the circle of fifths](https://en.wikipedia.org/wiki/Circle_of_fifths) and the [color wheel](https://en.wikipedia.org/wiki/Color_wheel). What is different this time, is that I decided to use the SFML library and C++.

## Dev Note

Currently, it workds fine with wav files with 44100Hz sample rate and 2 audio channels. For some reason, the processing of 44100 Hz, mono audio file is slower.
