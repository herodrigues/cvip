#ifndef CVIP_H
#define CVIP_H

#include <opencv2/opencv.hpp>
#include <iostream>

namespace cvip {

    template<typename T>
    cv::Mat_<T> const bilinear_interpolation(cv::Mat_<T> const &src, cv::Size dsize, float dx, float dy) {

        cv::Mat_<T> dst = dsize.area() == 0 ? cv::Mat_<T>(src.rows * dy, src.cols * dx) : cv::Mat_<T>(dsize);

        float const x_ratio = static_cast<float>((src.cols - 1)) / dst.cols;
        float const y_ratio = static_cast<float>((src.rows - 1)) / dst.rows;

        for(int row = 0; row != dst.rows; ++row) {
            int y = static_cast<int>(row * y_ratio);
            float const y_diff = (row * y_ratio) - y; //distance of the nearest pixel(y axis)
            float const y_diff_2 = 1 - y_diff;
            auto *dst_ptr = &dst(row, 0)[0];

            for(int col = 0; col != dst.cols; ++col) {
                int x = static_cast<int>(col * x_ratio);
                float const x_diff = (col * x_ratio) - x; //distance of the nearet pixel(x axis)
                float const x_diff_2 = 1 - x_diff;
                float const y2_cross_x2 = y_diff_2 * x_diff_2;
                float const y2_cross_x = y_diff_2 * x_diff;
                float const y_cross_x2 = y_diff * x_diff_2;
                float const y_cross_x = y_diff * x_diff;

                for(int channel = 0; channel != cv::DataType<T>::channels; ++channel) {
                    *dst_ptr++ = y2_cross_x2 * src(y, x)[channel] +
                                 y2_cross_x * src(y, x + 1)[channel] +
                                 y_cross_x2 * src(y + 1, x)[channel] +
                                 y_cross_x * src(y + 1, x + 1)[channel];
                }
            }
        }

        return dst;
    }
}

#endif // CVIP_H
