/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef APP_H
#define APP_H

#include <memory>
#include <string>
#include <list>

namespace knight {

class App {
  public:
    App();
    App(const App &app) = delete;
    App(App &&app) noexcept = delete;
    App &operator=(const App &app) = delete;
    App &operator=(App &&app) noexcept = delete;
    virtual ~App() = default;

    static std::shared_ptr<App> create();
    virtual int run(int argc, char **argv);
    virtual int parseArgs(int argc, char **argv);
    static std::shared_ptr<App> app();

  private:
      int _argc{0};
      const char **_argv{nullptr};
      bool _version{false};
      bool _help{false};
    std::string _binName;
    std::string _versionNum;
    std::string _devStage;
    std::string _usage;
    int _error{0};

    static std::weak_ptr<App> _theApp;
};

}

#endif

