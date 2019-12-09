/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

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
#include "App.h"
#include "Version.h"
#include "mainwindow.h"

namespace dragon {

std::weak_ptr<App> App::_theApp;

App::App() :
    _argc(0), _argv(nullptr),
    _version(false), _help(false),
    _versionNum(CASTLE_VERSION),
    _devStage(CASTLE_DEV_STAGE),
    _error(0)
{}

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
            optLong = long_options[option_index].name;
            _filename = optarg;
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

    QApplication a(argc, argv);
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cdUp();
    dir.cd("plugins");
    QCoreApplication::setLibraryPaths(QStringList(dir.absolutePath()));
    QApplication::setWindowIcon(QIcon(":/images/castle.icns"));
    MainWindow w;
    w.init(_filename);
    w.show();
    return QApplication::exec();
}

}
