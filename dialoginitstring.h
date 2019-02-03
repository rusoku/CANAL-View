#ifndef DIALOGINITSTRING_H
#define DIALOGINITSTRING_H

#include <QDialog>

namespace Ui {
class DialogInitString;
}

class DialogInitString : public QDialog
{
    Q_OBJECT

public:
    explicit DialogInitString(QWidget *parent = nullptr);
    ~DialogInitString();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DialogInitString *ui;
};

#endif // DIALOGINITSTRING_H
