/*
  ==============================================================================

    Distortions.cpp
    Created: 24 Mar 2021 3:23:45pm
    Author:  Rodolfo Ortiz

  ==============================================================================
*/

#include "Distortions.h"

float Distortions::arcTanDistortion (float input, float gain)
{
    gain = gain + 1.0f;
    
    float out = (2.0f / juce::MathConstants<float>::pi) * atan(gain * input);
    
    out = out / log(gain);
    
    return out;
}

float Distortions::softClipper (float input, float gain)
{
    float newInput = input * (gain / 10);
    float out = 0.0;
    
    if (newInput >= 1.0f)
        out = 1.0f;
    else if ((newInput > -1) && (newInput < 1))
        out = (3.0f / 2.0f) * (newInput - (powf(newInput, 3.0f) / 3.0f));
    else if (newInput <= -1)
        out = -1.0f;
    
    out = out * 0.5f;
    
    return out;
}

float Distortions::sigmoid (float input, float gain)
{
    gain = gain + 1.0f;
    
    float out = (2.0f * (1.0f / (1.0f + expf(-gain * input)))) - 1;
    
    out = out / log(gain);
    
    return out;
}

float Distortions::hyperbolicTangent (float input, float gain)
{
    gain = gain + 1.0f;
    float out = (tanh(gain * input)) / (tanh(gain));
    
//    out = out/log(gain);
    out = out * 0.4f;
    return out;
}

float Distortions::diodeClipping (float input, float gain)
{
//    gain = gain + 1.0f;
    
    float diodeClippingAlgorithm = expf((0.1f * input) / (0.0253f * 1.68f)) - 1.0f;
    float out = 2 / juce::MathConstants<float>::pi * atan(diodeClippingAlgorithm * (gain * 16));
    
    out = out * 0.3f;
//    out = out/log(gain);
    
    return out;
}

float Distortions::fuzzExponential (float input, float gain)
{
    float newInput = input * gain;
    float out;
    
    //Soft clipping
    if (newInput < 0.0f)
        out = -1.0f *  (1.0f - expf(-abs(newInput)));
    else
        out = 1.0f * (1.0f - expf(-abs(newInput)));
 
    //Half Wave Rectifier
    out = 0.5f * (out + abs(out));
    
    out = out * 0.8f;
    
    return out;

}

float Distortions::pieceWiseOverdrive (float input, float gain)
{
    float newInput = input * (gain) ;
    float out = 0.0f;
    
    if (abs(newInput) <= 1.0f / 3.0f)
        out = 2.0f * newInput;
    else if (abs(newInput) > 2.0f / 3.0f)
    {
        if (newInput > 0.0f)
            out = newInput;
        if (newInput < 0.0f)
            out = -newInput;
    } else
    {
        if (newInput > 0.0f)
            out = (3.0f - powf((2.0f - newInput * 3.0f), 2.0f)) / 3.0f;
        if (newInput < 0.0f)
            out = -(3.0f - powf((2.0f - newInput * 3.0f), 2.0f)) / 3.0f;
    }
    
    out = (out / log(gain + 1.0f)) * 0.5f;
    
    return out;
}

float Distortions::tube (float input, float gain)
{
    float Q = -1.5f; //more negative = more linear
    float distortion = 5; //higher number = higher distortion
    float out;
    
    float newInput = input * (gain / 10);
    
    if (Q == 0)
    {
        out = newInput / (1 - expf(-distortion * newInput));
        if (newInput == Q)
        {
            out = 1 / distortion;
        }
    } else
    {
        out = ((newInput - Q) / (1 - expf(-distortion * (newInput - Q)))) + (Q / (1 - expf(distortion * Q)));
        if (newInput == Q)
        {
            out = (1 / distortion) + (Q / (1 - expf(distortion * Q)));
        }
    }
    
    out = out * 0.8f;
    
    return out;
}

float Distortions::arraya (float input, float gain)
{
    auto newInput = input * 1.0f;
    
    //Arraya
    auto out = ((3.0f * newInput) / 2.0f) * (1.0f - (pow(newInput, 2.0f) / 3.0f));
    
//    Fuzz Exponential
    if (out < 0.0f)
        out = 1.0f * ((1.0f - expf(abs(out)) / (expf(1.0f) - 1.0f)));
    else
        out = -1.0f * ((1.0f - expf(abs(out)) / (expf(1.0f) - 1.0f)));
    
    //Exponential 2
//    out = (expf(1.0f) - expf(1.0f - out)) / (expf(1.0f) - 1.0f);
    
//    out = 0.5f * (out + abs(out));
//    out = abs(out);
    
    if (gain >= 10.0f)
        out = out * (gain / 100.0f);
    else
        out = out * (0.1f);
    
    //Arraya
    out = ((3.0f * out) / 2.0f) * (1.0f - (powf(out, 2.0f) / 3.0f));
    out *= 0.8f;
    
    return out;
}

float Distortions::gallo (float input, float gain)
{
    float a = -0.01f;
    float b = 0.7f;
    float k1 = powf(a, 2.0f);
    float k2 = 1 + (2 * a);
    float k3 = powf(b, 2.0f);
    float k4 = 1 - (2 * b);
    float out_1 = 0.0f;
    
    auto newInput = input * gain;
    
    if (newInput < a)
        out_1 = (k1 + newInput) / (k2 - newInput);
    if (newInput >= a && newInput <= b)
        out_1 = newInput;
    if (newInput > b)
        out_1 = (newInput - k3) / (newInput + k4);
    
    return out_1;
}

float Distortions::doubleSoftClipper (float input, float gain)
{
    auto slope = 2.0f;
    auto upperLim = 0.8f;
    auto lowerLim = -1.0f;
    auto upperSkew = 1.0f;
    auto lowerSkew = 1.0f;
    auto xOffFactor = 0.0f;
    auto out = 0.0f;
    
    auto xOff = (1.0f / slope) * powf(slope, xOffFactor);
    
    input *= (gain / 10.0f);
    
    if (input > 0.0f)
    {
        input = (input - xOff) * upperSkew;
        
        if (input >= 1.0f / slope)
            out = upperLim;
        else if (input <= -1.0f / slope)
            out = 0.0f;
        else
            out = (3.0f / 2.0f) * upperLim * (slope * input - powf(slope * input, 3.0f) / 3.0f) / 2.0f + (upperLim / 2.0f);
    } else
    {
        input = (input + xOff) * lowerSkew;
        
        if (input >= 1.0f / slope)
            out = 0.0f;
        else if (input <= -1.0f / slope)
            out = lowerLim;
        else
            out = (3.0f / 2.0f) * -lowerLim * (slope * input - powf(slope * input, 3.0f) / 3.0f) / 2.0f + (lowerLim / 2.0f);
    }
    
    return out;
}

float Distortions::crush (float input, float gain)
{
    auto out = 0.0f;
    
    gain /= 100.0f;
    
    float dry = input;
    float wet = round(input * powf(2, gain));
    out = (wet + dry)  * asin(gain) + dry;
    
//    out = out * 0.3;
    
    return out;
}

float Distortions::tuboid (float input, float gain)
{
    auto ktp = 1.0f;
    auto ktn = 3.0f;
    auto sfn = 0.0f;
    
    auto threshPos = 0.3f;
    auto threshNeg = -0.7f;
    
    auto out = 0.0f;
    
    gain /= 10.0f;
    
    auto so = input * gain;
    
    if (so >= threshPos)
        sfn = ktp * powf(so - threshPos, 3.0f);
    else if (so <= threshNeg)
        sfn = -ktn * abs(powf(so - threshNeg, 3.0f));
    else
        sfn = 0.0f;
    
    so = (input - sfn) * gain;
    out = so;
    
    return out;
}

float Distortions::pakarinen_Yeh (float input, float gain)
{
    auto out = 0.0f;
    
    gain /= 100.0f;
    
    auto x = input * gain;
    
    if ((x >= 0.320018f) && (x <= 1.0f))
        out = 0.630035f;
    else if ((x >= -0.08905f) && (x < 0.320018))
        out = (-6.153f * powf(x, 2.0f)) + (3.9375f * x);
    else if ((x >= -1.0f) && (x < -0.08905f))
        out = (-0.75f * (1.0f - powf(1.0f - (abs(x) - 0.029f), 12.0f) + (0.333f * (abs(x) - 0.029f)))) + 0.01f;
    else
        out = -0.9818f;
    
    out *= 1.5f;
    
    return out;
}

