#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void resetLabel();

private:
    bool receivingEnabled;
private slots:
   void handleSerialData();
  void initializeValues();

  void startSerialReading();
    void stopSerialReading();




private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
};

#endif //
