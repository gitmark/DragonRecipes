/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>
#include <QListWidgetItem>
#include <DragonRecipes.h>
#include "Config.h"

using namespace dragon;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace castle {

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    MainWindow(const MainWindow &mainWindow) = delete;
    MainWindow(MainWindow &&mainWindow) noexcept = delete;
    MainWindow &operator=(const MainWindow &mainWindow) = delete;
    MainWindow &operator=(MainWindow &&mainWindow) noexcept = delete;

    ~MainWindow() override;
    int init(Config *config);

  private:
    std::shared_ptr<Production> production;
    std::string _filename;

  private slots:
    void on_pushButton_clicked();

    void on_listExpressions_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::MainWindow *ui;
    std::string textTree;
    Config *_config;
};

}

#endif

