#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>

struct BuildTarget {
    std::string name;
    std::vector<std::string> sources;
    std::vector<std::string> dependencies;
};

class BuildSystemGenerator {
public:
    BuildSystemGenerator(const std::string& outputFormat, const std::string& directory)
        : outputFormat_(outputFormat), directory_(directory), cppVersion_("c++14") {}

    bool parse() {
        std::string filePath = directory_ + "/Blueprint.bp";
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file: " << filePath << std::endl;
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

    bool generateNinjaFile() {
        std::string outputFilename = "build.ninja";
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
                outfile << " ../" << source;
            }
            outfile << std::endl;
            outfile << "  description = Building " << target.name << std::endl << std::endl;
        }

        std::cout << "Build file generated successfully: " << outputFilename << std::endl;
        return true;
    }

    bool generateMakefile() {
        std::string outputFilename = "Makefile";
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
                outfile << "../" << source << " ";
            }
            for (const auto& dependency : target.dependencies) {
                outfile << dependency << " ";
            }
            outfile << std::endl;
            outfile << "\t@echo \"Building " << target.name << " [" << targetIndex << "/" << targets_.size() << "]\"";
            outfile << " && g++ -std=" << cppVersion_ << " -o " << target.name;
            for (const auto& source : target.sources) {
                outfile << " ../" << source;
            }
            for (const auto& dependency : target.dependencies) {
                outfile << " " << dependency;
            }
            outfile << std::endl << std::endl;
        }

        std::cout << "Build file generated successfully: " << outputFilename << std::endl;
        return true;
    }

private:
    std::string outputFormat_;
    std::string directory_;
    std::string cppVersion_;
    std::vector<BuildTarget> targets_;
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <output_format> <directory>" << std::endl;
        return 1;
    }

    std::string outputFormat = argv[1];
    std::string directory = argv[2];

    BuildSystemGenerator generator(outputFormat, directory);
    if (!generator.parse()) {
        return 1;
    }

    bool success;
    if (outputFormat == "Ninja") {
        success = generator.generateNinjaFile();
    } else if (outputFormat == "Makefile") {
        success = generator.generateMakefile();
    } else {
        std::cerr << "Error: Unsupported output format: " << outputFormat << std::endl;
        std::string SupportedFormats = "Ninja, Makefile";
        std::cout << "Supported formats are " << SupportedFormats << std::endl; 
        return 1;
    }

    if (!success) {
        return 1;
    }

    return 0;
}
