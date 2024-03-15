#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct BuildTarget {
    std::string name;
    std::vector<std::string> sources;
    std::vector<std::string> dependencies;
};

class BuildSystemGenerator {
public:
    BuildSystemGenerator(const std::string& filename, const std::string& outputFormat)
        : filename_(filename), outputFormat_(outputFormat), cppVersion_("c++14") {}

    bool parse() {
        std::ifstream file(filename_);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file: " << filename_ << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;
            iss >> token;
            if (token == "CPP_VERSION") {
                iss >> cppVersion_;
            } else if (token == "TARGET") {
                BuildTarget target;
                iss >> target.name;
                while (iss >> token) {
                    if (token == "SRC") {
                        std::string source;
                        iss >> source;
                        target.sources.push_back(source);
                    } else if (token == "DEP") {
                        std::string dependency;
                        iss >> dependency;
                        target.dependencies.push_back(dependency);
                    }
                }
                targets_.push_back(target);
            }
        }
        return true;
    }

    bool generateNinjaFile(const std::string& outputFilename) {
        std::ofstream outfile(outputFilename);
        if (!outfile.is_open()) {
            std::cerr << "Error: Unable to open output file: " << outputFilename << std::endl;
            return false;
        }

        outfile << "ninja_required_version = 1.10" << std::endl << std::endl;

        outfile << "rule cpp" << std::endl;
        outfile << "  command = g++ -std=" << cppVersion_ << " -o $out $in" << std::endl << std::endl;

        for (const auto& target : targets_) {
            outfile << "build " << target.name << ": cpp";
            for (const auto& source : target.sources) {
                outfile << " " << source;
            }
            outfile << std::endl;
            outfile << "  description = Building " << target.name << std::endl << std::endl;
        }

        return true;
    }

    bool generateMakefile(const std::string& outputFilename) {
        std::ofstream outfile(outputFilename);
        if (!outfile.is_open()) {
            std::cerr << "Error: Unable to open output file: " << outputFilename << std::endl;
            return false;
        }

        outfile << "all: ";
        for (const auto& target : targets_) {
            outfile << target.name << " ";
        }
        outfile << std::endl << std::endl;

        int targetIndex = 0;
        for (const auto& target : targets_) {
            targetIndex++;
            outfile << target.name << ": ";
            for (const auto& source : target.sources) {
                outfile << source << " ";
            }
            for (const auto& dependency : target.dependencies) {
                outfile << dependency << " ";
            }
            outfile << std::endl;
            outfile << "\t@echo \"Building " << target.name << " [" << targetIndex << "/" << targets_.size() << "]\"";
            outfile << " && g++ -std=" << cppVersion_ << " -o " << target.name;
            for (const auto& source : target.sources) {
                outfile << " " << source;
            }
            for (const auto& dependency : target.dependencies) {
                outfile << " " << dependency;
            }
            outfile << std::endl << std::endl;
        }

        return true;
    }

private:
    std::string filename_;
    std::string outputFormat_;
    std::string cppVersion_;
    std::vector<BuildTarget> targets_;
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_format> <output_file>" << std::endl;
        return 1;
    }

    std::string inputFilename = argv[1];
    std::string outputFormat = argv[2];
    std::string outputFilename = argv[3];

    BuildSystemGenerator generator(inputFilename, outputFormat);
    if (!generator.parse()) {
        return 1;
    }

    bool success;
    if (outputFormat == "Ninja") {
        success = generator.generateNinjaFile(outputFilename);
    } else if (outputFormat == "Makefile") {
        success = generator.generateMakefile(outputFilename);
    } else {
        std::cerr << "Error: Unsupported output format: " << outputFormat << std::endl;
        std::string SupportedFormats = "Ninja, Makefile";
        std::cout << "Supported formats are " << SupportedFormats << std::endl; 
        return 1;
    }

    if (!success) {
        return 1;
    }

    std::cout << "Build file generated successfully: " << outputFilename << std::endl;
    return 0;
}
