/*
  ==============================================================================

    Distortions.cpp
    Created: 24 Mar 2021 3:23:45pm
    Author:  Rodolfo Ortiz

  ==============================================================================
*/

#include "Distortions.h"

float Distortions::arcTanDistortion(float input, float gain)
{
    float out = (2.0f / M_PI) * atan(gain * input); //0.16 scale factor to maintain output volume control
    
    return out;

}

float Distortions::softClipper(float input, float gain)
{
    float newInput = input * (gain / 10);
    float out = 0.0;
    
    if (newInput >= 1.0f)
        out = 1.0f;
    else if ((newInput > -1) && (newInput < 1))
        out = (3.0f/2.0f) * (newInput - (pow(newInput, 3.0f)/3.0f));
    else if (newInput <= -1)
        out = -1.0f;
    
    return out;
}

float Distortions::sigmoid(float input, float gain)
{
//    auto newGain = gain + 1;
//
//    float out = (2.0f * (1.0f / (1.0f + exp(-newGain * input)))) - 1;
    
    gain = gain + 1.0f;
    
    float out = (2.0f * (1.0f / (1.0f + exp(-gain * input)))) - 1;
    
    out = out/log(gain);
    
    return out;
}

float Distortions::hyperbolicTangent(float input, float gain)
{
    float out =  (tanh(gain*input)) / (tanh(gain));
    
    return out;
}





