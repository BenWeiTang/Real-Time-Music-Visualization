# Real Time Music Visualization

## Overview

https://user-images.githubusercontent.com/78770681/202867241-61586c00-cfe4-429d-96a3-eb27eca97980.mp4

## How It Works

### What Is Going On?

What you’re looking at is some representation of music that I came up with. By taking in an audio file as input, it spits out a video that visualizes the music as it plays. Below is the step-by-step process of how everything comes together.

### Picking Up The Frequency

When the music is playing, the sound wave is like a very complex function, with random ups and downs. If we play the first five seconds of the music and feed the sound wave into a special transforming function, we will get a bunch of frequencies that are *loud* in the first five seconds. This special function is the Fast Fourier Transform, or FFT.

Right now, I’m omitting a lot of technicalities because I only understand FFT to the extent that I know what the things it spits out mean. (I need to catch up on my linear algebra for sure.) There are a lot of resources about the amazing math behind the FFT.

If we keep running FFT over every second of the music while it plays, we essentially get a bunch of music notes that are being played (or sung). It is as though we reverse-engineered the audio back into sheet music.

### How Notes Interact

One way to think about music is to think about the notes in relation to other notes, not in isolation. So, we can approach visualizing music by visualizing the interactions between different notes.

Let’s imagine that a note is a circle in 2D space. Then, when two notes overlap, we draw a line that connects their centers.

Simply applying some geometry tricks tells us when the two circles overlap. It is when the distance between the two centers is shorter than their radii combined.

<p align="center">
<img width="50%" height="50%" src="https://user-images.githubusercontent.com/78770681/219546230-08885b87-a9d0-4189-a5d4-1e410d78e77d.gif">
</p>

If we record the line overtime, we get something like this:

<p align="center">
<img width="50%" height="50%" src="https://user-images.githubusercontent.com/78770681/219546481-97fd619c-105d-49ea-950e-58c66997d1bd.gif">
</p>

That is too dense for my taste. Also, if we draw too many lines, there might be a performance issue with SFML. Let’s skip some lines and only draw only a few.

<p align="center">
<img width="50%" height="50%" src="https://user-images.githubusercontent.com/78770681/219546520-d59c26fc-dc88-4bbb-be41-3638b7ce4842.gif">
</p>

This is basically a die-down version of what you see above.

### What About the Entire Song?

We now have an idea of how two notes interact with one another when they overlap. But what about a complete song?

A song can be thought of as a series of notes being played across time. So, it is important that we also visualize the temporal aspect of a song. Most of Western cultures read from left to right, and the UI of your favorite music streaming service probably shows the progress of a song in the same way.

So the language we speak here is: notes on the left are played before those that are on the right. We can arrange all the notes that we picked up onto a horizontal line, with their positions corresponding to the moments at which they are played. We can then make them randomly go up or down, and get something like this:

![Circle_Intersection_04](https://user-images.githubusercontent.com/78770681/219546660-9a30cd84-fc4a-44d8-a607-323a2333fd19.gif)

Right now, they are just going either straight up or straight down. Let’s add some horizontal movement to each of them:

![Circle_Intersection_05](https://user-images.githubusercontent.com/78770681/219546688-1ba214f3-09c1-4938-80b6-22aa3f4afc9e.gif)

Let’s draw some lines when the circles overlap:

![Circle_Intersection_06](https://user-images.githubusercontent.com/78770681/219546725-16ba3960-4206-4f35-b977-55f0865a2bb9.gif)

If we only record the lines and don’t show the circles, this is what we get:

![Circle_Intersection_07](https://user-images.githubusercontent.com/78770681/219546772-c81d8b84-4ba9-45fd-aa67-d0c931d91413.gif)

Kinda cool already, right?

### Intervals As Colors

So far, it is interesting to see the interactions amongst notes over time, and that in and of itself also tells about the intensity of the music throughout the music. Check out this example in [my old project](https://github.com/BenWeiTang/Music-Visualization/blob/main/Examples/Merry_Christmas_Mr_Lawrence.pdf). The idea is that the more notes that are played within a certain window, the more lines that are drawn. On the contrary, if the music is just silence, not line will be there.

However, we are still interested in visualizing the quality of the sound. Remember that how notes are to each other is more important than just understanding the notes in isolation.

In music theory, the distance between two notes is called an **interval**. There are many kinds of intervals and each of them has a unique quality. Some intervals sound more pleasant than the others. Some intervals create tensions while the others present resolutions. We will come back to intervals in just a bit because we have to look at another concept first, the Circle of Fifths.

<p align="center">
<img width="50%" height="50%" src="https://user-images.githubusercontent.com/78770681/219546849-f1bb6a22-8d46-4563-bf26-428a8af70093.png">
</p>

This is an oversimplified version of the Circle of Fifths, but for the purpose of this project, this will suffice. The letters you see are the twelve unique notes in Western Music. In short, there are only twelve notes and the rest are just variations of these twelve notes.

As you can see, the notes are not arranged in an alphabetical order. In fact, they are arranged in a way so that the neighboring note in the clockwise rotation is a **perfect fifth** away. For example, the note G is a perfect fifth away from the note C, and the note Ab is a perfect fifth away from the note Db.

A perfect fifth is an interval, one that is stable and calming, if you will. As a side note, a step counterclockwise will result in a **perfect fourth**. So, F is a perfect fourth away from C. A perfect fourth also sounds pretty good. For this reason, notes next to each other sound better when played together. On the other hand, notes that are many steps apart on the Circle can make you feel uneasy.

(FYI, the notes on the opposite ends create a kind of interval called **tritones**. They have their uses in music, but that is out of scope for this project.)

The reason why I bring up the Circle of Fifth is that it gives us a way to look at distances between notes and how they will make us feel. It is an easy way to quantify the quality of intervals. If two notes in our 2D space are close enough and a line is drawn to connect their centers, it is an opportunity for us to visualize this connection because we now know how stable or unstable these two notes will sound together. We just have to look up their positions on the Circle of Fifth.

Take a look again at the Circle, and find C, G, and B. G is only a step away from C while B is five steps away from C. This means B and C probably won’t sound as stable as C and G. We can generalize the process that we just did into a graph like this:

<p align="center">
<img width="50%" height="50%" src="https://user-images.githubusercontent.com/78770681/219546883-d6cf69c5-4a0b-40fa-8420-40068e79ef02.png">
</p>

Here I changed from using steps to angles, but the concept is the same: a step is just 30° delta. (360° divided by 12 is 30°.) The farther away the two notes on the Circle, the larger the absolute value of the angle between.

So at this point, I hope it is obvious enough that the concept of the Circle of Fifth is not so dissimilar to the concept of the color wheel. The closer, the more analogous; the farther, the more complementary.

For that, we can map every interval to a color using the color wheel. We first randomly pick a hue for the zero-step interval—also called a **perfect unison**. (So many perfect intervals, I know.)  And then, we branch out to find more colors until we exhaust all intervals. Something like this:

<p align="center">
<img width="50%" height="50%" src="https://user-images.githubusercontent.com/78770681/219546911-8f7e4c02-d26c-4af8-b215-d438d164a49d.gif">
</p>

Notice two things that are a bit different from the color wheel we know.

First, the colors are symmetrical. This is because when two notes meet, there can be, and in most cases will be, two intervals generated. Take C and G. G is a perfect fifth to C, but C is a perfect fourth to G. In music theory, there are some nuances and differences, but here we know that the quality of the two intervals should be the same. Thus, we don’t want to distinguish between clockwise and counterclockwise steps.

Second, the colors for tritones are not really complementary. This is because I’m using the HSV color wheel. It just so happens that the hue in HSV is represented by angles, which is very handy in procedurally picking colors. Although complementary colors aren’t on the opposite sides of the wheel, analogous colors are still close to each other, which is good enough for us, as you will see later.

### Fine-tuning Color Selection

If we actually use the above method to generate colors for our intervals, it will be a very colorful canvas that we paint on. In fact, this is the approach that I used in [my previous project](https://github.com/BenWeiTang/Music-Visualization). A bunch of dissonance in frequencies actually makes a visual wonder.

For this project, I wanted it to be more subtle, chromatically. (No pun intended if you know color theory *AND* music theory.)

The first thing we can do is to limit the range of hues that we use.

<p align="center">
<img width="50%" height="50%" src="https://user-images.githubusercontent.com/78770681/219547005-d4698312-da9d-4883-9a6c-da965c3cb5d8.gif">
</p>

See how even the tritone can produce a more harmonious color with green that represents perfect unison, fourth and fifth. But that will become irrelevant as we have one more step to do.

Next thing we can do is to tone down the saturation and value (or brightness) as we move further and further from the perfect unison. We can also do the same to the alpha channel as well.

<p align="center">
<img width="50%" height="50%" src="https://user-images.githubusercontent.com/78770681/219547037-c088673d-0042-498f-8aca-b19db4824714.gif">
</p>

We effectively ignore the more dissonant intervals and only show the intervals that are more harmonious. The lower half of the circle is pretty much invisible.

With some more fine-tuning, we get what is shown in Overview.

I hope you enjoy it!

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
