#include <iostream>
#include <string>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include <openssl/rand.h>
#include <openssl/ripemd.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

// Function to generate a new private key
EC_KEY* generate_private_key() {
    EC_KEY *key = EC_KEY_new_by_curve_name(NID_secp256k1);
    if (key == nullptr) {
        std::cerr << "Error generating private key" << std::endl;
        return nullptr;
    }

    if (!EC_KEY_generate_key(key)) {
        std::cerr << "Error generating private key" << std::endl;
        EC_KEY_free(key);
        return nullptr;
    }

    return key;
}

// Function to extract public key from private key
EC_POINT* extract_public_key(EC_KEY* key) {
    const EC_GROUP *group = EC_KEY_get0_group(key);
    const EC_POINT *pub_key = EC_KEY_get0_public_key(key);
    EC_POINT *copy_pub_key = EC_POINT_new(group);

    if (!EC_POINT_copy(copy_pub_key, pub_key)) {
        std::cerr << "Error extracting public key" << std::endl;
        EC_POINT_free(copy_pub_key);
        return nullptr;
    }

    return copy_pub_key;
}

// Function to encode hexadecimal data to base58
std::string base58_encode(const unsigned char* input, size_t length) {
    static const char* base58_chars = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

    std::string result;
    result.reserve(length * 137 / 100 + 1);

    unsigned char buffer[SHA256_DIGEST_LENGTH];
    SHA256(SHA256(input, length, buffer), SHA256_DIGEST_LENGTH, buffer);

    const unsigned char* pbegin = buffer;
    const unsigned char* pend = buffer + SHA256_DIGEST_LENGTH;
    while (pbegin != pend) {
        unsigned carry = *pbegin;
        for (std::string::reverse_iterator it = result.rbegin(); it != result.rend(); ++it) {
            carry += 58 * (*it - '0');
            *it = base58_chars[carry % 256];
            carry /= 256;
        }

        while (carry > 0) {
            result.insert(result.begin(), base58_chars[carry % 256]);
            carry /= 256;
        }

        ++pbegin;
    }

    return result;
}

int main() {
    // Generate a new private key
    EC_KEY* private_key = generate_private_key();
    if (private_key == nullptr) {
        return 1;
    }

    // Extract the public key from the private key
    EC_POINT* public_key = extract_public_key(private_key);
    if (public_key == nullptr) {
        EC_KEY_free(private_key);
        return 1;
    }

    // Convert the public key to hexadecimal
    unsigned char* public_key_hex = nullptr;
    int public_key_hex_length = i2o_ECPublicKey(public_key, &public_key_hex);
    if (public_key_hex_length == 0) {
        std::cerr << "Error converting public key to hexadecimal" << std::endl;
        EC_POINT_free(public_key);
        EC_KEY_free(private_key);
        return 1;
    }

    // Encode the public key in base58
    std::string encoded_public_key = base58_encode


//wallet id public to BlockChain Header add 