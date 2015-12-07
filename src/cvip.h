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
     * Translada a imagem
     *
     * @param src_img Referência para a matriz da imagem
     * @param x Nova coordenada x da origem da imagem
     * @param y Nova coordenada y da origem da imagem
     * @return Matriz resultante
     */
    template<typename T>
    cv::Mat_<T> translate(cv::Mat_<T> &src_img, int x, int y) {

        cv::Mat_<T> output = cv::Mat_<T>::zeros(src_img.rows + x, src_img.cols + y);
        int width = src_img.rows;
        int height = src_img.cols;

        for(int row = 0; row < width; row++)
            for(int col = 0; col < height; col++)
                output(row + x, col + y) = src_img(row, col);

        return output;
    }

    /**
     * Adiciona uma imagem a outra
     *
     * @param src_img Referência para a matriz da imagem
     * @param tmp_img Referência para a matriz que será adicionada a src_img
     * @return Matriz resultante
     */
    template<typename T>
    cv::Mat_<T> addition(cv::Mat_<T> &src_img, cv::Mat_<T> &tmp_img) {

        cv::Mat_<T> output = cv::Mat_<T>(src_img.rows, src_img.cols);
        int width = src_img.rows;
        int height = src_img.cols;


        for(int row = 0; row < width; row++)
            for(int col = 0; col < height; col++)
                output(row, col) = (src_img(row, col) + tmp_img(row, col)) / 2;

        return output;
    }

    /**
     * Altera o contraste de uma imagem
     *
     * @param src_img Referência para a matriz da imagem
     * @param r1 Coordenada r1 do ponto P(r1, r2) que controla o formato da transformação
     * @param r2 Coordenada r2 do ponto P(r1, r2) que controla o formato da transformação
     * @param s1 Coordenada s1 do ponto P(s1, s2) que controla o formato da transformação
     * @param s2 Coordenada s2 do ponto P(s1, s2) que controla o formato da transformação
     * @return Matriz resultante
     */
    template<typename T>
    cv::Mat_<T> contrast_stretch(cv::Mat_<T> &src_img, float r1, float r2, float s1, float s2) {

        cv::Mat_<T> output = src_img.clone();
        int width = output.rows;
        int height = output.cols;
        float result;

        for(int row = 0; row < width; row++) {
            for(int col = 0; col < height; col++) {
                if(row >= 0 && row <= r1) {
                    result = s1/r1 * row;
                } else if(r1 < row && row <= r2) {
                    result = ((s2 - s1) / (r2 - r1)) * (row - r1) + s1;
                } else if(r2 < row && row <= 255) {
                    result = ((255 - s2) / (255 - r2)) * (row - r2) + s2;
                }
                output(col, row) = cv::saturate_cast<uchar>(int(result));
            }
        }
        return output;
    }

    /**
     * Subtrai uma imagem de outra
     *
     * @param src_img Referência para a matriz da imagem
     * @param tmp_img Referência para a matriz que será subtraída de src_img
     * @return Matriz resultante
     */
    template<typename T>
    cv::Mat_<T> subtraction(cv::Mat_<T> &src_img, cv::Mat_<T> &tmp_img) {

        cv::Mat_<T> output = cv::Mat_<T>(src_img.rows, src_img.cols);
        int width = src_img.rows;
        int height = src_img.cols;


        for(int row = 0; row < width; row++)
            for(int col = 0; col < height; col++)
                output(row, col) = src_img(row, col) - tmp_img(row, col);

        return output;
    }

    /**
     * Calcula a distância euclidiana entre dois pontos
     *
     * @param x1 Coordenada x do ponto P1
     * @param y1 Coordenada y do ponto P1
     * @param x2 Coordenada x do ponto P2
     * @param y2 Coordenada y do ponto P2
     * @return Resultado da distância euclidiana
     */
    template<typename T>
    inline T euclidean_distance(T x1, T y1, T x2, T y2) {
        return sqrt( pow(x1-x2, 2) + pow(y1-y2, 2) );
    }

    /**
     * Calcula a distância D4 (City-Block)
     *
     * @param x1 Coordenada x do ponto P1
     * @param y1 Coordenada y do ponto P1
     * @param x2 Coordenada x do ponto P2
     * @param y2 Coordenada y do ponto P2
     * @return Resultado da distância D4
     */
    template<typename T>
    inline T city_block_distance(T x1, T y1, T x2, T y2) {
        return abs(x1-x2) + abs(y1-y2);
    }

    /**
     * Calcula a distância D8 (Chessboard)
     *
     * @param x1 Coordenada x do ponto P1
     * @param y1 Coordenada y do ponto P1
     * @param x2 Coordenada x do ponto P2
     * @param y2 Coordenada y do ponto P2
     * @return Resultado da distância D8
     */
    template<typename T>
    inline T chessboard_distance(T x1, T y1, T x2, T y2) {
        return std::max(abs(x1-x2), abs(y1-y2));
    }

    /**
     * Conta o número de pixels para cada valor de intensidade
     *
     * @param src_img Referência para a matriz da imagem
     * @return Matriz resultante
     */
    template<typename T>
    const std::vector<int> histogram(cv::Mat_<T> &src_img) {
        std::vector<int> hist(255, 0);

        for(int row = 0; row < src_img.rows; row++)
            for(int col = 0; col < src_img.cols; col++)
                hist[(int) src_img(row, col)[0]]++;

        return hist;
    }

    /**
     * Gera frequência cumulativa do histograma
     *
     * @param hist Histograma da imagem
     * @return Frequência cumulativa
     */
    template<typename T>
    T frequency(T &hist) {
        T freq(255, 0);
        freq[0] = hist[0];

        for(int i = 1; i < 256; i++)
            freq[i] = hist[i] + freq[i-1];

        return freq;
    }

    /**
     * Exibe histograma
     *
     * @param hist Histograma da imagem
     * @return Frequência cumulativa
     */
    template<typename T>
    void show_histogram(T hist, std::string title) {
        int width = 512;
        int height = 400;
        int ratio = cvRound((double) width / 256);

        cv::Mat hist_image(height, width, CV_8UC1, cv::Scalar(255, 255, 255));

        int max = *std::max_element(hist.begin(), hist.end());

        for(int i = 0; i < 256; i++)
            hist[i] = ((double) hist[i] / max) * hist_image.rows;

        for(int i = 0; i < 256; i++)
            cv::line(hist_image, cv::Point(ratio * i, height), cv::Point(ratio * i, height - hist[i]),
                     cv::Scalar(0,0,0), 1, 8, 0);

        cv::imshow(title, hist_image);
    }

    /**
     * Calcula o filtro da média
     *
     * @param src_img Referência para a matriz da imagem
     * @return Matriz resultante
     */
    template<typename T>
    cv::Mat_<T> median_filter(cv::Mat_<T> &src_img) {
        cv::Mat_<T> dst_img = cv::Mat_<T>(src_img.rows + 1, src_img.cols + 1);

        for(int row = 1; row < dst_img.rows - 1; row++)
            for(int col = 1; col < dst_img.cols - 1; col++)
                dst_img(row, col) = src_img(row - 1, col - 1);

        std::vector<int> square(9, 0);

        for(int row = 1; row < dst_img.rows - 1; row++) {
            for(int col = 1; col < dst_img.cols - 1; col++) {

                square[0] = dst_img(row - 1 , col - 1)[0];
                square[1] = dst_img(row, col - 1)[0];
                square[2] = dst_img(row + 1, col - 1)[0];
                square[3] = dst_img(row - 1, col)[0];
                square[4] = dst_img(row, col)[0];
                square[5] = dst_img(row + 1, col)[0];
                square[6] = dst_img(row - 1, col + 1)[0];
                square[7] = dst_img(row, col + 1)[0];
                square[8] = src_img(row + 1, col + 1)[0];

                double median = std::accumulate(square.begin(), square.end(), 0) / 9.0;
                src_img(row, col)[0] = cvRound(median);
                src_img(row, col)[1] = cvRound(median);
                src_img(row, col)[2] = cvRound(median);
            }
        }

        return src_img;
    }
}

#endif // CVIP_H
