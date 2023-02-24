#include <iostream>
#include <string>
#include <sstream>
#include <openssl/sha.h>             //update

class Block {
public:
    int index;
    std::string timestamp;
    std::string data;
    std::string previousHash;
    std::string hash;

    Block(int idx, std::string ts, std::string d, std::string ph) {
        index = idx;
        timestamp = ts;
        data = d;
        previousHash = ph;
        hash = calculateHash();
    }

private:
    std::string calculateHash() {                                                   //need to  -> update zkp 
        std::stringstream ss;
        ss << index << timestamp << data << previousHash;
        std::string input = ss.str();

        unsigned char hash[SHA256_DIGEST_LENGTH];                                    //Sha256 x merkel Tree (update)
        SHA256((const unsigned char*)input.c_str(), input.length(), hash);

        std::stringstream s;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            s << std::hex << (int)hash[i];
        }
        return s.str();
    }
};

 


class Blockchain {
public:
    std::vector<Block> blocks;

    Blockchain() {
        // Create the genesis block
        Block genesisBlock(0, "01/01/2023", "LFNBlockNode", "0");
        blocks.push_back(genesisBlock);
    }

    void addBlock(std::string data) {
        int index = blocks.size();
        std::string previousHash = blocks[index - 1].hash;
        std::string timestamp = getTimeStamp();
        Block newBlock(index, timestamp, data, previousHash);
        blocks.push_back(newBlock);
    }

private:
    std::string getTimeStamp() {
                                                  //  current timestamp --> update
        return "timestamp";
    }
};
                                                                   