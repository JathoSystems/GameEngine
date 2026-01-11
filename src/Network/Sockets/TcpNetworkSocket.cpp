#include "Network/Sockets/TcpNetworkSocket.h"
#include <iostream>

void TcpNetworkSocket::connect(std::string ip, std::string port) {
    endpoints = resolver.resolve(ip, port);
    try {
        asio::connect(socket, endpoints);
        std::cout << "Client successfully connected to " << ip << ":" << port << "\n";
    } catch (std::exception &e) {
        std::cerr << "Failed to connect: " << e.what() << "\n";
        throw;
    }
}

void TcpNetworkSocket::send(const Packet &packet) {
    try {
        const_cast<Packet &>(packet).serialize();
        const std::vector<uint8_t> &buffer = packet.getBuffer().getData();

        uint32_t bufferSize = static_cast<uint32_t>(buffer.size());
        std::vector<uint8_t> sizeHeader(4);
        sizeHeader[0] = (bufferSize >> 24) & 0xFF;
        sizeHeader[1] = (bufferSize >> 16) & 0xFF;
        sizeHeader[2] = (bufferSize >> 8) & 0xFF;
        sizeHeader[3] = bufferSize & 0xFF;

        asio::write(socket, asio::buffer(sizeHeader));
        asio::write(socket, asio::buffer(buffer));
    } catch (std::exception &e) {
        std::cerr << "Error sending packet: " << e.what() << "\n";
        throw;
    }
}

void TcpNetworkSocket::disconnect() {
    try {
        if (socket.is_open()) {
            asio::error_code ec;
            socket.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
            socket.close(ec);
            std::cout << "Socket disconnected\n";
        }
    } catch (std::exception &e) {
        std::cerr << "Error during disconnect: " << e.what() << "\n";
    }
}

void TcpNetworkSocket::asyncReceive(std::function<void(const Packet &)> callback, std::function<void()> onError) {
    auto sizeBuffer = std::make_shared<std::vector<uint8_t> >(4);

    asio::async_read(socket, asio::buffer(*sizeBuffer),
                     [this, sizeBuffer, callback, onError](const asio::error_code &ec, std::size_t bytes_transferred) {
                         if (ec) {
                             if (ec == asio::error::eof) {
                                 std::cout << "Client disconnected (EOF)\n";
                             } else if (ec == asio::error::connection_reset) {
                                 std::cout << "Client disconnected (connection reset)\n";
                             } else if (ec == asio::error::operation_aborted) {
                                 std::cout << "Socket disconnected\n";
                             } else {
                                 std::cerr << "Error reading packet size: " << ec.message() << "\n";
                             }

                             if (onError) onError();
                             return;
                         }

                         uint32_t packetSize = (static_cast<uint32_t>((*sizeBuffer)[0]) << 24) |
                                               (static_cast<uint32_t>((*sizeBuffer)[1]) << 16) |
                                               (static_cast<uint32_t>((*sizeBuffer)[2]) << 8) |
                                               static_cast<uint32_t>((*sizeBuffer)[3]);

                         auto dataBuffer = std::make_shared<std::vector<uint8_t> >(packetSize);

                         asio::async_read(socket, asio::buffer(*dataBuffer),
                                          [this, dataBuffer, callback, onError](
                                      const asio::error_code &ec2, std::size_t) {
                                              if (ec2) {
                                                  if (ec2 == asio::error::operation_aborted) {
                                                  } else {
                                                      std::cerr << "Error reading packet data: " << ec2.message() <<
                                                              "\n";
                                                  }
                                                  if (onError) onError();
                                                  return;
                                              }

                                              try {
                                                  auto packet = Packet::createFromBuffer(*dataBuffer);
                                                  if (packet) {
                                                      callback(*packet);
                                                  }
                                                  asyncReceive(callback, onError);
                                              } catch (std::exception &e) {
                                                  std::cerr << "Error deserializing packet: " << e.what() << "\n";
                                                  asyncReceive(callback, onError);
                                              }
                                          });
                     });
}

void TcpNetworkSocket::asyncSend(const Packet &packet, std::function<void(bool)> callback) {
    try {
        const_cast<Packet &>(packet).serialize();
        auto buffer = std::make_shared<std::vector<uint8_t> >(
            packet.getBuffer().getData()
        );

        uint32_t bufferSize = static_cast<uint32_t>(buffer->size());
        auto sizeHeader = std::make_shared<std::vector<uint8_t> >(4);
        (*sizeHeader)[0] = (bufferSize >> 24) & 0xFF;
        (*sizeHeader)[1] = (bufferSize >> 16) & 0xFF;
        (*sizeHeader)[2] = (bufferSize >> 8) & 0xFF;
        (*sizeHeader)[3] = bufferSize & 0xFF;

        asio::async_write(socket, asio::buffer(*sizeHeader),
                          [this, buffer, sizeHeader, callback](const asio::error_code &ec, std::size_t) {
                              if (ec) {
                                  std::cerr << "Error sending packet size: " << ec.message() << "\n";
                                  callback(false);
                                  return;
                              }

                              asio::async_write(socket, asio::buffer(*buffer),
                                                [callback, buffer](const asio::error_code &ec2, std::size_t) {
                                                    if (ec2) {
                                                        std::cerr << "Error sending packet data: " << ec2.message() <<
                                                                "\n";
                                                        callback(false);
                                                        return;
                                                    }
                                                    callback(true);
                                                });
                          });
    } catch (std::exception &e) {
        std::cerr << "Error in asyncSend: " << e.what() << "\n";
        callback(false);
    }
}

void TcpNetworkSocket::setSocket(asio::ip::tcp::socket &&newSocket) {
    this->socket = std::move(newSocket);
}
