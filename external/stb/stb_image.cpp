/*
Not part of upstream stb. stb_image.h is a single-file library: including it
normally yields declarations only. Defining STB_IMAGE_IMPLEMENTATION before the
include expands the function bodies too, and that must happen in exactly one
translation unit or the definitions collide at link time. This file is that
translation unit; every other file just includes "stb_image.h" plainly.
*/
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
