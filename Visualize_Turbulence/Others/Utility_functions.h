#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H

#include <QString>
#include <QMessageBox>
#include <set>
#include "Geometry/Tet.h"

inline void throwErrorMessage( const QString message ){
    QMessageBox messageBox;
    messageBox.critical(0, "Error", message);
    messageBox.setFixedSize(500,200);
    exit(-1);
}


inline void ScreenToSecondWin(
        const int px, const int py,
        const int screen_leftx, const int screen_bottomy,
        const int win_screen_sizex, const int win_screen_sizey,
        const double world_leftx, const double world_bottomy,
        const double win_world_sizex, const double win_world_sizey,
        double &s, double &t
)
{
    double ratiox = (double)(px-screen_leftx)/ (double)win_screen_sizex;
    double ratioy = (double)(screen_bottomy - py)/(double)win_screen_sizey;

    s = (double) world_leftx + ratiox * win_world_sizex;
    t = (double) world_bottomy + ratioy * win_world_sizey;
}


inline bool is_in_set(set<Tet*> s, Tet* tet)
{
    if(s.find(tet) == s.end()) return false;
    return true;
}


#endif // UTILITY_FUNCTIONS_H
