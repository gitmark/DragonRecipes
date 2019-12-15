#ifndef CASTLE_CONFIG_H
#define CASTLE_CONFIG_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <DragonRecipes/StringTools.h>

using namespace dragon;

namespace castle {

bool fileExists(const std::string &filename);
int makeDir(const std::string &path);

class Config {
public:
    Config() {}

    int loadFile(const std::string &filename) {
        if (!fileExists(filename)) {
            std::cerr << "error: cannot find config file '" << filename << "'\n";
            return 1;
        }

        std::ifstream infile(filename);

        if (!infile.good()) {
            std::cerr << "error: cannot open config file '" << filename << "'\n";
            return 2;
        }

        std::string line;

        std::string section;

        while(std::getline(infile,line)) {
            line = trim(line);

            if (line.empty())
                continue;

            if (line.front() == ';' || line.front() == '#') {
                std::cout << "comment line\n";
                continue;
            }

            if (line.front() == '[') {
                std::vector<std::string> vec;
                split(line, " []", vec);
                if (vec.size() == 1)
                    section = vec.front();

                std::cout << "section = " << section << "\n";
                continue;
            }

            std::vector<std::string> vec;
            split(line, "=", vec);
            if (vec.size() == 2) {
                std::string name = trim(vec[0]);
                std::string val = trim(vec[1]);
                std::vector<std::string> parts;
                split(val, "#;", parts);

                if (parts.empty())
                    val.clear();
                else
                    val = trim(parts[0]);

                data[section][name] = val;
            }
        }

        return loadExpressions();
    }

    int loadExpressions() {
        std::string docsDir = data["dirs"]["docs"];
        std::string expressionsFilename = data["files"]["expressions"];
        std::string expFilename = docsDir + "/" + expressionsFilename;
        std::ifstream infile(expFilename);

        if (!infile.good()) {
            std::cerr << "error: cannot open expressions file '" << expFilename << "'\n";
            return 2;
        }

        std::string line;

        while(std::getline(infile,line)) {
            line = trim(line);

            if (line.empty())
                continue;

            expressions.push_back(line);
        }

        return 0;
    }

    std::map<std::string, std::map<std::string,std::string>> data;
    std::vector<std::string> expressions;
};

}

#endif
