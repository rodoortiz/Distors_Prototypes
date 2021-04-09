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
    gain = gain + 1.0f;
    
    float out = (2.0f / M_PI) * atan(gain * input);
    
    out = out/log(gain);
    
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
    gain = gain + 1.0f;
    
    float out = (2.0f * (1.0f / (1.0f + exp(-gain * input)))) - 1;
    
    out = out/log(gain);
    
    return out;
}

float Distortions::hyperbolicTangent(float input, float gain)
{
    gain = gain + 1.0f;
    float out =  (tanh(gain*input)) / (tanh(gain));
    
    out = out/log(gain);
    return out;
}

float Distortions::diodeClipping(float input, float gain)
{
//    gain = gain + 1.0f;
    
    float diodeClippingAlgorithm = exp((0.1 * input) / (0.0253 * 1.68)) - 1;
    float out = 2/M_PI * atan(diodeClippingAlgorithm * (gain * 16));
    
    out = out * 0.3;
//    out = out/log(gain);
    
    return out;
}

float Distortions::fuzzExponential(float input, float gain)
{
    float newInput = input * gain;
    float out;

//    if (newInput < 0) {
//        float out = 1 * ((1 - exp(abs(newInput))/(exp(1) - 1)));
//        return out;
//    } else {
//        float out = -1 * ((1 - exp(abs(newInput))/(exp(1) - 1)));
//        return out;
//    }

    if (newInput < 0) {
        out = -1 *  (1 - exp(-abs(newInput)));
    } else {
        out = 1 *  (1 - exp(-abs(newInput)));
    }
 
    //Half Wave Rectifier
    out = 0.5 * (out + abs(out));
    
    return out;

}









