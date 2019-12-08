/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QCoreApplication>

#include <iostream>
#include <getopt.h>
#include <DragonRecipes.h>
#include <App.h>

int main(int argc, char *argv[]) {
    auto app = App::create();
    return app->run(argc, argv);
}
