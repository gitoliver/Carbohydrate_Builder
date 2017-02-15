#ifndef CARBOHYDRATEBUILDER_H
#define CARBOHYDRATEBUILDER_H

#include <QMainWindow>

namespace Ui {
class CarbohydrateBuilder;
}

class CarbohydrateBuilder : public QMainWindow
{
    Q_OBJECT

public:
    explicit CarbohydrateBuilder(QWidget *parent = 0);
    ~CarbohydrateBuilder();

private:
    Ui::CarbohydrateBuilder *ui;
};

#endif // CARBOHYDRATEBUILDER_H
