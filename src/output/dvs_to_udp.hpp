#pragma once

#include <string>
#include <vector>

// socket programming
#include <netdb.h>

#include "../aer.hpp"
#include "../generator.hpp"

class Socket
{
  int sockfd = -1;
  struct addrinfo *p = NULL;

public:
  Socket(const std::string &port, const std::string &IP);
  Socket(Socket && s);
  ~Socket();

  Socket(const Socket &) = delete;
  Socket &operator=(const Socket &) = delete;

  void send(const std::vector<uint32_t> &message, size_t num_bytes) const;
};

template <typename T> class DVSToUDP {
  uint32_t buffer_size;
  bool rotate180;
  int input_width, input_height;
  int scale_x, scale_y;

  static const uint16_t UDP_max_bytesize = 512;

  std::vector<Socket> sockets;

public:
  DVSToUDP(uint32_t bfsize, const std::string &port, const std::string &IP, const std::string &port2, const std::string &IP2, bool rotate180, int input_width, int input_height, int scale_x, int scale_y);

  void stream(Generator<T> &input_generator, bool include_timestamp);
};