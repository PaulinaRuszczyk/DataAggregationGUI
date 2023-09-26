#ifndef CINFOWINDOW_H
#define CINFOWINDOW_H

#include <QDialog>

namespace Ui {
class CInfoWindow;
}

class CInfoWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CInfoWindow(QWidget *parent = nullptr);
    ~CInfoWindow();
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();
signals:
    void okClicked();
private:
    Ui::CInfoWindow *ui;
};

#endif // CINFOWINDOW_H
