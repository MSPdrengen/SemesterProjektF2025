#include <modbus/modbus.h>
#include <iostream>
#include <cstring>
#include <csignal>
#include <unistd.h>   // for close() og sleep()
#include <cstdlib>    // for exit()
#include <cerrno>     // for errno
#include <thread>     // for std::this_thread::sleep_for
#include <chrono>     // for std::chrono::milliseconds

modbus_t *ctx;
modbus_mapping_t *mb_mapping;
int server_socket = -1;
int previous_coil_value = -1;  // For at holde styr på den tidligere værdi af Coil[0]

void cleanup(int signum) {
    std::cout << "\nCleaning up and exiting...\n";
    if (server_socket != -1) close(server_socket);
    if (ctx) modbus_free(ctx);
    if (mb_mapping) modbus_mapping_free(mb_mapping);
    exit(0);
}

int main() {
    signal(SIGINT, cleanup);

    // Opret ny Modbus TCP server på port 502
    ctx = modbus_new_tcp("0.0.0.0", 502);  // Kører på alle IP'er og port 502
    if (ctx == nullptr) {
        std::cerr << "Unable to create Modbus context\n";
        return -1;
    }

    // Tildel én coil og ingen input/output registers
    mb_mapping = modbus_mapping_new(1, 0, 0, 0);
    if (mb_mapping == nullptr) {
        std::cerr << "Failed to allocate mapping: " << modbus_strerror(errno) << std::endl;
        modbus_free(ctx);
        return -1;
    }

    // Opret socket
    server_socket = modbus_tcp_listen(ctx, 1);
    if (server_socket == -1) {
        std::cerr << "Unable to listen on port: " << modbus_strerror(errno) << std::endl;
        modbus_free(ctx);
        modbus_mapping_free(mb_mapping);
        return -1;
    }

    std::cout << "✅ Modbus TCP Server is running on port 502...\n";

    // Acceptér forbindelser og håndtér forespørgsler
    modbus_tcp_accept(ctx, &server_socket);

    // Skift til en ventetilstand, hvor vi kun reagerer på forespørgsler
    while (true) {
        uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
        int rc = modbus_receive(ctx, query);

        if (rc > 0) {
            // Der er en aktiv forespørgsel, så send svar
            modbus_reply(ctx, query, rc, mb_mapping);

            // Tjek om værdien af Coil[0] er ændret
            if (mb_mapping->tab_bits[0] != previous_coil_value) {
                std::cout << "📥 Coil[0] er nu: " << (int)mb_mapping->tab_bits[0] << std::endl;
                previous_coil_value = mb_mapping->tab_bits[0];  // Opdater tidligere værdi
            }
        } else if (rc == -1) {
            // Hvis der ikke er nogen forespørgsel (RC=-1), så gør vi ikke noget
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Vent i 100 ms
        }
    }

    cleanup(0);
    return 0;
}
