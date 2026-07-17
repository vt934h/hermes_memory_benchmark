#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>
#include <signal.h>
#include <fstream>

using namespace std;

static volatile bool keepRunning = true;

void intHandler(int) {
    
    keepRunning = false;
}

std::string exec(const char* cmd) {
    
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

static array<char, 1024 * 10> buffer;
static string output;
static vector<uint32_t> samples;


static uint32_t extractValueFor(const char* name, bool single = false) {
    
    auto pos = output.find(name);
    if(pos == output.npos)
        return numeric_limits<uint32_t>::max();
    
    uint32_t result = numeric_limits<uint32_t>::max();
    
    if(!single)
        sscanf(&output[pos], "%s %s %d", &buffer[0], &buffer[0], &result);
    else sscanf(&output[pos], "%s %d", &buffer[0], &result);
    
    return result;
}

static void dumpToFile(const char* fileName) {
    
    fstream file(fileName, ios::binary | ios::out);
    file << "RSS" << endl;
    
    for(size_t i=0;i<samples.size() - 1;++i)
        file << to_string(samples[i]) << endl;
}

static uint32_t extractRss(const std::string& output) {
    
    std::array<char, 16> rss_data = {};
    size_t count = {};
    
    char numbers[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    for(auto& ch: output) {
        
        bool is_number = {};
        for(size_t i=0;i<sizeof(numbers);++i) {
            if(is_number = ch == numbers[i]) {
                rss_data[count++] = numbers[i];
                break;
            }
        }
        
        if(count >0 && !is_number)
            break;
        
    }
    
    
    auto end_pos = output.find("/");
    return stoi(string(&rss_data[0]));
}

int main(int argc, const char * argv[]) {
    
    if(argc != 3) {
        
        cout << "Usage: meminfo [process_name] [output_file]" << endl;
        return 0;
    }
    
    signal(SIGINT, intHandler);
    
    std::array<char, 256> processName = {};
    sscanf(argv[1], "%s", &processName[0]);
    
    char fileName[256];
    sscanf(argv[2], "%s", fileName);
    
    
    
    if(processName[0] == 0x0) {
        
        cout << "Invalid/empty process name!" << endl;
        return 0;
    }
    
    string cmd = "ps -axm -o rss,comm | grep -i ";
    cmd += string(&processName[0]);
    
    cout << "Command=" << cmd << endl;
    
    
    while(keepRunning) {

        output = exec(cmd.c_str());
        if(!output.size())
            continue;
        
        samples.push_back(extractRss(output) / 1024);
    }
    
    
    cout << "Total samples collected: " << samples.size() << endl;
    
    dumpToFile(fileName);
    
    return 0;
}

