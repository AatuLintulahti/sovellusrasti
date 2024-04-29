#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_fetchCarsButton_clicked();
    void on_addCarButton_clicked();
    void on_updateCarButton_clicked();
    void on_deleteCarButton_clicked();
    void on_carsTableWidget_itemSelectionChanged();


    void onNetworkReply(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *networkManager;
    QString selectedCarId;



    void fetchCars();
    void createCar(const QString &branch, const QString &model);
    void updateCar(const QString &id, const QString &branch, const QString &model);
    void deleteCar(const QString &id);
    void displayCars(const QByteArray &data);

};
#endif
