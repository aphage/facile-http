#pragma once

#include <sstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace facile_http {

class Response {
private:
    class Foo;
public:
    class Builder {
    public:
        Builder();
        virtual ~Builder();
        Builder(const Builder&);
        Builder& operator=(const Builder&);

        Builder& status_code(const std::uint32_t status_code);
        Builder& status_message(const std::string& status_message);
        Builder& header(const std::string& key, const std::string& value);
        Builder& header(const std::string& key, const bool value);
        Builder& header(const std::string& key, const std::int32_t value);
        Builder& header(const std::string& key, const std::uint32_t value);
        Builder& header(const std::string& key, const std::int64_t value);
        Builder& header(const std::string& key, const std::uint64_t value);
        Builder& header(const std::string& key, const float value);
        Builder& header(const std::string& key, const double value);
        Builder& headers(const std::unordered_map<std::string, std::string>& headers);
        Builder& body(const std::string& body);
        Response build();
    
    private:
        std::unique_ptr<Foo> foo;
        friend class Response;
    };

    virtual ~Response();
    Response(const Response&);
    Response& operator=(const Response&);

    const std::uint32_t status_code() const;
    const std::string& status_message() const;
    const std::unordered_map<std::string, std::string>& headers() const;
    const std::string& body() const;

private:
    Response(const Builder& builder);

    std::unique_ptr<Foo> foo;
};

} // namespace facile_http