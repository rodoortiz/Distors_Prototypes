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
    
    float out = (2.0f / juce::MathConstants<float>::pi) * atan(gain * input);
    
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
        out = (3.0f / 2.0f) * (newInput - (pow(newInput, 3.0f) / 3.0f));
    else if (newInput <= -1)
        out = -1.0f;
    
    out = out * 0.5;
    
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
    float out = (tanh(gain * input)) / (tanh(gain));
    
//    out = out/log(gain);
    out = out * 0.4;
    return out;
}

float Distortions::diodeClipping(float input, float gain)
{
//    gain = gain + 1.0f;
    
    float diodeClippingAlgorithm = exp((0.1 * input) / (0.0253 * 1.68)) - 1;
    float out = 2/juce::MathConstants<float>::pi * atan(diodeClippingAlgorithm * (gain * 16));
    
    out = out * 0.3;
//    out = out/log(gain);
    
    return out;
}

float Distortions::fuzzExponential(float input, float gain)
{
    float newInput = input * gain;
    float out;
    
    //Soft clipping
    if (newInput < 0.0f) {
        out = -1.0f *  (1.0f - exp(-abs(newInput)));
    } else {
        out = 1.0f * (1.0f - exp(-abs(newInput)));
    }
 
    //Half Wave Rectifier
    out = 0.5f * (out + abs(out));
    
    out = out * 0.8f;
    
    return out;

}

float Distortions::pieceWiseOverdrive(float input, float gain)
{
    float newInput = input * (gain) ;
    float out;
    
    if (abs(newInput) <= 1/3)
        out = 2 * newInput;
    else if (abs(newInput) > 2/3) {
        if (newInput > 0)
            out = newInput;
        if (newInput < 0)
            out = -newInput;
    } else {
        if (newInput > 0)
            out = (3 - pow((2 - newInput * 3), 2)) / 3;
        if (newInput < 0)
            out = -(3 - pow((2 - newInput * 3), 2)) / 3;
    }
    
    out = (out/log(gain + 1)) * 0.5f;
    
    return out;
}

float Distortions::tube(float input, float gain)
{
    float Q = -1.0f;
    float distortion = 25;
    float out;
    
    float newInput = input * (gain / 10);
    
    if (Q == 0) {
        out = newInput / (1 - exp(-distortion * newInput));
        if (newInput == Q) {
            out = 1 / distortion;
        }
    } else {
        out = ((newInput - Q) / (1 - exp(-distortion * (newInput - Q)))) + (Q / (1 - exp(distortion * Q)));
        if (newInput == Q) {
            out = (1 / distortion) + (Q / (1 - exp(distortion * Q)));
        }
    }
    
    out = out * 0.8f;
    
    return out;
}

float Distortions::arraya(float input, float gain)
{
    auto newInput = input * 1.0f;
    
    //Arraya
    auto out = ((3.0f * newInput) / 2.0f) * (1.0f - (pow(newInput, 2.0f) / 3.0f));
    
//    Fuzz Exponential
    if (out < 0.0f) {
        out = 1.0f * ((1.0f - exp(abs(out))/(exp(1.0f) - 1.0f)));
    } else {
        out = -1.0f * ((1.0f - exp(abs(out))/(exp(1.0f) - 1.0f)));
    }
    
    //Exponential 2
//    out = (exp(1) - exp(1 - out)) / (exp(1) - 1);
    
//    out = 0.5 * (out + abs(out));
//    out = abs(out);
    
    if (gain >= 10.0f) {
        out = out * (gain / 100.0f);
    } else
        out = out * (0.1f);
    
    //Arraya
    out = ((3.0f * out) / 2.0f) * (1.0f - (pow(out, 2.0f) / 3.0f));
    out *= 0.8f;
    
    return out;
}

float Distortions::gallo(float input, float gain)
{
    float a = -0.01f;
    float b = 0.7f;
    float k1 = pow(a, 2.0f);
    float k2 = 1 + (2 * a);
    float k3 = pow(b, 2.0f);
    float k4 = 1 - (2 * b);
    float out_1 = 0.0f;
    
    auto newInput = input * gain;
    
    if (newInput < a) {
        out_1 = (k1 + newInput) / (k2 - newInput);
    }
    if (newInput >= a && newInput <= b) {
        out_1 = newInput;
    }
    if (newInput > b) {
        out_1 = (newInput - k3) / (newInput + k4);
    }
    
    return out_1;
}

float Distortions::doubleSoftClipper(float input, float gain)
{
    auto slope = 2.0f;
    auto upperLim = 0.8f;
    auto lowerLim = -1.0f;
    auto upperSkew = 1.0f;
    auto lowerSkew = 1.0f;
    auto xOffFactor = 0.0f;
    auto out = 0.0f;
    
    auto xOff = (1.0f / slope) * pow(slope, xOffFactor);
    
    input *= (gain / 10.0f);
    
    if (input > 0.0f)
    {
        input = (input - xOff) * upperSkew;
        
        if (input >= 1.0f / slope)
        {
            out = upperLim;
        } else if (input <= -1.0f / slope)
        {
            out = 0.0f;
        } else
        {
            out = (3.0f / 2.0f) * upperLim * (slope * input - pow(slope * input, 3.0f) / 3.0f) / 2.0f + (upperLim / 2.0f);
        }
    }
    else
    {
        input = (input + xOff) * lowerSkew;
        
        if (input >= 1.0f / slope)
        {
            out = 0.0f;
        } else if (input <= -1.0f / slope)
        {
            out = lowerLim;
        } else
        {
            out = (3.0f / 2.0f) * -lowerLim * (slope * input - pow(slope * input, 3.0f) / 3.0f) / 2.0f + (lowerLim / 2.0f);
        }
    }
    
    return out;
}






