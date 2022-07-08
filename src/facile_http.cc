#include "facile_http.h"

#include <fstream>
#include <iostream>

#include <curl/curl.h>

namespace facile_http {

static size_t write_callback(char* buffer, size_t size, size_t nmemb, void* userp) {
    ((std::ostream*)userp)->write(buffer, size * nmemb);
    return size * nmemb;
}

static size_t read_callback(char* buffer, size_t size, size_t nmemb, void* userp) {
    ((std::istream*)userp)->read(buffer, size * nmemb);
    return size * nmemb;
}

static int seek_callback(void* userp, curl_off_t offset, int origin) {
    ((std::istream*)userp)->seekg(offset, std::ios_base::seekdir(origin));
    return 0;
}

static struct curl_slist* make_header(const std::unordered_map<std::string, std::string>& headers, struct curl_slist *header = nullptr) {
    for (auto& h : headers) {
        header = curl_slist_append(header, (h.first + ": " + h.second).c_str());
    }
    return header;
}

static std::unordered_map<std::string, std::string> parse_header(const std::string& header) {
    std::unordered_map<std::string, std::string> headers;
    std::stringstream ss(header);
    std::string line;

    std::getline(ss, line);
    {
        std::stringstream ss2(line);
        std::string http_version, status_code, status_message;
        std::getline(ss2, http_version, ' ');
        std::getline(ss2, status_code, ' ');
        std::getline(ss2, status_message);
        headers["http-version"] = http_version;
        headers["status-code"] = status_code;
        headers["status-message"] = status_message;
    }

    while (std::getline(ss, line, '\n')) {
        if(line.length() == 1) {
            break;
        }
        std::stringstream ss2(line);
        std::string key, value;
        std::getline(ss2, key, ':');
        std::getline(ss2, value, '\r');
        headers[key] = value;
    }
    return headers;
}

class HttpClient::Foo {
public:
    CURL* curl = nullptr;
    const std::string version = "1.0.0";
};

HttpClient::HttpClient() {
    foo = std::make_unique<Foo>();
    auto connect_timeout = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::seconds(5));
    foo->curl = curl_easy_init();

    if(foo->curl == nullptr) {
        throw std::runtime_error("curl_easy_init() failed");
    }

    curl_easy_setopt(foo->curl, CURLOPT_CONNECTTIMEOUT_MS, connect_timeout.count());
    curl_easy_setopt(foo->curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(foo->curl, CURLOPT_NOSIGNAL, 1L);
}

HttpClient::~HttpClient() {
    curl_easy_cleanup(foo->curl);
}

HttpClient::HttpClient(HttpClient&&) = default;
HttpClient& HttpClient::operator=(HttpClient&&) = default;

void HttpClient::connect_timeout(const std::chrono::milliseconds& timeout) {
    curl_easy_setopt(foo->curl, CURLOPT_CONNECTTIMEOUT_MS, timeout.count());
}

void HttpClient::ssl_verify_peer(bool verify) {
    curl_easy_setopt(foo->curl, CURLOPT_SSL_VERIFYPEER, verify ? 1 : 0);
}

void HttpClient::ssl_verify_host(bool verify) {
    curl_easy_setopt(foo->curl, CURLOPT_SSL_VERIFYHOST, verify ? 2 : 0);
}

void HttpClient::ssl_cert(const std::string& cert) {
    curl_easy_setopt(foo->curl, CURLOPT_SSLCERT, cert.c_str());
}

void HttpClient::ssl_cert(const std::vector<std::uint8_t>& cert) {
    struct curl_blob blob;
    blob.data = const_cast<std::uint8_t*>(cert.data());
    blob.len = cert.size();
    blob.flags = CURL_BLOB_COPY;
    curl_easy_setopt(foo->curl, CURLOPT_SSLCERT_BLOB, &blob);
}

void HttpClient::ssl_cert_type(const std::string& cert_type) {
    curl_easy_setopt(foo->curl, CURLOPT_SSLCERTTYPE, cert_type.c_str());
}

void HttpClient::ssl_key(const std::string& key) {
    curl_easy_setopt(foo->curl, CURLOPT_SSLKEY, key.c_str());
}

void HttpClient::ssl_key(const std::vector<std::uint8_t>& key) {
    struct curl_blob blob;
    blob.data = const_cast<std::uint8_t*>(key.data());
    blob.len = key.size();
    blob.flags = CURL_BLOB_COPY;
    curl_easy_setopt(foo->curl, CURLOPT_SSLKEY_BLOB, &blob);
}

void HttpClient::ssl_key_type(const std::string& key_type) {
    curl_easy_setopt(foo->curl, CURLOPT_SSLKEYTYPE, key_type.c_str());
}

void HttpClient::ssl_key_password(const std::string& key_password) {
    curl_easy_setopt(foo->curl, CURLOPT_KEYPASSWD, key_password.c_str());
}

void HttpClient::ssl_cainfo(const std::string& cainfo) {
    curl_easy_setopt(foo->curl, CURLOPT_CAINFO, cainfo.c_str());
}

void HttpClient::ssl_cainfo(const std::vector<std::uint8_t>& cainfo) {
    struct curl_blob blob;
    blob.data = const_cast<std::uint8_t*>(cainfo.data());
    blob.len = cainfo.size();
    blob.flags = CURL_BLOB_COPY;
    curl_easy_setopt(foo->curl, CURLOPT_CAINFO_BLOB, &blob);
}

void HttpClient::ssl_cadir(const std::string& cadir) {
    curl_easy_setopt(foo->curl, CURLOPT_CAPATH, cadir.c_str());
}

void HttpClient::ssl_crl(const std::string& crl) {
    curl_easy_setopt(foo->curl, CURLOPT_CRLFILE, crl.c_str());
}

void HttpClient::ssl_proxy_verify_peer(bool verify) {
    curl_easy_setopt(foo->curl, CURLOPT_PROXY_SSL_VERIFYPEER, verify ? 1 : 0);
}

void HttpClient::ssl_proxy_verify_host(bool verify) {
    curl_easy_setopt(foo->curl, CURLOPT_PROXY_SSL_VERIFYHOST, verify ? 2 : 0);
}

void HttpClient::ssl_proxy_cert(const std::string& cert) {
    curl_easy_setopt(foo->curl, CURLOPT_PROXY_SSLCERT, cert.c_str());
}

void HttpClient::ssl_proxy_cert(const std::vector<std::uint8_t>& cert) {
    struct curl_blob blob;
    blob.data = const_cast<std::uint8_t*>(cert.data());
    blob.len = cert.size();
    blob.flags = CURL_BLOB_COPY;
    curl_easy_setopt(foo->curl, CURLOPT_PROXY_SSLCERT_BLOB, &blob);
}

void HttpClient::ssl_proxy_cert_type(const std::string& cert_type) {
    curl_easy_setopt(foo->curl, CURLOPT_PROXY_SSLCERTTYPE, cert_type.c_str());
}

void HttpClient::ssl_proxy_key(const std::string& key) {
    curl_easy_setopt(foo->curl, CURLOPT_PROXY_SSLKEY, key.c_str());
}

void HttpClient::ssl_proxy_key(const std::vector<std::uint8_t>& key) {
    struct curl_blob blob;
    blob.data = const_cast<std::uint8_t*>(key.data());
    blob.len = key.size();
    blob.flags = CURL_BLOB_COPY;
    curl_easy_setopt(foo->curl, CURLOPT_PROXY_SSLKEY_BLOB, &blob);
}

void HttpClient::ssl_proxy_key_type(const std::string& key_type) {
    curl_easy_setopt(foo->curl, CURLOPT_PROXY_SSLKEYTYPE, key_type.c_str());
}

void HttpClient::ssl_proxy_key_password(const std::string& key_password) {
    curl_easy_setopt(foo->curl, CURLOPT_PROXY_KEYPASSWD, key_password.c_str());
}

void HttpClient::ssl_proxy_cainfo(const std::string& cainfo) {
    curl_easy_setopt(foo->curl, CURLOPT_PROXY_CAINFO, cainfo.c_str());
}

void HttpClient::ssl_proxy_cainfo(const std::vector<std::uint8_t>& cainfo) {
    struct curl_blob blob;
    blob.data = const_cast<std::uint8_t*>(cainfo.data());
    blob.len = cainfo.size();
    blob.flags = CURL_BLOB_COPY;
    curl_easy_setopt(foo->curl, CURLOPT_PROXY_CAINFO_BLOB, &blob);
}

void HttpClient::ssl_proxy_cadir(const std::string& cadir) {
    curl_easy_setopt(foo->curl, CURLOPT_PROXY_CAPATH, cadir.c_str());
}

void HttpClient::ssl_proxy_crl(const std::string& crl) {
    curl_easy_setopt(foo->curl, CURLOPT_PROXY_CRLFILE, crl.c_str());
}

void HttpClient::user_agent(const std::string& user_agent) {
    curl_easy_setopt(foo->curl, CURLOPT_USERAGENT, user_agent.c_str());
}

void HttpClient::proxy(const std::string& proxy) {
    curl_easy_setopt(foo->curl, CURLOPT_PROXY, proxy.c_str());
}

void HttpClient::dns_server(const std::vector<std::string>& dns_servers) {
    if(dns_servers.empty()) {
        return;
    }
    std::string dns_servers_str;
    for(const auto& dns_server : dns_servers) {
        dns_servers_str += dns_server + ",";
    }
    dns_servers_str.pop_back();
    curl_easy_setopt(foo->curl, CURLOPT_DNS_SERVERS, dns_servers_str.c_str());
}

void HttpClient::dns_server(const std::string& dns_server) {
    curl_easy_setopt(foo->curl, CURLOPT_DNS_SERVERS, dns_server.c_str());
}

void HttpClient::verbose(bool verbose) {
    curl_easy_setopt(foo->curl, CURLOPT_VERBOSE, verbose ? 1 : 0);
}

void HttpClient::tcp_fastopen(bool tcp_fastopen) {
    curl_easy_setopt(foo->curl, CURLOPT_TCP_FASTOPEN, tcp_fastopen ? 1 : 0);
}

void HttpClient::tcp_no_delay(bool tcp_no_delay) {
    curl_easy_setopt(foo->curl, CURLOPT_TCP_NODELAY, tcp_no_delay ? 1 : 0);
}

void HttpClient::tcp_keepalive(bool keepalive) {
    curl_easy_setopt(foo->curl, CURLOPT_TCP_KEEPALIVE, keepalive ? 1 : 0);
}

void HttpClient::tcp_keepalive_idle(const std::chrono::seconds& idle) {
    curl_easy_setopt(foo->curl, CURLOPT_TCP_KEEPIDLE, (long)idle.count());
}

void HttpClient::tcp_keepalive_interval(const std::chrono::seconds& interval) {
    curl_easy_setopt(foo->curl, CURLOPT_TCP_KEEPINTVL, (long)interval.count());
}

void HttpClient::fllow_redirects(bool follow_redirects) {
    curl_easy_setopt(foo->curl, CURLOPT_FOLLOWLOCATION, follow_redirects ? 1 : 0);
}

void HttpClient::max_redirects(int max_redirects) {
    curl_easy_setopt(foo->curl, CURLOPT_MAXREDIRS, max_redirects);
}

void HttpClient::upload_buffer_size(std::uint32_t size) {
    curl_easy_setopt(foo->curl, CURLOPT_UPLOAD_BUFFERSIZE, size);
}

void HttpClient::buffer_size(std::uint32_t size) {
    curl_easy_setopt(foo->curl, CURLOPT_BUFFERSIZE, size);
}

const std::string& HttpClient::version() {
    return foo->version;
}

std::unique_ptr<Response> HttpClient::send(const Request& request) {
    auto response_header = std::make_unique<std::stringstream>();
    auto response_body = std::make_unique<std::stringstream>();

    curl_easy_setopt(foo->curl, CURLOPT_URL, request.url().c_str());
    curl_easy_setopt(foo->curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(foo->curl, CURLOPT_WRITEDATA, response_body.get());
    curl_easy_setopt(foo->curl, CURLOPT_HEADERFUNCTION, write_callback);
    curl_easy_setopt(foo->curl, CURLOPT_HEADERDATA, response_header.get());
    curl_easy_setopt(foo->curl, CURLOPT_TIMEOUT_MS, request.timeout().count());

    auto header = make_header(request.headers(), nullptr);
    curl_easy_setopt(foo->curl, CURLOPT_HTTPHEADER, header);

    if(request.method() == "GET") {
        curl_easy_setopt(foo->curl, CURLOPT_HTTPGET, 1L);
    } else if(request.method() == "POST") {
        curl_easy_setopt(foo->curl, CURLOPT_POST, 1L);
        curl_easy_setopt(foo->curl, CURLOPT_POSTFIELDS, request.body().c_str());
        curl_easy_setopt(foo->curl, CURLOPT_POSTFIELDSIZE_LARGE, request.body().size());
    } else {
        curl_easy_setopt(foo->curl, CURLOPT_CUSTOMREQUEST, request.method().c_str());
        curl_easy_setopt(foo->curl, CURLOPT_POSTFIELDS, request.body().c_str());
        curl_easy_setopt(foo->curl, CURLOPT_POSTFIELDSIZE_LARGE, request.body().size());
    }

    CURLcode res = curl_easy_perform(foo->curl);
    curl_slist_free_all(header);
    if(res != CURLE_OK) {
        return std::make_unique<Response>(Response::Builder().status_code(0).status_message(std::to_string(res) + ", " + curl_easy_strerror(res)).build());
    }

    long status_code = 0;
    curl_easy_getinfo(foo->curl, CURLINFO_RESPONSE_CODE, &status_code);
    auto headers = parse_header(response_header->str());

    return std::make_unique<Response>(Response::Builder().status_code(status_code).status_message(headers["status-message"]).headers(headers).body(response_body->str()).build());
}

std::unique_ptr<Response> HttpClient::download(const Request& request, const std::string& filename) {
    auto response_header = std::make_unique<std::stringstream>();
    auto response_body = std::make_unique<std::fstream>(filename, std::ios::binary | std::ios::in | std::ios::out);
    
    response_body->seekg(0, std::ios::end);
    auto file_size = response_body->tellg();
    response_body->seekg(0, std::ios::beg);
    if(file_size > 0) {
        curl_easy_setopt(foo->curl, CURLOPT_RESUME_FROM_LARGE, file_size);
        response_body->seekp(file_size, std::ios::beg);
    } else {
        response_body = std::make_unique<std::fstream>(filename, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
    }

    curl_easy_setopt(foo->curl, CURLOPT_URL, request.url().c_str());
    curl_easy_setopt(foo->curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(foo->curl, CURLOPT_WRITEDATA, response_body.get());
    curl_easy_setopt(foo->curl, CURLOPT_HEADERFUNCTION, write_callback);
    curl_easy_setopt(foo->curl, CURLOPT_HEADERDATA, response_header.get());

    auto header = make_header(request.headers(), nullptr);
    curl_easy_setopt(foo->curl, CURLOPT_HTTPHEADER, header);

    auto res = curl_easy_perform(foo->curl);
    curl_slist_free_all(header);
    if(res != CURLE_OK) {
        return std::make_unique<Response>(Response::Builder().status_code(0).status_message(std::to_string(res) + ", " + curl_easy_strerror(res)).build());
    }

    long status_code = 0;
    curl_easy_getinfo(foo->curl, CURLINFO_RESPONSE_CODE, &status_code);
    auto headers = parse_header(response_header->str());

    return std::make_unique<Response>(Response::Builder().status_code(status_code).status_message(headers["status-message"]).headers(headers).build());
}

std::unique_ptr<Response> HttpClient::upload(const Request& request, const std::string& filename) {
    auto response_header = std::make_unique<std::stringstream>();
    auto response_body = std::make_unique<std::stringstream>();
    auto request_body = std::make_unique<std::fstream>(filename, std::ios::binary | std::ios::in);

    request_body->seekg(0, std::ios::end);
    auto file_size = request_body->tellg();
    request_body->seekg(0, std::ios::beg);

    curl_easy_setopt(foo->curl, CURLOPT_URL, request.url().c_str());
    curl_easy_setopt(foo->curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(foo->curl, CURLOPT_WRITEDATA, response_body.get());
    curl_easy_setopt(foo->curl, CURLOPT_HEADERFUNCTION, write_callback);
    curl_easy_setopt(foo->curl, CURLOPT_HEADERDATA, response_header.get());
    
    auto header = make_header(request.headers(), nullptr);
    curl_easy_setopt(foo->curl, CURLOPT_HTTPHEADER, header);
    
    curl_easy_setopt(foo->curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(foo->curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(foo->curl, CURLOPT_READDATA, request_body.get());
    curl_easy_setopt(foo->curl, CURLOPT_INFILESIZE_LARGE, file_size);
    curl_easy_setopt(foo->curl, CURLOPT_SEEKFUNCTION, seek_callback);
    curl_easy_setopt(foo->curl, CURLOPT_SEEKDATA, request_body.get());
    
    auto res = curl_easy_perform(foo->curl);
    curl_slist_free_all(header);
    if(res != CURLE_OK) {
        return std::make_unique<Response>(Response::Builder().status_code(0).status_message(std::to_string(res) + ", " + curl_easy_strerror(res)).build());
    }
    
    long status_code = 0;
    curl_easy_getinfo(foo->curl, CURLINFO_RESPONSE_CODE, &status_code);
    auto headers = parse_header(response_header->str());

    return std::make_unique<Response>(Response::Builder().status_code(status_code).status_message(headers["status-message"]).headers(headers).body(response_body->str()).build());
}

} // namespace facile_http