#Spectrum
<body>This is a personal project designed to be used to analyze color in films. It still has a long way to go, but it so far has the basic feature I was trying to accomplish: a "color timeline". Currently, you can compute, view, and save this image. I have many more ideas for how to improve and add to this program, some of which can be viewed in the issue tracker.</body>
##How It Works
<body>Spectrum currently works by using OpenCV to analyze color in each frame of a video. The computed average color from this frame is used as one "line" in the color timeline. The process of computing the color timeline can be sped up by analyzing less than every frame (skipping frames), at the cost of having a lower resultion for the color timeline.</body>


<body>Another major issue that effects performance is the resultion of the source video. When analyzing a full length HD movie, it can take hours to compute the color timeline. Currently this is solved by using a system call to run ffmpeg to compress the source video, which is then deleted after Spectrum has been run. There are plans to integrate ffmpeg into the program in the future, without requring system calls.</body>

## Installation

<body>Currently I am only aware of how to run on OS X and Linux. The steps to do so are as follows: </body>

### OS X:
<body>Using the Homebrew package manager: </body>
<body>Install OpenCV

<code>brew install opencv </code>

Install ffmpeg

<code>brew install ffmpeg </code>

Clone the repo (install git if you already haven't):

<code>git clone https://github.com/mtonsmann/Spectrum.git Spectrum</code>

From the cloned directory, make and run the program:

<code>make</code>

<code>./spectrum</code>
