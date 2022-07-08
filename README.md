# facile-http

facile-http use [libcurl](https://github.com/curl/curl), is a libcurl wrapper, only http

## install build before

ubuntu

```sh
sudo apt install clang lld cmake ninja-build
```

ninja >= 1.10.1

cmake >= 3.18.0

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

## use facile-http

```cpp
facile_http::HttpClient client;

auto response = client.send(facile_http::Request::Builder().url("https://bing.com").GET().build());

std::cout << "status code: " << response->status_code() \
    << " message: " << response->status_message() << std::endl;

std::cout << "response: " << response->body() << std::endl;


// download file
response = client.download(facile_http::Request::Builder().url("https://speed.hetzner.de/100MB.bin").build(), "./100MB.bin");

std::cout << "status code: " << response->status_code() \
    << " message: " << response->status_message() << std::endl;
```

