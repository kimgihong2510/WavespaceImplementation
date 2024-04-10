#pragma once

#include <math.h>
#define _USE_MATH_DEFINES

constexpr float PI = static_cast<float>(M_PI);

constexpr unsigned int SIZE_AMP = 512; // size of amp output vector
constexpr unsigned int SIZE_PHASE = 512; // size of phase vector
constexpr unsigned int SIZE_WAVEFORM = 1024; // size of waveform
constexpr unsigned int SIZE_WAVEFORM_LOG2 = 10; // size of waveform

constexpr unsigned int THUMBWIDTH = 30;

constexpr unsigned int NUM_STYLE = 2; // number of styles
constexpr unsigned int NUM_DESCRIPTOR = 5; // number of discriptors 
constexpr unsigned int IDX_SYMMETRY = 4; // index of symmetry, if not using set to -1
constexpr char* NAMES_STYLE[] = {"softwaves", "rect"}; // names of each style
constexpr char* NAMES_DESCRIPTOR[] = {"Brightness", "Richness", "Fullness", "Undulation", "Symmetry"};  // names of each descriptor

constexpr float ADSR_MIN = 0.001f;
constexpr float ADSR_MAX = 3.0f;

constexpr float STYLE_CONDITION_MIN = 0.0f; // minimum of x, y values of style
constexpr float STYLE_CONDITION_MAX = 5.0f;
constexpr float DESCRIPTOR_CONDITION_MIN = 0.0f; // minimum of x, y values of style
constexpr float DESCRIPTOR_CONDITION_MAX = 1.0f;

constexpr unsigned int NUM_SYNTH_POLYPHONY = 8;
constexpr int LOAD_MODEL_PER_SEC = 60;