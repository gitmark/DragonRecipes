/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#include <sys/stat.h>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <getopt.h>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <exception>
#include <vector>
#include <initializer_list>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <mutex>
#include <stdexcept>
#include <QApplication>
#include <QDir>
#include <QCoreApplication>
#include <DragonRecipes/Production.h>
#include <DragonRecipes/Version.h>
#include <DragonRecipes/StringTools.h>

#include "App.h"
#include "Version.h"
#include "MainWindow.h"
using namespace dragon;
namespace castle {

bool fileExists(const std::string &filename) {
// https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
    struct stat buffer;
    return (stat (filename.c_str(), &buffer) == 0);
}

int makeDir(const std::string &inPath) {

    std::string path = trim(inPath);
    std::vector<std::string> parts;
    split(path, "/", parts);

    bool absolutePath = false;

    if (!path.empty() && path.front() == '/')
        absolutePath = true;

    std::string currentPath;
    for (size_t i = 0; i < parts.size(); ++i) {
        auto &p = parts[i];
        if (p.empty()) {
            continue;
        }

        if (i == 0) {
            if (absolutePath)
                currentPath += "/";
        } else {
            currentPath += "/";
        }

        currentPath += p;

        if (p == ".." || p == ".")
            continue;

        if (!fileExists(currentPath)) {
            int rc = mkdir(currentPath.c_str(), 0777);
            if (rc == -1) {
                std::cerr << "error: " << strerror(errno) << "\n";
                return 1;
            }
        }
    }

    return 0;
}


std::weak_ptr<App> App::_theApp;

App::App() :
    _versionNum(appVersion()),
    _devStage(appDevStage())
{
    _homeDir = std::getenv("HOME");
    _defaultConfigFilename = _homeDir + "/.castle.conf";
    _configFilename = _defaultConfigFilename;
    _defaultConfigContent = ";Castle config settings\n"
                            "[dirs]\n"
                            "docs=Documents/Castle\n"
                            "[files]\n"
                            "expressions=expressions.txt\n"
                            "\n";
}


App::~App() {

}

int App::parseArgs(int argc, char **argv) {
    std::string optLong;
    std::string optArg;
    std::vector<char> charBuf(2);

    static std::vector<struct option> long_options = {
        { "config",	required_argument,			nullptr,			'c' },
        { "version",	no_argument,			nullptr,			'v' },
        { "help",		no_argument,			nullptr,			'h' },
        { nullptr, 0, nullptr, 0 }
    };

    int option_index = 0;

    while (true) {
        int opt = getopt_long(argc, argv, "+:vhc:",
                              long_options.data(), &option_index);

        if (opt == -1)
            break;

        switch (opt) {

        case 0:
            //printf("option %s", long_options[option_index].name);
            //if (optarg)
              //  printf(" with arg %s", optarg);
            //printf("\n");
            break;

        case 'c':
            optLong = long_options[static_cast<size_t>(option_index)].name;
            _configFilename = optarg;
            break;

        case 'v':
            _version = true;
            break;

        case 'h':
            _help = true;
            break;

        case '?': {
            charBuf[0] = static_cast<char>(optopt);
            std::cerr << "error: bad option: " << charBuf.data() << "\n";
            std::cerr << _usage << "\n";
            _error = 1;
        }
        return 0;

        case ':': {
            charBuf[0] = static_cast<char>(optopt);
            std::cerr << "error: mission argument for option: " << charBuf.data() << "\n";
            std::cerr << _usage << "\n";
            _error = 1;
        }
        return 0;

        default:
            std::cerr << "error: invalid argument\n";
            std::cerr << _usage;
            _error = 1;
            return 1;
        }
    }

    int rc = 0;

    if (_help) {
        std::cout << _usage;
    } else if (_version) {
        std::cout << "castle version " << _versionNum << " " << _devStage << "\n";
        std::cout << "dragonrecipes version " << dragonRecipesVersion() << "\n";
    }

    return rc;
}

std::shared_ptr<App> App::create() {
    std::shared_ptr<App> ptr = App::_theApp.lock();
    if (ptr != nullptr)
        return nullptr;

    std::shared_ptr<App> app(new App());
    App::_theApp = app;
    return app;
}

int App::run(int argc, char **argv) {
    std::stringstream ss;

    ss << "Usage: castle [-v] [-h]\n";
    ss << "\n";
    ss << "  -v, --version    Displays the version info for the app.\n";
    ss << "  -h, --help       Displays help info for the app.\n";
    ss << "\n";

    _usage = ss.str();

    int rc = parseArgs(argc, argv);

    if (rc != 0)
        return rc;

    if (_help || _version)
        return 0;

    if (!fileExists(_configFilename)) {
        std::cerr << "error: config file does not exist '" << _configFilename << "'.\n";

        if (_configFilename != _defaultConfigFilename) {
            return 1;
        }

        _configFilename = trim(_configFilename);

        bool absolutePath = false;

        if (!_configFilename.empty() && _configFilename.front() == '/')
            absolutePath = true;

        std::cerr << "error: trying to create default config file now '" << _configFilename << "'\n";

        if (!_configFilename.empty() && _configFilename.back() != '/') {
            std::vector<std::string> parts;
            split(_configFilename, "/", parts);
            std::string path;

            for (size_t i = 0; i < parts.size() - 1; ++i) {
                auto &p = parts[i];
                if (p.empty()) {
                    continue;
                }

                if (i == 0) {
                    if (absolutePath)
                        path += "/";
                } else {
                    path += "/";
                }

                path += p;
            }

            rc = makeDir(path);
            if (rc != 0) {
                std::cerr << "error: could not create config file directory '" << path << "'\n";
                return rc;
            }

            std::ofstream outfile(_configFilename);
            if (!outfile.good()) {
                std::cerr << "error: could not open config file for writing '" << _configFilename << "'\n";
                return 1;
            }
            outfile << _defaultConfigContent;
        } else {
            std::cerr << "error: config filename is not valid '" << _configFilename << "'\n";
            return 1;
        }
    }

    ///////////



    rc = _config.loadFile(_configFilename);

    if (rc != 0)
        return rc;

    std::cout << "data[dirs][docs] = "  << _config.data["dirs"]["docs"] << "\n";

    QApplication a(argc, argv);
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cdUp();
    dir.cd("plugins");
    QCoreApplication::setLibraryPaths(QStringList(dir.absolutePath()));
    QApplication::setWindowIcon(QIcon(":/images/castle.icns"));
    MainWindow w;
    w.init(&_config);
    w.show();
    return QApplication::exec();
}

}
