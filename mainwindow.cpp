
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupFileMenu();

    editor = new CodeEditor();
    editor->setStyleSheet("font: 24pt Helvetica");
    highlighter = new Highlighter(editor->document());

    setCentralWidget(editor);
    setWindowTitle(tr("Editori"));
}

void MainWindow::newFile()
{
    editor->clear();
}

void MainWindow::openFile(const QString &path) {

    QString filename = path;
    if (filename.isNull()) {
        filename = QFileDialog::getOpenFileName(this, tr("Open File"), "", "");
    }

    if (!filename.isEmpty()) {
        QFile file(filename);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            editor->setPlainText(file.readAll());
        }
    }
}

void MainWindow::saveFile() {
      QString filename = QFileDialog::getSaveFileName(this, "Save as");
      QFile file(filename);
      if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Could not save file: " + file.errorString());
        return;
      }

      currentFile = filename;
      setWindowTitle(filename);
      QTextStream out(&file);
      QString text = editor->toPlainText();
      out << text;
      file.close();
}

void MainWindow::action_copy() {
    editor->copy();
}

void MainWindow::action_paste() {
    editor->paste();
}

void MainWindow::action_cut() {
    editor->cut();
}

void MainWindow::action_undo() {
    editor->undo();
}

void MainWindow::action_redo() {
    editor->redo();
}

void MainWindow::action_print() {
    QPrinter printer;
    printer.setPrinterName(("Printer Name"));
    QPrintDialog pDialog(&printer, this);
    if (pDialog.exec() == QDialog::Rejected) {
        QMessageBox::warning(this, "Warning", "Issue regarding printer");
        return;
    }
    editor->print(&printer);
}

void MainWindow::action_exit() {
    QApplication::quit(); // determine if the destructor will be called here
    /* may want to delete no longer used memory [objects] here for performance reasons [to avoid memory leak/s]
        either here or in the MainWindow's destructor assuming that "delete &ui;" does not prevent memeory leaks
     */
}

void MainWindow::setupFileMenu() { // and EditMenu
    /// File Menu
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    // note: for usabillity issues it may help to disable some of the QKeySequences
    // [and provide an alternative behavior to help avoid users accidentally erasing their work]

    // update the following with new syntax
    fileMenu->addAction(tr("&New"), this, SLOT(newFile()), QKeySequence::New);
    fileMenu->addAction(tr("&Open"), this, SLOT(openFile()), QKeySequence::Open);
    fileMenu->addAction(tr("Save"), this, SLOT(saveFile()), QKeySequence::Save);
    fileMenu->addAction(tr("Print"), this, SLOT(action_print()), QKeySequence::Print);

    fileMenu->addAction(tr("Exit"), this, SLOT(action_exit()), QKeySequence::Close);

    /// Edit Menu [move to own method]
    QMenu *editMenu = new QMenu(tr("&Edit"), this);
    menuBar()->addMenu(editMenu);

    // update the following with new syntax
    editMenu->addAction(tr("Copy"), this, SLOT(action_copy()), QKeySequence::Copy);
    editMenu->addAction(tr("Paste"), this, SLOT(action_paste()), QKeySequence::Paste);
    editMenu->addAction(tr("Cut"), this, SLOT(action_cut()), QKeySequence::Cut);
    editMenu->addAction(tr("Undo"), this, SLOT(action_undo()), QKeySequence::Undo);
    editMenu->addAction(tr("Redo"), this, SLOT(aciton_redo()), QKeySequence::Redo);
}

MainWindow::~MainWindow() {
    delete &ui;
}
