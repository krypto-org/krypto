#include <krypto/config.h>

krypto::Config::Config(const std::string &file_path) : root_{} {
    std::ifstream file(file_path);
    root_ = nlohmann::json::parse(file);
    root_ = root_.flatten();
    KRYP_LOG(info, root_.dump(4));
}

const nlohmann::json &krypto::Config::get() const {
    return root_;
}
