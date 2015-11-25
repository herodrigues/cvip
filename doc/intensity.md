Funções de intensidade
---------------------

- Contrast stretch
```c++
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
```
