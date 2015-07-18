/*
 *  Console chess written in Borland C++
 *  Copyright (C) 2015 Utkarsh Gupta
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <conio.h>
#include <fstream.h>
#include <stdio.h>
#include <stdlib.h>

// #define LEGACY_IDE

int turn = 1;
char xi, yi, xf, yf;
int dx, dy;
char ci, cf;
int mIsMoveStraight, mIsMoveDiagonal;

int Game[8][8]  = {
    { 'R', 'P', ' ', ' ', ' ', ' ', -'P', -'R' },
    { 'N', 'P', ' ', ' ', ' ', ' ', -'P', -'N' },
    { 'B', 'P', ' ', ' ', ' ', ' ', -'P', -'B' },
    { 'K', 'P', ' ', ' ', ' ', ' ', -'P', -'Q' },
    { 'Q', 'P', ' ', ' ', ' ', ' ', -'P', -'K' },
    { 'B', 'P', ' ', ' ', ' ', ' ', -'P', -'B' },
    { 'N', 'P', ' ', ' ', ' ', ' ', -'P', -'N' },
    { 'R', 'P', ' ', ' ', ' ', ' ', -'P', -'R' }
};

void saveGame() {
    ofstream save;
    save.open("save.dat", ios::binary | ios::out);

    if (!save) {
        cout<<endl<<"Save game could not be saved! Exiting now.";
    } else {
        save.write((char *) Game, sizeof(Game));
    }

    save.close();
}

void loadGame() {
    ifstream save;
    save.open("save.dat", ios::binary | ios::in);

    if (!save) {
        cout<<endl<<"Save game could not be loaded! Starting a new game.";
    } else {
        save.read((char *) Game, sizeof(Game));
    }

    save.close();
}

void pressAnyKeyToContinue() {
    cout<<endl<<"Press any to continue!";
    getch();
}

char intToChar(int i) {
    char c;

    if (i == ' ') {
        c = i;
    } else if (i > 0) {
        c = turn == 1 ? i : i + 32;
    } else if (i < 0) {
        c = turn == 2 ? -i : -i + 32;
    }

    return c;
}

void print() {
#ifdef LEGACY_IDE
        clrscr();
#else
        system("CLS");
#endif

    char c;

    cout<<"              ";

    for (int i = 'A'; i <= 'H'; i++) {
        c = i;
        cout<<c<<"       ";
    }

    cout<<endl<<endl<<endl<<endl<<endl;

    for (int i = 0; i <= 7; i++) {
        cout<<endl<<endl<<endl<<endl<<endl<<(i + 1)<<"             ";

        for (int j = 0; j <= 7; j++) {
            c = intToChar(Game[j][i]);
            cout<<c<<"       ";
        }
    }
}

void prepare() {
    dx = abs(xf - xi);
    dy = abs(yf - yi);

    mIsMoveStraight = dx == 0 || dy == 0 ? 1 : 0;
    mIsMoveDiagonal = dx == dy ? 1 : 0;
}

int prepareIntial() {
    if (xi > 96) {
        xi -= 32;
    }

    if (xi < 'A' || xi > 'H' || yi < '1' || yi > '8') {
        cout<<endl<<"Invalid coordinates!";
        return 0;
    }

    xi -= 65;
    yi -= 49;

    ci = intToChar(Game[xi][yi]);

    if (ci == ' ') {
       cout<<endl<<"Invalid piece!";
       return 0;
    } else if (ci > 96) {
       cout<<endl<<"You must select your own piece";
       return 0;
    }

    cout<<ci;

    return 1;
}

int prepareFinal() {
    if (xf > 96) {
        xf -= 32;
    }

    if (xf < 'A' || xf > 'H' || yf < '1' || yf > '8') {
        cout<<endl<<"Invalid coordinates!";
        return 0;
    }

    xf -= 65;
    yf -= 49;

    cf = intToChar(Game[xf][yf]);
    cout<<cf;

    return 1;
}

int isPathClear() {
    int ix = xf > xi ? xi : xf;
    int iy = yf > yi ? yi : yf;
    int fx = ix++ + dx;
    int fy = iy++ + dy;

    if (mIsMoveStraight == 1) {
        if (dx == 0) {
            for (int i = ix; i < fx; i++) {
                if (Game[i][iy] != ' ') {
                    return 0;
                }
            }
        } else {
            for (int j = iy; j < fy; j++) {
                if (Game[ix][j] != ' ') {
                    return 0;
                }
            }
        }
    } else if (mIsMoveDiagonal == 1) {
        for (int i = ix, j = iy; i < fx, j < fy; i++, j++) {
            if (Game[i][j] != ' ') {
                return 0;
            }
        }
    }
    return 1;
}

int isMoveValid() {
    if (dx == 0 && dy == 0) {
        cout<<endl<<"Null move not permitted.";
        return 0;
    } else if (mIsMoveDiagonal == 0 && mIsMoveStraight == 0 && ci != 'N') {
        cout<<endl<<"Invalid move.";
        return 0;
    } else if (isPathClear() == 0 && ci != 'N') {
        cout<<endl<<"Can't jump over another piece.";
        return 0;
    } else if (cf != ' ' && cf < 96) {
        cout<<endl<<"Can't capture your own piece.";
        return 0;
    }

    switch (ci) {
        case 'P':
                    if ((turn == 1 ? yf - yi: yi - yf) > 0) {
                        if (dx == 0 && cf == ' ') {
                            if (dy == 1) {
                                return 1;
                            } else if (dy == 2 && turn == 1 ? yi == 1 : yi == 6) {
                                return 1;
                            }
                        } else if (dx == 1 && dy == 1 && cf > 96) {
                            return 1;
                        }
                    }

                    break;

        case 'N':
                    if (dx == 1 && dy == 2 || dx == 2 && dy == 1) {
                        return 1;
                    }

                    break;

        case 'B':
                    if (mIsMoveDiagonal == 1) {
                        return 1;
                    }

                    break;

        case 'R':
                    if (mIsMoveStraight == 1) {
                        return 1;
                    }

                    break;

        case 'Q':
                    if (mIsMoveDiagonal == 1 || mIsMoveStraight == 1) {
                        return 1;
                    }

                    break;

        case 'K':
                    if (dx <= 1 && dy <= 1) {
                        return 1;
                    }

                    break;

    }

    cout<<endl<<"Invalid move!";
    return 0;
}

void proceed() {
    prepare();

    if (isMoveValid() == 1) {
        if (cf == 'k') {
            remove("save.dat");
            cout<<endl<<"Player "<<turn<<" has won!";
            pressAnyKeyToContinue();
            turn = 0;
            return;
        }

        if (ci == 'P' && turn == 1 ? yf == 7 : yf == 0) {
            Game[xf][yf] = turn == 1 ? 'Q' : -'Q';
        } else {
            Game[xf][yf] = turn == 1 ? ci : -ci;
        }

        Game[xi][yi] = ' ';
        turn = turn == 1 ? 2 : 1;
    } else {
        pressAnyKeyToContinue();
    }
}

int main() {
    cout<<"Welcome to console Chess!";
    cout<<endl<<"Enter 00 any time to exit the game.";
    cout<<endl<<"Enter 0 to continue without loading the saved game: ";

    if (getche() != '0') {
        loadGame();
    }

    pressAnyKeyToContinue();

    while(turn != 0) {
        print();
        cout<<endl<<endl<<endl<<endl<<endl<<"Move from: ";

        xi = getch();
        yi = getch();

        if (xi == '0' && yi == '0') {
            cout<<endl<<"Enter 0 to exit without saving the game: ";

            if (getche() != '0') {
                saveGame();
            }

            pressAnyKeyToContinue();
            exit(0);
        } else if (prepareIntial() == 0) {
            pressAnyKeyToContinue();
            continue;
        }

        cout<<endl<<"Move to: ";

        xf = getch();
        yf = getch();

        if (prepareFinal() == 0) {
            pressAnyKeyToContinue();
            continue;
        }

        proceed();
    }

    return 0;
}
