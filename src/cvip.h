#ifndef CVIP_H
#define CVIP_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

namespace cvip {

    namespace interpolation {

        template<typename T>
        cv::Mat_<T> bilinear(cv::Mat_<T> &src, double width, double height) {

            cv::Mat_<T> dst = cv::Mat_<T>(height, width);
            double x_ratio = static_cast<double>(src.cols) / dst.cols;
            double y_ratio = static_cast<double>(src.rows) / dst.rows;

            for(int row = 0; row != dst.rows; row++) {
                int y = static_cast<int>(row * y_ratio);

                auto *dst_ptr = &dst(row, 0)[0]; // dst copy

                double y_diff = (row * y_ratio) - y; // Y axis nearest pixel
                double y_diff_2 = 1 - y_diff;

                for(int col = 0; col != dst.cols; col++) {
                    int x = static_cast<int>(col * x_ratio); // X axis nearest pixel

                    double x_diff = (col * x_ratio) - x;
                    double x_diff_2 = 1 - x_diff;

                    // for neighboring pixels
                    double y2_cross_x2 = y_diff_2 * x_diff_2;
                    double y2_cross_x = y_diff_2 * x_diff;
                    double y_cross_x2 = y_diff * x_diff_2;
                    double y_cross_x = y_diff * x_diff;

                    for(int channel = 0; channel != cv::DataType<T>::channels; channel++) {
                        *dst_ptr++ = y2_cross_x2 * src(y, x)[channel] + y2_cross_x * src(y, x + 1)[channel] +
                                     y_cross_x2 * src(y + 1, x)[channel] + y_cross_x * src(y + 1, x + 1)[channel];
                    }
                }
            }

            return dst;
        }

        template<typename T>
        cv::Mat_<T> nearest_neighbor(cv::Mat_<T> &src, double width, double height) {

            cv::Mat_<T> dst = cv::Mat_<T>(height, width);
            double x_ratio = static_cast<double>(src.cols) / dst.cols;
            double y_ratio = static_cast<double>(src.rows) / dst.rows;

            for (int row = 0; row != dst.rows; row++) {
                for (int col = 0; col != dst.cols; col++)
                {
                    cv::Point2d point(floor(col * x_ratio), floor(row * y_ratio));
                    dst((row * dst.cols) + col) = src((point.y * src.cols) + point.x);
                }
            }

            return dst;
        }
    }
}

#endif // CVIP_H
