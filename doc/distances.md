Distâncias
---------------------

- Distância euclidiana
```c++
template<typename T>
inline T euclidean_distance(T x1, T y1, T x2, T y2) {
    return sqrt( pow(x1-x2, 2) + pow(y1-y2, 2) );
}
```

- Distância D4 (City-Block)
```c++
template<typename T>
inline T city_block_distance(T x1, T y1, T x2, T y2) {
    return abs(x1-x2) + abs(y1-y2);
}
```

- Distância 8 (Chessboard)
```c++
template<typename T>
inline T chessboard_distance(T x1, T y1, T x2, T y2) {
    return std::max(abs(x1-x2), abs(y1-y2));
}

```
