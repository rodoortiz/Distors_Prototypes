/*
  ==============================================================================

    Distortions.h
    Created: 24 Mar 2021 3:23:45pm
    Author:  Rodolfo Ortiz

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <cmath>

class Distortions {
public:
    float arcTanDistortion(float input, float gain);
    float softClipper(float input, float gain);
    float sigmoid(float input, float gain);
    float hyperbolicTangent(float input, float gain);
    float diodeClipping(float input, float gain);
    float fuzzExponential(float input, float gain);
    float pieceWiseOverdrive(float input, float gain);
};
