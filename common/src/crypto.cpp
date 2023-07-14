#include <crypto.hpp>

#include <botan/cipher_mode.h>
#include <botan/hash.h>
#include <botan/hex.h>
#include <instruction_t.hpp>

using Instruction_t = VMPilot::Common::Instruction_t;

std::vector<uint8_t> VMPilot::Common::Crypto::Encrypt_AES_256_CBC_PKCS7(
    const std::vector<uint8_t>& data, const std::string& key) noexcept {
    auto cipher = Botan::Cipher_Mode::create("AES-256/CBC/PKCS7",
                                             Botan::Cipher_Dir::Encryption);

    cipher->set_key(reinterpret_cast<const uint8_t*>(key.data()), key.size());
    cipher->start(reinterpret_cast<const uint8_t*>(data.data()), data.size());

    Botan::secure_vector<uint8_t> encrypted_data(data.size());
    cipher->finish(encrypted_data, encrypted_data.size());

    return *reinterpret_cast<std::vector<uint8_t>*>(&encrypted_data);
}

std::vector<uint8_t> VMPilot::Common::Crypto::Decrypt_AES_256_CBC_PKCS7(
    const std::vector<uint8_t>& data, const std::string& key) noexcept {

    // TODO: Use thread pool to speed up the decryption
    auto cipher = Botan::Cipher_Mode::create("AES-256/CBC/PKCS7",
                                             Botan::Cipher_Dir::Decryption);
    cipher->set_key(reinterpret_cast<const uint8_t*>(key.data()), key.size());
    cipher->start(reinterpret_cast<const uint8_t*>(data.data()), data.size());
    Botan::secure_vector<uint8_t> decrypted_data(data.size());
    cipher->finish(decrypted_data, decrypted_data.size());

    return *reinterpret_cast<std::vector<uint8_t>*>(&decrypted_data);
}

std::vector<uint8_t> VMPilot::Common::Crypto::SHA256(
    const std::vector<uint8_t>& data,
    const std::vector<uint8_t>& salt) noexcept {
    auto hash_fn = Botan::HashFunction::create("SHA-256");
    hash_fn->update(data);
    hash_fn->update(salt);
    auto hash_vec = hash_fn->final();

    return *reinterpret_cast<std::vector<uint8_t>*>(&hash_vec);
}