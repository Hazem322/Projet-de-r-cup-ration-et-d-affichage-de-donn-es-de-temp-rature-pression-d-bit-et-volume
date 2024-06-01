#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    serial(new QSerialPort(this)),
    receivingEnabled(false) // Initialisation de receivingEnabled à false
{
    ui->setupUi(this);

    // Configuration du port série
    serial->setPortName("COM13");  // Modifier avec le bon port série
    serial->setBaudRate(QSerialPort::Baud9600);

    // Tentative d'ouverture du port série
    if (serial->open(QIODevice::ReadOnly)) {
        connect(serial, &QSerialPort::readyRead, this, &MainWindow::handleSerialData);
        ui->label_12->setText("Connected!");
        ui->label_12->setStyleSheet("color: green;");
    } else {
        qDebug() << "Erreur lors de l'ouverture du port série.";
        ui->label_12->setText( "Disconnected!");
         ui->label_12->setStyleSheet("color: red;");
    }

    // Connexion du signal de clic du bouton "INITIALISATION"
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::initializeValues);
    // Connexion du signal de clic des boutons "Start" et "Stop"
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::startSerialReading);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::stopSerialReading);
    // Connexion du bouton à la fonction de réinitialisation
        connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::resetLabel);
}

MainWindow::~MainWindow()
{
    if (serial->isOpen())
        serial->close();
    delete ui;
    delete serial;
}

void MainWindow::handleSerialData()
{
    if (!receivingEnabled) // Vérifie si la réception est activée
        return; // Si non, arrête le traitement

    QByteArray data = serial->readAll();

    // Affichage de la trame brute dans le QTextBrowser
      if (data.startsWith("FEFF")) {
    ui->textBrowser->append(QString(data));

    // Vérification de la taille des données reçues
    if (data.size() < 28) {
        // Ignorer si la trame n'est pas complète
        return;
    }

    // Extraction des valeurs hexadécimales de la trame
    QByteArray t1Hex = data.mid(4, 2);
    QByteArray t2Hex = data.mid(6, 2);
    QByteArray t3Hex = data.mid(8, 2);
    QByteArray t4Hex = data.mid(10, 2);
    QByteArray t5Hex = data.mid(12, 2);
    QByteArray t6Hex = data.mid(14, 2);
    QByteArray t7Hex = data.mid(16, 2);
    QByteArray t8Hex = data.mid(18, 2);
    QByteArray DHex = data.mid(20, 2);
    QByteArray VHex = data.mid(22, 2);
    QByteArray PHex = data.mid(24, 2);
    //QByteArray ss = data.right(2); // Les 4 derniers octets de la trame
    //QString ss = QString::fromLatin1(data.mid(26));
int val=data.length()-26-1;
QByteArray ss=data.mid(26,val);



    // Conversion des valeurs hexadécimales en entiers décimaux
    bool ok;
    int t1Value = t1Hex.toInt(&ok, 16);
    int t2Value = t2Hex.toInt(&ok, 16);
    int t3Value = t3Hex.toInt(&ok, 16);
    int t4Value = t4Hex.toInt(&ok, 16);
    int t5Value = t5Hex.toInt(&ok, 16);
    int t6Value = t6Hex.toInt(&ok, 16);
    int t7Value = t7Hex.toInt(&ok, 16);
    int t8Value = t8Hex.toInt(&ok, 16);
    int dValue = DHex.toInt(&ok, 16);
    int pValue = VHex.toInt(&ok, 16);
    int vValue = PHex.toInt(&ok, 16);
    int sum = t1Value + t2Value + t3Value + t4Value + t5Value + t6Value + t7Value + t8Value + dValue + pValue + vValue;
    int saa = ss.toInt(&ok, 16); // Convertir les 3 derniers octets en entier décimal

    // Affichage des valeurs dans les QLCDNumber correspondants
    ui->lcdNumber->display(t1Value);
    ui->lcdNumber_2->display(t2Value);
    ui->lcdNumber_3->display(t3Value);
    ui->lcdNumber_4->display(t4Value);
    ui->lcdNumber_5->display(t5Value);
    ui->lcdNumber_6->display(t6Value);
    ui->lcdNumber_7->display(t7Value);
    ui->lcdNumber_8->display(t8Value);
    ui->lcdNumber_9->display(dValue);
    ui->lcdNumber_10->display(pValue);
    ui->lcdNumber_11->display(vValue);
      //  ui->lineEdit_2->display(saa);
    // Affichage de la somme dans le QLineEdit
      ui->lineEdit_2->display(sum);
    ui->lineEdit_3->display(saa);

    // Suppression des données traitées de la QByteArray
    data.remove(0, 28);
    // Vérification de la transmission
       QString transmissionStatus;
       if (sum == saa) {
           transmissionStatus = "<span style='color: green; font-weight: bold;'>somme correct</span>";
       } else {
           transmissionStatus = "<span style='color: red; font-weight: bold;'>somme  non correct</span>";
       }

       // Mettre à jour le texte de label_17 avec le statut de transmission mis en forme
       ui->label_17->setText(transmissionStatus);

       // Suppression des données traitées de la QByteArray
       data.remove(0, 28);

}
else{
          ui->label_18->setText("trame incorrect");

      }

}
void MainWindow::initializeValues()
{
    // Réinitialisation des QLCDNumber à zéro
    ui->lcdNumber->display(0);
    ui->lcdNumber_2->display(0);
    ui->lcdNumber_3->display(0);
    ui->lcdNumber_4->display(0);
    ui->lcdNumber_5->display(0);
    ui->lcdNumber_6->display(0);
    ui->lcdNumber_7->display(0);
    ui->lcdNumber_8->display(0);
    ui->lcdNumber_9->display(0);
    ui->lcdNumber_10->display(0);
    ui->lcdNumber_11->display(0);
    ui->lineEdit_3->display(0);
    ui->lineEdit_2->display(0);
}

void MainWindow::startSerialReading()
{
    if (!receivingEnabled) {  // Vérifie si la réception n'est pas déjà activée
        receivingEnabled = true;  // Active la réception
          // Change le libellé du bouton
    }
}

void MainWindow::stopSerialReading()
{
    if (receivingEnabled) {  // Vérifie si la réception est active
        receivingEnabled = false;  // Désactive la réception
        // Change le libellé du bouton
    }
}
void MainWindow::resetLabel()
{
    ui->label_17->setText("test");
}
