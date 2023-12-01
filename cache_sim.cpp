#include <iostream>
#include <fstream>
#include <vector>

struct CacheEntry {
    bool valid;
    int tag;
};

int extractTag(int address, int numBits) {
    return address >> numBits;
}

void cacheSimulator(int memSize, int cacheSize, const std::string& accessFilename) {
    std::vector<CacheEntry> cache(cacheSize, {false, 0});
    int numBlocks = memSize;
    int numCacheBlocks = cacheSize;
    int blockBits = 0;

    while ((1 << blockBits) < numBlocks) {
        blockBits++;
    }

    std::ifstream inputFile(accessFilename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file." << std::endl;
        return;
    }

    std::ofstream outputFile("hitmiss-out.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Error creating output file." << std::endl;
        return;
    }

    int hits = 0;
    int misses = 0;

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        int address;
        while (iss >> address) {
            int blockAddress = address / numBlocks;
            int tag = extractTag(blockAddress, blockBits);

            bool found = false;
            for (auto& entry : cache) {
                if (entry.valid && entry.tag == tag) {
                    outputFile << address << " Hit" << std::endl;
                    hits++;
                    found = true;
                    break;
                }
            }

            if (!found) {
                outputFile << address << " Miss" << std::endl;
                misses++;
            }
        }
    }

    int totalAccesses = hits + misses;
    double hitRate = (static_cast<double>(hits) / totalAccesses) * 100.0;
    double missRate = (static_cast<double>(misses) / totalAccesses) * 100.0;

    std::cout << "Out of " << totalAccesses << " memory accesses, there was a "
              << hitRate << "% cache hit rate and a " << missRate << "% miss rate." << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: ./cache_sim mem_size cache_size access_filename" << std::endl;
        return 1;
    }

    int memSize = std::stoi(argv[1]);
    int cacheSize = std::stoi(argv[2]);
    std::string accessFilename = argv[3];

    if (memSize % 2 != 0 || memSize % cacheSize != 0) {
        std::cerr << "Invalid memory or cache size." << std::endl;
        return 1;
    }

    cacheSimulator(memSize, cacheSize, accessFilename);

    return 0;
}
