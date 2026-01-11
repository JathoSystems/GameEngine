#include "Network/Sockets/UdpNetworkSocket.h"

void UdpNetworkSocket::connect(std::string ip, std::string port) {
}

void UdpNetworkSocket::send(const Packet &packet) {
}

void UdpNetworkSocket::disconnect() {
}

void UdpNetworkSocket::asyncReceive(std::function<void(const Packet &)> callback, std::function<void()> onError) {
}

void UdpNetworkSocket::asyncSend(const Packet &packet, std::function<void(bool)> callback) {
}
