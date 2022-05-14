/*
  ==============================================================================

    KnobLookAndFeel.h
    Created: 13 Apr 2021 11:14:13pm
    Author:  Rodolfo Ortiz

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class KnobLookAndFeel : public LookAndFeel_V4 {
public:
    KnobLookAndFeel()
    {
        knobImage = ImageCache::getFromMemory(BinaryData::knob_png, BinaryData::knob_pngSize);
    }
    
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, [[maybe_unused]] float sliderPos,
                           [[maybe_unused]] const float rotaryStartAngle, [[maybe_unused]] const float rotaryEndAngle, juce::Slider& slider) override
    {
        const double rotation = (slider.getValue()
            - slider.getMinimum())
            / (slider.getMaximum()
                - slider.getMinimum());
        
        const int frames = knobImage.getHeight() / knobImage.getWidth();
        const int frameId = (int)ceil(rotation * ((double)frames - 3.0));
        const float radius = jmin(width / 2.0f, height / 2.0f);
        const float centerX = x + width * 0.5f;
        const float centerY = y + height * 0.5f;
        const float rx = centerX - radius - 1.0f;
        const float ry = centerY - radius;

        g.drawImage (knobImage,
            (int)rx,
            (int)ry,
            2 * (int)radius,
            2 * (int)radius,
            0,
            frameId * knobImage.getWidth(),
            knobImage.getWidth(),
            knobImage.getWidth());
    }
    
private:
    Image knobImage;
};
