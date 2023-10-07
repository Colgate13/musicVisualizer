#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <raylib.h>

#define SCREEN_WIDTH (int)800
#define SCREEN_HEIGHT (int)600
#define FPS (int)60
#define ARRAY_LEN(xs) sizeof(xs) / sizeof(xs[0])

typedef struct
{
  float left;
  float right;
} Frame;

Frame global_frames[4800 * 2] = {0};
size_t global_frames_count = 0;

void callback(void *bufferData, unsigned int frames)
{
  size_t capacity = ARRAY_LEN(global_frames);

  if (frames <= capacity - global_frames_count)
  {
    memcpy(global_frames + global_frames_count, bufferData, sizeof(Frame) * frames);
    global_frames_count += frames;
  }
  else if (frames <= capacity)
  {
    memmove(global_frames, global_frames + frames, sizeof(Frame) * (capacity - frames));
    memcpy(global_frames + (capacity - frames), bufferData, sizeof(Frame) * frames);
  }
  else
  {
    memcpy(global_frames, bufferData, sizeof(Frame) * capacity);
    global_frames_count = capacity;
  }
}

int main(void)
{
  printf("Hello to CoolSound desktop local player!\n");

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CoolSound desktop local player");
  SetTargetFPS(FPS);
  InitAudioDevice();

  Music music = LoadMusicStream("The-Tower-of-Dreams-_new__1_.ogg");

  printf("music.frameCount: %u\n", music.frameCount);
  printf("music.sampleRate: %u\n", music.stream.sampleRate);
  printf("music.sampleSize: %u\n", music.stream.sampleSize);
  printf("music.channels: %u\n", music.stream.channels);

  assert(music.stream.sampleSize == 16);
  assert(music.stream.channels == 2);

  PlayMusicStream(music);
  SetMusicVolume(music, 0.5f);
  AttachAudioStreamProcessor(music.stream, callback);

  while (!WindowShouldClose())
  {
    UpdateMusicStream(music);

    if (IsKeyPressed(KEY_SPACE))
    {
      if (IsMusicStreamPlaying(music))
      {
        PauseMusicStream(music);
      }
      else
      {
        ResumeMusicStream(music);
      }
    }

    int w = GetRenderWidth();
    int h = GetRenderHeight();

    BeginDrawing();
    ClearBackground(CLITERAL(Color){0x18, 0x18, 0x18, 0xFF});
    float cell_width = (float)w / global_frames_count;
    for (size_t i = 0; i < global_frames_count; i++)
    {
      float t = global_frames[i].left;
      if (t > 0)
      {
        DrawRectangle(i * cell_width, h / 2 - h / 2 * t, 1, h / 2 * t, RED);

        // float tRight = global_frames[i].right;
        // DrawRectangle(i * cell_width, h / 2 - 0.1, 1, h / 2 * tRight, PURPLE);
      }
      else
      {
        DrawRectangle(i * cell_width, h / 2, 1, h / 2 * t, PURPLE);
      }
    }
    EndDrawing();
  }

  return 0;
}