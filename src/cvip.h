#ifndef CVIP_H
#define CVIP_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

/**
 * \namespace cvip
 * \brief cvip - Computer Vision Image Processing
 *
 * Este namespace possui diversos algoritmos básicos de processamento de imagens que
 * foram adicionados ao longo da disciplina CET653 - Processamento de Imagens durante
 * o semestre de 2015,1 no curso de Ciência da Computação (UFT)
 *
 * \author Herinson Rodrigues
 */
namespace cvip {

    namespace interpolation {

        /**
         * Aplica a interpolação bilinear na matriz de origem da imagem de
         * a matriz resultante
         *
         * @param src_img Referência para a matriz da imagem
         * @param width Novo comprimento da imagem
         * @param height Nova altura da imagem
         * @return Matriz resultante
         */
        template<typename T>
        cv::Mat_<T> bilinear(cv::Mat_<T> &src_img, double width, double height) {

            cv::Mat_<T> output = cv::Mat_<T>(height, width);
            double ratio_x = static_cast<double>(src_img.cols) / output.cols;
            double ratio_y = static_cast<double>(src_img.rows) / output.rows;

            for(int row = 0; row < output.rows; row++) {

                cv::Point2d point;
                point.y = static_cast<int>(row * ratio_y);

                double y_diff = (row * ratio_y) - point.y;
                double y_diff2 = 1 - y_diff;

                auto *temp_ptr = &output(row, 0);

                for(int col = 0; col < output.cols; col++) {
                    point.x = static_cast<int>(col * ratio_x);
                    double x_diff = (col * ratio_x) - point.x;
                    double x_diff2 = 1 - x_diff;

                    double x1_y1 = y_diff * x_diff;
                    double x1_y2 = y_diff2 * x_diff;
                    double x2_y1 = y_diff * x_diff2;
                    double x2_y2 = y_diff2 * x_diff2;

                    *temp_ptr++ = x1_y1 * src_img(point.y + 1, point.x + 1) + x1_y2 * src_img(point.y, point.x + 1) +
                                  x2_y1 * src_img(point.y + 1, point.x) + x2_y2 * src_img(point.y, point.x);
                }
            }

            return output;
        }

        /**
         * Aplica a interpolação por vizinho mais próximo e retorna a matriz resultante
         *
         * @param src_img Referência para a matriz da imagem
         * @param width Novo comprimento da imagem
         * @param height Nova altura da imagem
         * @return Matriz resultante
         */
        template<typename T>
        cv::Mat_<T> nearest_neighbor(cv::Mat_<T> &src_img, double width, double height) {

            cv::Mat_<T> output = cv::Mat_<T>(height, width);
            double ratio_x = static_cast<double>(src_img.cols) / output.cols;
            double ratio_y = static_cast<double>(src_img.rows) / output.rows;

            for (int row = 0; row < output.rows; row++) {
                for (int col = 0; col < output.cols; col++)
                {
                    cv::Point2d point;
                    point.x = static_cast<int>(col * ratio_x);
                    point.y = static_cast<int>(row * ratio_y);
                    output((row * output.cols) + col) = src_img((point.y * src_img.cols) + point.x);
                }
            }

            return output;
        }
    }
}

#endif // CVIP_H
