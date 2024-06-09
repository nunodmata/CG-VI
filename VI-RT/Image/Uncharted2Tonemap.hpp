#ifndef Uncharted2Tonemap_hpp
#define Uncharted2Tonemap_hpp

#include "../utils/RGB.hpp"

inline float Uncharted2TonemapCurve(float x) {
    const float A = 0.15f;
    const float B = 0.50f;
    const float C = 0.10f;
    const float D = 0.20f;
    const float E = 0.02f;
    const float F = 0.30f;

    return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

inline RGB Uncharted2Tonemap(RGB color) {
    // Apply the tone mapping curve to each channel
    float exposureBias = 2.0f;
    color = color * exposureBias;

    RGB mappedColor;
    mappedColor.R = Uncharted2TonemapCurve(color.R);
    mappedColor.G = Uncharted2TonemapCurve(color.G);
    mappedColor.B = Uncharted2TonemapCurve(color.B);

    // Normalize and map to 0-1 range
    float whiteScale = 1.0f / Uncharted2TonemapCurve(11.2f);
    mappedColor.R *= whiteScale;
    mappedColor.G *= whiteScale;
    mappedColor.B *= whiteScale;

    return mappedColor;
}

#endif /* Uncharted2Tonemap_hpp */
