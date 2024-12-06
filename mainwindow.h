#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <random>
#include <QTimer>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileInfo>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void Anzeigen();
    void Rotieren();
    bool LadeAntworten(const QString &dateiName);
    void antwortenErstellen(const QString &dateiName);


private:
    Ui::MainWindow *ui;
    QTimer* schuettelnTimer;
    QTimer* rotationTimer;
    QPixmap kugelBild;
    QPixmap kugelBildDreieck;
    QStringList antworten;
    QString dateiName;
    static QStringList StandardAntworten();

};
#endif // MAINWINDOW_H
