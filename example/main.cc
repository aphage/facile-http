#include <iostream>

#include <facile_http.h>

void test1() {
    facile_http::HttpClient client;
    auto response = client.send(facile_http::Request::Builder()
        .url("http://eu.httpbin.org/status/200")
        .PUT("Hello World!")
        .build());
    
    std::cout << "status code: " << response->status_code() \
        << " message: " << response->status_message() << std::endl;

    std::cout << "body: " << response->body() << std::endl;

    response = client.send(facile_http::Request::Builder()
        .url("http://eu.httpbin.org/status/200")
        .GET()
        .build());

    std::cout << "status code: " << response->status_code() \
        << " message: " << response->status_message() << std::endl;

    std::cout << "body: " << response->body() << std::endl;

    response = client.send(facile_http::Request::Builder()
        .url("http://eu.httpbin.org/status/200")
        .POST("")
        .build());
    
    std::cout << "status code: " << response->status_code() \
        << " message: " << response->status_message() << std::endl;
    
    std::cout << "body: " << response->body() << std::endl;

    response = client.send(facile_http::Request::Builder()
        .url("http://eu.httpbin.org/status/200")
        .DELETE()
        .build());
    
    std::cout << "status code: " << response->status_code() \
        << " message: " << response->status_message() << std::endl;
    
    std::cout << "body: " << response->body() << std::endl;
}

int main() {
    std::cout << "Hello World~" << std::endl;

    test1();

    facile_http::HttpClient client;

    auto response = client.send(facile_http::Request::Builder().url("https://bing.com").GET().build());
    
    std::cout << "status code: " << response->status_code() \
    << " message: " << response->status_message() << std::endl;

    // print headers
    for(auto& n:response->headers()) {
        std::cout << n.first << ": " << n.second << std::endl;
    }
    std::cout << std::endl;

    std::cout << "response: " << response->body() << std::endl;

    auto response2 = client.download(facile_http::Request::Builder().url("https://speed.hetzner.de/100MB.bin").build(), "./100MB.bin");

    std::cout << "status code: " << response2->status_code() \
    << " message: " << response2->status_message() << std::endl;

    // print headers
    for(auto& n:response2->headers()) {
        std::cout << n.first << ": " << n.second << std::endl;
    }
    std::cout << std::endl;

    std::cout << "response: " << response2->body() << std::endl;

    return 0;
}