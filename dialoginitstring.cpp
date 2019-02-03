#include "dialoginitstring.h"
#include "ui_dialoginitstring.h"

DialogInitString::DialogInitString(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInitString)
{
    ui->setupUi(this);
}

DialogInitString::~DialogInitString()
{
    delete ui;
}

void DialogInitString::on_pushButton_clicked()
{
    close();
}
