#include "request.h"

#include <fstream>

namespace facile_http {

class Request::Foo {
public:
    std::string method = "GET";
    std::string url;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
    std::chrono::milliseconds timeout = std::chrono::milliseconds(0);
};

Request::Builder::Builder() {
    foo = std::make_unique<Foo>();
}

Request::Builder::~Builder() = default;

Request::Builder::Builder(const Builder& other) {
    foo = std::make_unique<Foo>(*other.foo);
}

Request::Builder& Request::Builder::operator=(const Builder& other) {
    foo = std::make_unique<Foo>(*other.foo);
    return *this;
}

Request::Builder& Request::Builder::method(const std::string& method) {
    foo->method = method;
    return *this;
}

Request::Builder& Request::Builder::method(const std::string& method, const std::string& body) {
    foo->method = method;
    foo->body = body;
    return *this;
}

Request::Builder& Request::Builder::url(const std::string& url) {
    foo->url = url;
    return *this;
}

Request::Builder& Request::Builder::header(const std::string& key, const std::string& value) {
    foo->headers[key] = value;
    return *this;
}

Request::Builder& Request::Builder::header(const std::string& key, const bool value) {
    foo->headers[key] = value ? "true" : "false";
    return *this;
}

Request::Builder& Request::Builder::header(const std::string& key, const std::int32_t value) {
    foo->headers[key] = std::to_string(value);
    return *this;
}

Request::Builder& Request::Builder::header(const std::string& key, const std::uint32_t value) {
    foo->headers[key] = std::to_string(value);
    return *this;
}

Request::Builder& Request::Builder::header(const std::string& key, const std::int64_t value) {
    foo->headers[key] = std::to_string(value);
    return *this;
}

Request::Builder& Request::Builder::header(const std::string& key, const std::uint64_t value) {
    foo->headers[key] = std::to_string(value);
    return *this;
}

Request::Builder& Request::Builder::header(const std::string& key, const float value) {
    foo->headers[key] = std::to_string(value);
    return *this;
}

Request::Builder& Request::Builder::header(const std::string& key, const double value) {
    foo->headers[key] = std::to_string(value);
    return *this;
}

Request::Builder& Request::Builder::headers(const std::unordered_map<std::string, std::string>& headers) {
    foo->headers = headers;
    return *this;
}

Request::Builder& Request::Builder::timeout(const std::chrono::milliseconds& timeout) {
    foo->timeout = timeout;
    return *this;
}

Request::Builder& Request::Builder::timeout(const std::chrono::seconds& timeout) {
    foo->timeout = std::chrono::duration_cast<std::chrono::milliseconds>(timeout);
    return *this;
}

Request::Builder& Request::Builder::GET() {
    foo->method = "GET";
    return *this;
}

Request::Builder& Request::Builder::POST(const std::string& body) {
    foo->method = "POST";
    foo->body = body;
    return *this;
}

Request::Builder& Request::Builder::PUT(const std::string& body) {
    foo->method = "PUT";
    foo->body = body;
    return *this;
}

Request::Builder& Request::Builder::DELETE() {
    foo->method = "DELETE";
    return *this;
}

Request Request::Builder::build() {
    return Request(*this);
}

Request::Request(const Builder& builder) {
    foo = std::make_unique<Foo>(*builder.foo);
}

Request::~Request() = default;

Request::Request(const Request& other) {
    foo = std::make_unique<Foo>(*other.foo);
}

Request& Request::operator=(const Request& other) {
    foo = std::make_unique<Foo>(*other.foo);
    return *this;
}

const std::string& Request::method() const {
    return foo->method;
}

const std::string& Request::url() const {
    return foo->url;
}

const std::unordered_map<std::string, std::string>& Request::headers() const {
    return foo->headers;
}

const std::string& Request::body() const {
    return foo->body;
}

const std::chrono::milliseconds& Request::timeout() const {
    return foo->timeout;
}

} // namespace facile_http