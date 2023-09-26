#ifndef CINSERTWINDOW_H
#define CINSERTWINDOW_H

#include <QDialog>

namespace Ui {
class CInsertWindow;
}

class CInsertWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CInsertWindow(QWidget *parent = nullptr);
    ~CInsertWindow();

private slots:
    void on_buttonBox_accepted();
signals:
    void onOkClicked(std::string name, std::string id);
private:
    Ui::CInsertWindow *ui;
};

#endif // CINSERTWINDOW_H
