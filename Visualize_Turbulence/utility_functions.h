#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H

#include <QString>
#include <QMessageBox>

inline void throwErrorMessage( QString message ){
    QMessageBox messageBox;
    messageBox.critical(0, "Error", message);
    messageBox.setFixedSize(500,200);
    exit(-1);
}


inline void ScreenToSecondWin(
        int px, int py,
        int screen_leftx, int screen_bottomy,
        int win_screen_sizex, int win_screen_sizey,
        double world_leftx, double world_bottomy,
        double win_world_sizex, double win_world_sizey,
        double &s, double &t
)
{
    double ratiox = (double)(px-screen_leftx)/ (double)win_screen_sizex;
    double ratioy = (double)(screen_bottomy - py)/(double)win_screen_sizey;

    s = (double) world_leftx + ratiox * win_world_sizex;
    t = (double) world_bottomy + ratioy * win_world_sizey;
}

#endif // UTILITY_FUNCTIONS_H
