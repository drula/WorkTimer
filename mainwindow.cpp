#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    Q_ASSERT(connect(_ui->startFinishButton, &QPushButton::clicked, this, &MainWindow::onStartButton));

    setState(State::Initial);
}

void MainWindow::setState(State state)
{
    switch (state) {
    case State::Initial:
        _ui->totalTimeEdit->setEnabled(true);
        _ui->workTimeEdit->setEnabled(true);
        _ui->restTimeEdit->setEnabled(true);

        _ui->startFinishButton->setEnabled(true);
        _ui->startFinishButton->setText("Start");

        _ui->sessionTimeEdit->setEnabled(false);
        _ui->workRestButton->setEnabled(false);
        _ui->workRestButton->setText("Work");
        _ui->nextSessionTimeEdit->setEnabled(true);

        setDefaultValues();

        break;

    default:
        break; //! message or exception in debug
    }

    _state = state;
}

void MainWindow::setDefaultValues()
{
    //! get values from settings
    _ui->totalTimeEdit->setTime(QTime(8, 0));
    _ui->workTimeEdit->setTime(QTime(6, 0));
    _ui->restTimeEdit->setTime(QTime(2, 0));

    _ui->nextSessionTimeEdit->setTime(QTime(0, 25, 0));
}

void MainWindow::onStartButton()
{
    if (_state == State::Initial)
        setState(State::Started);
}

MainWindow::~MainWindow()
{
    delete _ui;
}
