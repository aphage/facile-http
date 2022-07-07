# facile-http

facile-http use [libcurl](https://github.com/curl/curl)

## build

```sh
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/example/example
```

## install

```sh
cmake --install build
```