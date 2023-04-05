#include <cstring>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "dvs_to_udp.hpp"

using std::string;
using std::vector;

Socket::Socket(const string &port, const string &IP)
{
  struct addrinfo hints, *servinfo;
  int rv;

  memset(&hints, 0, sizeof hints);
  hints.ai_family =
      AF_INET; // set to AF_INET to use IPv4, to AF_INET6 to use IPv6
  hints.ai_socktype = SOCK_DGRAM;

  if (IP == "localhost")
    hints.ai_flags = AI_PASSIVE; // if IP adress not specified, use own IP

  if ((rv = getaddrinfo(IP.c_str(), port.c_str(), &hints,
                        &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    throw "Error raised";
  }

  // loop through all the results and make a socket
  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("talker: socket");
      continue;
    }

    break;
  }

  if (p == NULL) {
    fprintf(stderr, "talker: failed to create socket\n");
    throw "Error raised";
  }
}

Socket::Socket(Socket && s)
: sockfd{s.sockfd},
p{s.p}
{
  s.sockfd = -1;
}

Socket::~Socket()
{
  if (sockfd != -1) close(sockfd);
}

void Socket::send(const std::vector<uint32_t> &message, size_t num_elements) const
{
  if (sendto(sockfd, &message[0], sizeof(message[0]) * num_elements, 0, p->ai_addr,
                          p->ai_addrlen) == -1) {
    perror("talker error: sendto");
    exit(1);
  }
}

// Constructor - initialize socket
template <typename T>
DVSToUDP<T>::DVSToUDP(uint32_t bfsize, const string &port, const string &IP, const string &port2, const string &IP2, bool rotate180, int input_width, int input_height, int scale_x, int scale_y)
  : buffer_size{bfsize},
  rotate180{rotate180},
  input_width{input_width},
  input_height{input_height},
  scale_x{scale_x},
  scale_y{scale_y}
{
  sockets.emplace_back(port, IP);
  if (!IP2.empty()) sockets.emplace_back(port2, IP2);
}

// Process a packet of events and send it using UDP over the socket
template <typename T>
void DVSToUDP<T>::stream(Generator<T> &input_generator,
                         bool include_timestamp) {
  uint64_t events_sent = 0;
  bool sent;
  int current_event = 0;

  const int event_size = include_timestamp ? 8 : 4;

  const uint64_t max_events = UDP_max_bytesize / event_size;

  vector<vector<uint32_t>> messages(sockets.size());
  for (auto &m: messages) m.resize(max_events);

  uint64_t count = 0;

  for (AER::Event event : input_generator) {
    count += 1;
    sent = false;

    if (rotate180)
    {
      event.x = input_width - event.x;
      event.y = input_height - event.y;
    }

    if (scale_x) event.x /= scale_x;
    if (scale_y) event.y /= scale_y;

    // Encoding according to protocol
    if (include_timestamp) {
      messages[0][current_event] =
          (event.x & 0x7FFF)
          << 16; // Be aware that for machine-independance it should be:
                 // htons(polarity_event.x & 0x7FFF);
      messages[0][current_event + 1] = event.timestamp;
    } else {
      messages[0][current_event] =
          (event.x | 0x8000)
          << 16; // Be aware that for machine-independance it should be:
                 // htons(polarity_event.x | 0x8000);
    }

    if (event.polarity) {
      messages[0][current_event] |=
          event.y | 0x8000; // Be aware that for machine-independance it
                            // should be: htons(polarity_event.y | 0x8000);
    } else {
      messages[0][current_event] |=
          event.y & 0x7FFF; // Be aware that for machine-independance it
                            // should be: htons(polarity_event.y & 0x7FFF);
    }

    if (messages.size() == 2)
    {
      messages[1][current_event] = (((event.x << 1) + event.polarity) << 16) | 0x80000000 | event.y;
    }

    if (include_timestamp) {
      current_event += 2;
    } else {
      current_event += 1;
    }

    if (current_event == max_events) {
      for (size_t i = 0, num = sockets.size(); i < num; ++i)
      {
        sockets[i].send(messages[i], current_event);
      }

      sent = true;
      current_event = 0;
      events_sent += max_events;
    }
  }

  printf("Sent a total of %lu events\n", count);

  if (sent == false) {
      for (size_t i = 0, num = sockets.size(); i < num; ++i)
      {
        sockets[i].send(messages[i], current_event);
      }
    events_sent += current_event;
  }
}

template class DVSToUDP<AER::Event>;