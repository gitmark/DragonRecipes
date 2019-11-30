#include <vector>
#include <string>
#include <sstream>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDir>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadPlugins();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    std::string head = ui->lineEditHead->text().toStdString();
    std::string body = ui->lineEditBody->text().toStdString();
    production = std::make_shared<Production>(head, body);
    std::vector<std::string> parts = production->body();
    std::stringstream ss;
    ss << production->head() << "\n\n";
    for(auto p : parts) {
        ss << p << "\n";
    }

    ui->textEditResults->setText(ss.str().c_str());
 //   QMessageBox::information(this,tr("my ap"),tr("hello"), QMessageBox::Ok);
}

void MainWindow::loadPlugins()
{
    /*
    QDir pluginDir(QApplication::applicationDirPath());
#if defined(Q_OS_WIN)
    if (pluginDir.dirName().toLower() == "debug" || pluginDir.dirName().toLower() == "release")
        pluginDir.cdUp();
#eldif defined(Q_OS_MAC)
    if (pluginDir.dirName() == "MacOS") {
        pluginDir.cdUp();
    }
#endif
     */
}
