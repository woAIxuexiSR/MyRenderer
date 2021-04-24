#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <cmath>
#include "math/vector.hpp"

/*
* P1 Bitmap ASCII
* P2 Graymap ASCII
* P3 Pixmap ASCII
* P4 Bitmap Binary
* P5 Graymap Binary
* P6 Pixmap Binary
*/
enum class MAGIC_NUM { P1, P2, P3, P4, P5, P6 };

class FrameBuffer
{
private:
    int width, height;
    color* data;

public:
    FrameBuffer(): width(0), height(0), data(nullptr) {}
    FrameBuffer(int _w, int _h): width(_w), height(_h)
    {
        data = new color[_w * _h];
    }
    ~FrameBuffer() { delete[] data; }

    color get_pixel(int row, int col) const { return data[row * width + col]; }
    void set_pixel(int row, int col, color c) { data[row * width + col] = c; }

    void output(const std::string& filePath, bool gamma_correction = true, MAGIC_NUM type = MAGIC_NUM::P3) const
    {
        std::ofstream f(filePath, std::ios::out);

        if(!f.is_open())
        {
            std::cout << "Failed to open file" << filePath << std::endl;
            exit(-1);
        }

        f << 'P' << (char)('1' + (int)type) << std::endl;
        f << width << ' ' << height << std::endl << ((int)(type) % 3 == 0 ? "1\n" : "255\n");

        double correction = 1.0;
        if(gamma_correction) correction = 1.0 / 2.2;

        for(int i = height - 1; i >= 0; --i)
            for(int j = 0; j < width; ++j)
            {
                //std::cout << "\rScanlines remaining: " << j << ' ' << std::flush;

                color c = get_pixel(i, j);
                if(gamma_correction) c = c.gamma_correction(correction);
                c = c * 255.0;

                f << (int)c.x << ' ' << (int)c.y << ' ' << (int)c.z << '\n';
            }
        std::cout << "Done.\n";
        
        f.close();
    }
};