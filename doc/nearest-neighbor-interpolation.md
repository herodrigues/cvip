Interpolação por vizinho mais próximo
---------------------

Neste algoritmo, foi utilizada uma técnica que permite tanto a ampliação quando a redução de uma imagem. Para isso, é preciso 
calcular a proporção nos eixos X e Y do tamanho da imagem original pelo tamanho da imagem resultante.

- src_img: Imagem original
- output: Imagem resultante
```c++
double ratio_x = static_cast<double>(src_img.cols) / output.cols;
double ratio_y = static_cast<double>(src_img.rows) / output.rows;
```

O método de interpolação por vizinho mais próximo requer apenas dois loops percorrendo linhas e colunas da matriz
```c++
for (int row = 0; row < output.rows; row++) {
    for (int col = 0; col < output.cols; col++)
    {
        cv::Point2d point;
        point.x = static_cast<int>(col * ratio_x);
        point.y = static_cast<int>(row * ratio_y);
        output((row * output.cols) + col) = src_img((point.y * src_img.cols) + point.x);
    }
}
```

Onde ```c++ output((row * output.cols) + col) = src_img((point.y * src_img.cols) + point.x); ``` é o trecho de código responsável 
pela interpolação.
Onde a cada iteração, o ponto da matriz resultante na coordenada onde valor da coluna atual multiplicado pelo comprimento do tamanho 
desejado da imagem somado à coluna atual receberá o ponto do eixo Y acima do ponto a ser interpolado somado ao ponto do eixo X.

Graficamente, temos:

<img src="https://raw.githubusercontent.com/herodrigues/cvip/master/img/nearest_neighbour_chart.png" width="200">

Abaixo temos uma imagem ampliada e reduzida utilizando interpolação por vizinho mais próximo.

|  Original (100x100)  |  Reduzida (50x50)  | Ampliada (500x500) |
| :---: | :-----: | :---: |
| ![](https://raw.githubusercontent.com/herodrigues/cvip/master/img/tux.jpg) |  ![](https://raw.githubusercontent.com/herodrigues/cvip/master/img/tux-nearest-50.jpg) | ![](https://raw.githubusercontent.com/herodrigues/cvip/master/img/tux-nearest-500.jpg)  |
