Operações aritméticas
---------------------

- Adição
```c++
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
```

- Subtração
```c++
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
```
