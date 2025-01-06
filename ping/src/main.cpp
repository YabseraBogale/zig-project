#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ip/icmp.hpp>
#include <boost/array.hpp>
#include <chrono>

using namespace boost::asio;
using namespace boost::asio::ip;

int main() {
    try {
        io_context io;
        icmp::resolver resolver(io);
        icmp::endpoint destination = *resolver.resolve(icmp::v4(), "8.8.8.8", "").begin();
        icmp::socket socket(io, icmp::v4());

        // Create an ICMP echo request packet
        std::string body = "Ping test";
        boost::array<unsigned char, 64> buffer;
        std::fill(buffer.begin(), buffer.end(), 0);

        // Set the ICMP header
        buffer[0] = 8; // Type: Echo Request
        buffer[1] = 0; // Code: No Code
        // Remaining fields: checksum, identifier, sequence number, and payload (to be filled)

        socket.send_to(boost::asio::buffer(buffer), destination);

        // Wait for a response
        icmp::endpoint sender;
        socket.receive_from(boost::asio::buffer(buffer), sender);

        std::cout << "Ping received from: " << sender.address().to_string() << std::endl;
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
