#ifndef __COMMON_CRYPTO_HPP__
#define __COMMON_CRYPTO_HPP__

#include <cstdint>
#include <string>
#include <vector>
namespace VMPilot {
namespace Common {
namespace Crypto {

std::vector<uint8_t> Encrypt_AES_256_CBC_PKCS7(const std::vector<uint8_t>& data,
                                               const std::string& key) noexcept;

std::vector<uint8_t> Decrypt_AES_256_CBC_PKCS7(const std::vector<uint8_t>& data,
                                               const std::string& key) noexcept;

std::vector<uint8_t> SHA256(const std::vector<uint8_t>& data,
                            const std::vector<uint8_t>& salt) noexcept;
}  // namespace Crypto
}  // namespace Common
}  // namespace VMPilot
#endif