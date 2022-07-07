#pragma once

#include <sstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <chrono>

namespace facile_http {

class Request {
private:
    class Foo;
public:
    class Builder {
    public:
        Builder();
        virtual ~Builder();
        Builder(const Builder&);
        Builder& operator=(const Builder&);

        Builder& method(const std::string& method);
        Builder& method(const std::string& method, const std::string& body);
        Builder& url(const std::string& url);
        Builder& header(const std::string& key, const std::string& value);
        Builder& header(const std::string& key, const bool value);
        Builder& header(const std::string& key, const std::int32_t value);
        Builder& header(const std::string& key, const std::uint32_t value);
        Builder& header(const std::string& key, const std::int64_t value);
        Builder& header(const std::string& key, const std::uint64_t value);
        Builder& header(const std::string& key, const float value);
        Builder& header(const std::string& key, const double value);
        Builder& headers(const std::unordered_map<std::string, std::string>& headers);
        Builder& timeout(const std::chrono::milliseconds& timeout);
        Builder& timeout(const std::chrono::seconds& timeout);
        Builder& GET();
        Builder& POST(const std::string& body);
        Builder& PUT(const std::string& body);
        Builder& DELETE();
        Request build();
    
    private:
        std::unique_ptr<Foo> foo;
        friend class Request;
    };

    virtual ~Request();
    Request(const Request&);
    Request& operator=(const Request&);

    const std::string& method() const;
    const std::string& url() const;
    const std::unordered_map<std::string, std::string>& headers() const;
    const std::string& body() const;
    const std::chrono::milliseconds& timeout() const;

private:
    Request(const Builder& builder);
    
    std::unique_ptr<Foo> foo;
};

} // namespace facile_http