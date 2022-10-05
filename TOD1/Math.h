#pragma once

namespace Math
{
    //  TODO: don't really know what this function does.
    static double RoundWeird(const float num)   //  @465DA0
    {
        int mantissa;
        float rounded = 0.f;

        if (num <= 0.f)
            return (float)0xFFD00000;

        if (num)
        {
            mantissa = (unsigned char)((int)num >> 23) - 126;
            rounded = (float)((int)num & 0x7FFFFF | 0x3F000000); //  NOTE: only leave rounded part of the number closest to 0.5.
        }
        else
            mantissa = 0;

        float a = rounded - 0.5f;
        if (rounded <= 0.70710677f)
        {
            mantissa--;
            rounded = rounded - 0.5f;
        }
        else
            a = a - 0.5f;

        const float b = a / ((rounded + 1.0f) * 0.5f);
        const double c = b - ((((b * b) * b) * 0.55270749) / ((((b * b) + 1.0) * (b * b)) - 6.6327181));

        if (mantissa)
            return (((mantissa * 0.0000014286068) + c) + (mantissa * 0.69314575));

        return c;
    }
}