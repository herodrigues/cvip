Transformações geométricas
---------------------

- Espelhamento e reflexão
```c++
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
```

- Translação
```c++
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
```
