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

    enum {
        FLIP_HORIZONTAL,
        FLIP_VERTICAL
    };

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

        width = src_img.cols + (src_img.cols * (width/100));
        height = src_img.rows + (src_img.rows * (height/100));

        cv::Mat_<T> output = cv::Mat_<T>(width, height);
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

        width = src_img.cols + (src_img.cols * (width/100));
        height = src_img.rows + (src_img.rows * (height/100));

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

    /**
     * Aplica o espelhamento horizontal ou vertical
     *
     * @param src_img Referência para a matriz da imagem
     * @param flip_code Flag indicando qual flip será realizado (horizontal ou vertical)
     * @return Matriz resultante
     */
    template<typename T>
    cv::Mat_<T> flip(cv::Mat_<T> &src_img, int flip_code) {

        cv::Mat_<T> output = cv::Mat_<T>(src_img.rows, src_img.cols);
        int width = output.rows;
        int height = output.cols;

        switch(flip_code) {
            case 0: {
                for(int row = 0; row < width; row++)
                    for(int col = 0; col < height; col++)
                        output(row, col) = src_img(row, height - col - 1);
                break;
            }
            case 1: {
                for(int row = 0; row < width; row++)
                    for(int col = 0; col < height; col++)
                        output(row, col) = src_img(width - row - 1, col);
                break;
            }
            break;
        }

        return output;
    }

    /**
     * Calcula a distância euclidiana entr dois pontos
     *
     * @param x1 Coordenada x do ponto P1
     * @param y1 Coordenada y do ponto P1
     * @param x2 Coordenada x do ponto P2
     * @param y2 Coordenada x do ponto P2
     * @return Resultado da distância euclidiana
     */
    template<typename T>
    double euclidean_distance(T x1, T y1, T x2, T y2) {
        return sqrt( pow(x1-x2, 2) + pow(y1-y2, 2) );
    }
}

#endif // CVIP_H
