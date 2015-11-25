Distância
---------------------

Neste algoritmo, foi utilizada uma técnica que permite tanto a ampliação quando a redução de uma imagem. Para isso, é preciso 
calcular a proporção nos eixos X e Y do tamanho da imagem original pelo tamanho da imagem resultante.

- src_img: Imagem original
- output: Imagem resultante
```c++
double ratio_x = static_cast<double>(src_img.cols) / output.cols;
double ratio_y = static_cast<double>(src_img.rows) / output.rows;
```

Uma conversão de valores é necessário nesse trecho já que as propriedades cols e rows do tipo Mat (Matriz) da biblioteca 
OpenCV retorna valores inteiros.

Dentro do primeiro loop que irá percorrer as linhas da matriz, escolhe-se um ponto P(x,y) que será o ponto a ser interpolado. 
A coordenada y desse ponto receberá o valor da coluna atual multiplicado pela proporção do eixo Y calculada anteriormente.
```c++
for(int row = 0; row < output.rows; row++) {
    cv::Point2d point;
    point.y = static_cast<int>(row * ratio_y);
    // ...
}
```
Em seguida, calcula-se a diferença da coordenada Y em relação ao ponto P.
```c++
double y_diff = (row * ratio_y) - point.y;
double y_diff2 = 1 - y_diff;
```

Analogamente, a coordenada x do ponto P será calculada dentro do loop que irá percorrer as colunas da matriz, assim 
como as diferenças em relação ao eixo X do ponto P.
```c++
for(int col = 0; col < output.cols; col++) {
    point.x = static_cast<int>(col * ratio_x);
    double x_diff = (col * ratio_x) - point.x;
    double x_diff2 = 1 - x_diff;
    // ...
```
E agora, os vizinhos do ponto P:
```c++
double x1_y1 = y_diff * x_diff;
double x1_y2 = y_diff2 * x_diff;
double x2_y1 = y_diff * x_diff2;
double x2_y2 = y_diff2 * x_diff2;
```
Graficamente, temos a seguinte situação:

|    |   x1  | x |  y1   |
| --- | ----- | --- | ----- |
| y1 | x1_y1 |   | x2_y1 |
|  y |       | P |       |
| y2 | x1_y2 |   | x2_y2 |  

Agora só é necessário aplicar a equação da interpolação bilinear para cada linha.

![](https://raw.githubusercontent.com/herodrigues/cvip/master/img/bilinear_interpolation_equation.png)

Onde Q11 = x1_y1, Q12 = x1_y2, Q21 = x2_y1 e Q22 = x2_y2

O ponteiro temp_ptr guadra uma referência de cada linha da matriz resultante. No trecho abaixo, é calculado o ponto
a ser interpolado em cada linha e coluna da imagem:
```c++
*temp_ptr++ = x1_y1 * src_img(point.y + 1, point.x + 1) + x1_y2 * src_img(point.y, point.x + 1) +
              x2_y1 * src_img(point.y + 1, point.x) + x2_y2 * src_img(point.y, point.x);
```              

Abaixo temos uma imagem ampliada e reduzida utilizando interpolação bilinar.

|  Original (100x100)  |  Reduzida (50x50)  | Ampliada (500x500) |
| :---: | :-----: | :---: |
| ![](https://raw.githubusercontent.com/herodrigues/cvip/master/img/tux.jpg) |  ![](https://raw.githubusercontent.com/herodrigues/cvip/master/img/tux-bilinear-50.jpg) | ![](https://raw.githubusercontent.com/herodrigues/cvip/master/img/tux-bilinear-500.jpg)  |
