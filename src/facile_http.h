#pragma once

#include "request.h"
#include "response.h"

namespace facile_http {

class HttpClient {
private:
    class Foo;
public:
    HttpClient();
    virtual ~HttpClient();

    HttpClient(const HttpClient&) = delete;
    HttpClient& operator=(const HttpClient&) = delete;
    
    HttpClient(HttpClient&&);
    HttpClient& operator=(HttpClient&&);

    void connect_timeout(const std::chrono::milliseconds& timeout);
    void ssl_verify_peer(bool verify);
    void ssl_verify_host(bool verify);
    void ssl_cert(const std::string& cert);
    void ssl_cert(const std::vector<std::uint8_t>& cert);
    void ssl_cert_type(const std::string& cert_type);
    void ssl_key(const std::string& key);
    void ssl_key(const std::vector<std::uint8_t>& key);
    void ssl_key_type(const std::string& key_type);
    void ssl_key_password(const std::string& password);
    void ssl_cainfo(const std::string& cainfo);
    void ssl_cainfo(const std::vector<std::uint8_t>& cainfo);
    void ssl_cadir(const std::string& cadir);
    void ssl_crl(const std::string& crl);
    void ssl_proxy_verify_peer(bool verify);
    void ssl_proxy_verify_host(bool verify);
    void ssl_proxy_cert(const std::string& cert);
    void ssl_proxy_cert(const std::vector<std::uint8_t>& cert);
    void ssl_proxy_cert_type(const std::string& cert_type);
    void ssl_proxy_key(const std::string& key);
    void ssl_proxy_key(const std::vector<std::uint8_t>& key);
    void ssl_proxy_key_type(const std::string& key_type);
    void ssl_proxy_key_password(const std::string& password);
    void ssl_proxy_cainfo(const std::string& cainfo);
    void ssl_proxy_cainfo(const std::vector<std::uint8_t>& cainfo);
    void ssl_proxy_cadir(const std::string& cadir);
    void ssl_proxy_crl(const std::string& crl);
    void user_agent(const std::string& user_agent);
    void proxy(const std::string& proxy);
    void dns_server(const std::vector<std::string>& dns_servers);
    void dns_server(const std::string& dns_server);
    void verbose(bool verbose);
    void tcp_fastopen(bool tcp_fastopen);
    void tcp_no_delay(bool tcp_no_delay);
    void tcp_keepalive(bool keepalive);
    void tcp_keepalive_idle(const std::chrono::seconds& idle);
    void tcp_keepalive_interval(const std::chrono::seconds& interval);
    void fllow_redirects(bool follow);
    void max_redirects(std::int32_t max_redirects);
    void upload_buffer_size(std::uint32_t size);
    void buffer_size(std::uint32_t size);
    const std::string& version();
    
    std::unique_ptr<Response> send(const Request& request);
    std::unique_ptr<Response> download(const Request& request, const std::string& filename);
    std::unique_ptr<Response> upload(const Request& request, const std::string& filename);

private:
    std::unique_ptr<Foo> foo;
};

} // namespace facile_http