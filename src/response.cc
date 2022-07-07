#include "response.h"

#include <fstream>

namespace facile_http {

class Response::Foo {
public:
    std::uint32_t status_code = 200;
    std::string status_message = "OK";
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

Response::Builder::Builder() {
    foo = std::make_unique<Foo>();
}

Response::Builder::~Builder() = default;

Response::Builder::Builder(const Builder& other) {
    foo = std::make_unique<Foo>(*other.foo);
}

Response::Builder& Response::Builder::operator=(const Builder& other) {
    foo = std::make_unique<Foo>(*other.foo);
    return *this;
}

Response::Builder& Response::Builder::status_code(const std::uint32_t status_code) {
    foo->status_code = status_code;
    return *this;
}

Response::Builder& Response::Builder::status_message(const std::string& status_message) {
    foo->status_message = status_message;
    return *this;
}

Response::Builder& Response::Builder::header(const std::string& key, const std::string& value) {
    foo->headers[key] = value;
    return *this;
}

Response::Builder& Response::Builder::header(const std::string& key, const bool value) {
    foo->headers[key] = value ? "true" : "false";
    return *this;
}

Response::Builder& Response::Builder::header(const std::string& key, const std::int32_t value) {
    foo->headers[key] = std::to_string(value);
    return *this;
}

Response::Builder& Response::Builder::header(const std::string& key, const std::uint32_t value) {
    foo->headers[key] = std::to_string(value);
    return *this;
}

Response::Builder& Response::Builder::header(const std::string& key, const std::int64_t value) {
    foo->headers[key] = std::to_string(value);
    return *this;
}

Response::Builder& Response::Builder::header(const std::string& key, const std::uint64_t value) {
    foo->headers[key] = std::to_string(value);
    return *this;
}

Response::Builder& Response::Builder::header(const std::string& key, const float value) {
    foo->headers[key] = std::to_string(value);
    return *this;
}

Response::Builder& Response::Builder::header(const std::string& key, const double value) {
    foo->headers[key] = std::to_string(value);
    return *this;
}

Response::Builder& Response::Builder::headers(const std::unordered_map<std::string, std::string>& headers) {
    foo->headers = headers;
    return *this;
}

Response::Builder& Response::Builder::body(const std::string& body) {
    foo->body = body;
    return *this;
}

Response Response::Builder::build() {
    return Response(*this);
}

Response::Response(const Response::Builder& builder) {
    foo = std::make_unique<Foo>(*builder.foo);
}

Response::~Response() = default;

Response::Response(const Response& other) {
    foo = std::make_unique<Foo>(*other.foo);
}

Response& Response::operator=(const Response& other) {
    foo = std::make_unique<Foo>(*other.foo);
    return *this;
}

const std::uint32_t Response::status_code() const {
    return foo->status_code;
}

const std::string& Response::status_message() const {
    return foo->status_message;
}

const std::unordered_map<std::string, std::string>& Response::headers() const {
    return foo->headers;
}

const std::string& Response::body() const {
    return foo->body;
}

} // namespace facile_http