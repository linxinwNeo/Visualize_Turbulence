#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H

#include <QString>
#include <QMessageBox>
void throwErrorMessage( QString message ){
    QMessageBox messageBox;
    messageBox.critical(0, "Error", message);
    messageBox.setFixedSize(500,200);
    exit(-1);
}

#endif // UTILITY_FUNCTIONS_H
