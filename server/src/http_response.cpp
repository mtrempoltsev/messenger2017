#include "http_response.h"

namespace m2 {
namespace server {

HttpResponse::HttpResponse() {
}

void HttpResponse::setData(const std::string &data, int code) {
  data_ = data;
  changeHeader(code);
}

std::string HttpResponse::toString() const {
  return header_ + data_;
}

void HttpResponse::changeHeader(int code) {
  if (code == 200) {
    header_ =
        "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: " + std::to_string(data_.size()) + "\r\n"
            "Connection: keep-alive\r\n"
            "\r\n";
  } else if (code == 404) {
    header_ =
        "HTTP/1.0 404 Not Found\r\n"
            "Content-Length: " + std::to_string(data_.size()) + "\r\n"
            "\r\n";
  } else if (code == 403) {
    header_ =
        "HTTP/1.0 403 403 Forbidden\r\n"
            "Content-Length: " + std::to_string(data_.size()) + "\r\n"
            "\r\n";
  }
}
}
}
