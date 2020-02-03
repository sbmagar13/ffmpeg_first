
//
// Video processing example using FFmpeg
// Written by Ted Burke - last updated 12-2-2017
//

#include <stdio.h>
#include <math.h>
#include <stdint.h>

// Video resolution
#define W 1280
#define H 720

// Allocate a buffer to store one frame
unsigned char frame[H][W][3] = { 0 };

void main()
{
    int x, y, count;

    // Open an input pipe from ffmpeg and an output pipe to a second instance of ffmpeg
    FILE* pipein = _popen("ffmpeg -i A.mp4 -f s16le -ac 1 -", "r");
    FILE* pipeout = _popen("ffmpeg -y -f s16le -ar 44100 -ac 1 -i - -vf format=yuv420p -map 0:a -codec:a aac -strict -2 VoutA.mp4", "w");
    
    //// Process video frames
    //while (1)
    //{
    //    // Read a frame from the input pipe into the buffer
    //    count = fread(frame, 1, H * W * 3, pipein);

    //    // If we didn't get a frame of video, we're probably at the end
    //    if (count != H * W * 3) break;

    //    // Process this frame
    //    for (y = 0; y < H; ++y) for (x = 0; x < W; ++x)
    //    {
    //        // Invert each colour component in every pixel
    //        frame[y][x][0] = 255 - frame[y][x][0]; // red
    //        frame[y][x][1] = 255 - frame[y][x][1]; // green
    //        frame[y][x][2] = 255 - frame[y][x][2]; // blue
    //    }

    //    // Write this frame to the output pipe
    //    fwrite(frame, 1, H * W * 3, pipeout);
    //}

    // Read, modify and write one sample at a time
    int16_t sample;
    int n = 0;
    while (1)
    {
        count = fread(&sample, 2, 1, pipein); // read one 2-byte sample
        if (count != 1) break;
        ++n;
        sample = sample * sin(n * 5.0 * 2*M_PI / 44100.0);
        fwrite(&sample, 2, 1, pipeout);
    }

    // Flush and close input and output pipes
    fflush(pipein);
    _pclose(pipein);
    fflush(pipeout);
    _pclose(pipeout);
}