
#include "krypto/root_certificates.h"
#include "krypto/http.h"
#include "krypto/logger.h"
#include <boost/lexical_cast.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "Host and Port Not Provided\n";
        return EXIT_FAILURE;
    }
    auto const host = argv[1];
    auto const port = argv[2];

    krypto::HttpClient client{host, boost::lexical_cast<int16_t >(port)};
    auto response = client.get("/product");

    if (response.has_value()) {
        KRYP_LOG(debug, response.value());
    }
}