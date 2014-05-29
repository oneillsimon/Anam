#pragma once

bool SDLGetIsCloseRequested();
void SDLSetIsCloseRequested(bool value);
void SDLCreateWindow(const char* title, int x, int y, int width, int height, bool fullScreen);
void SDLSetFullScreen(bool value);
void SDLSwapBuffers();
void SDLDestroyWindow();
void SDLSetMousePosition(int x, int y);