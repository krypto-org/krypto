#include <krypto/config.h>

int main(int argc, char ** argv) {
    if (argc == 3) {
        krypto::Config config{argv[1]};
        KRYP_LOG(info, config.at<std::string>(argv[2]));
        return 0;
    }
    KRYP_LOG(error, "3 Arguments required");
    return 1;
}