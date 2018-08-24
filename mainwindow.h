#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "highlighter.h"
#include "codeeditor.h"

#include <QtWidgets>
#include <QMainWindow>
#include <QFile>
#include <QFileDialog>

#include <QPrinter>
#include <QPrintDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

    /// File Mneu
    void newFile();
    void openFile(const QString &path = QString());
    void saveFile();
    void action_print();

    /// Edit Menu
    void action_copy();
    void action_paste();
    void action_cut();
    void action_undo();
    void action_redo();

    // should be under "Other"
    // * help
    // * about
    void action_exit();

private:
    void setupFileMenu();

    CodeEditor *editor;
    Highlighter *highlighter;

    QString currentFile;
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
