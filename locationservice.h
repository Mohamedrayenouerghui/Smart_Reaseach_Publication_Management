#ifndef LOCATIONSERVICE_H
#define LOCATIONSERVICE_H

#include <QWidget>
#include <QLineEdit>

namespace Ui {
class LocationService;
}

class LocationService : public QWidget
{
    Q_OBJECT

public:
    explicit LocationService(QWidget *parent = nullptr);
    ~LocationService();

    // Pré-remplir le champ lieu depuis l'extérieur
    void setLieu(const QString &lieu);

private:
    Ui::LocationService *ui;
    QLineEdit *m_lieuLineEdit = nullptr;
};

#endif // LOCATIONSERVICE_H
