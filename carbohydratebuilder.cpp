#include "carbohydratebuilder.h"
#include "ui_carbohydratebuilder.h"

CarbohydrateBuilder::CarbohydrateBuilder(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CarbohydrateBuilder)
{
    ui->setupUi(this);
}

CarbohydrateBuilder::~CarbohydrateBuilder()
{
    delete ui;
}
