#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   connect(ui->theButton1, &QPushButton::pressed,
           this, &MainWindow::setDemo1TextButton);

   connect(ui->theButton2, &QPushButton::pressed,
           this, &MainWindow::setDemo2TextButton);
}

MainWindow::~MainWindow()
{
   delete ui;
}

void MainWindow::setDemo1TextButton()
{
   ui->theDemo1->setText(ui->theTestText1->text());
}

void MainWindow::setDemo2TextButton()
{
   ui->theDemo2->setText(ui->theTestText2->text());
}

