#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <future>
#include <string>
#include <vector>
#include <cctype>
#include <iomanip>
#include <pthread.h>

using namespace std;

void locate(int x,int y)
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD C;
    C.X=(SHORT)x;
    C.Y=(SHORT)y;
    SetConsoleCursorPosition(H,C);

}

int getx()
  {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (!GetConsoleScreenBufferInfo(
         GetStdHandle( STD_OUTPUT_HANDLE ),
         &csbi
         ))
    return -1;
  return csbi.dwCursorPosition.X;
  }

int gety()
  {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (!GetConsoleScreenBufferInfo(
         GetStdHandle( STD_OUTPUT_HANDLE ),
         &csbi
         ))
    return -1;
  return csbi.dwCursorPosition.Y;
  }

void writeVertical(string text, int x, int y)
{
    if(text.size() <=0)
        return;
    for(int i = 0 ; i < (int) (text.size()-1) ; i++)
    {
        locate(x,y+i);
        cout << text.at(i);
    }
}

bool disableCursor()
{
    CONSOLE_CURSOR_INFO cci;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleCursorInfo(hConsole, &cci);

    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cci);
    return false;
}

bool enableCursor()
{
    CONSOLE_CURSOR_INFO cci;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleCursorInfo(hConsole, &cci);

    cci.bVisible = TRUE;
    SetConsoleCursorInfo(hConsole, &cci);
    return true;
}

bool displayPos()
{
    int saveX = getx();
    int saveY = gety();
    locate(1,27);
    cout << saveX << "/" << saveY << "  ";
    locate(saveX,saveY);
    //std::this_thread::sleep_for(std::chrono::seconds(2));
    return true;
}

bool hidePos()
{
    int saveX = getx();
    int saveY = gety();
    locate(0,22);
    cout << "                    ";
    locate(saveX,saveY);
    return false;
}

void erasePers()
{
    locate(getx()-4 , gety()-1);
    cout << "    ";
    locate(getx()-4 , gety()+1);
    cout << "    ";
}
void createPers()
{
    locate(getx()-4 , gety()-1);
    cout << "/@o-";
    locate(getx()-4 , gety()+1);
    cout << "<_/ ";
}

void loadFirstScreen(bool debugInfo)
{
    system ("mode con cols=50 lines=33");
    system("title Flappy Bird vConsole");
    writeVertical("||||||||||||||||||||||||||||||||||",0,0);
    writeVertical("||||||||||||||||||||||||||||||||||",49,0);
    locate(1,0);
    cout << "------------------------------------------------";
    locate(1,3);
    cout << "------------------------------------------------";
    locate(22,1);
    cout << "Flappy";
    locate(23,2);
    cout << "Bird";
    locate(41,1);
    cout << "SCORE :";
    locate(1,26);
    cout << "Infos----------------------------Menu-----------";
    writeVertical("||||||",33,27);
    locate(1,32);
    cout << "------------------------------------------------";
    if(debugInfo)
    {
        locate(1,27);
        cout << "15/14";
        locate(1,28);
        cout << "c = false";
        disableCursor();
        locate(1,29);
        cout << "gm = false";
        locate(1,30);
        cout << "speed = 0";
        locate(1,31);
        cout << "cm = false";
        locate(19,27);
        cout << "e+ = 0.00";
        locate(19,28);
        cout << "e- = 0.00";
        locate(19,29);
        cout << "[0/0]";
        locate(19,30);
        cout << "[0/0]";
        locate(19,31);
        cout << "[0/0]";
    }
    locate(36,27);
    cout << "continue";
    locate(36,28);
    cout << "cheatcode";
    locate(36,29);
    cout << "options";
    locate(36,30);
    cout << "help";
    locate(36,31);
    cout << "exit";

    locate(15,14);
    createPers();
    locate(0,0);
    locate(15,14);
}

void moveUp()
{
    erasePers();
    locate(getx(),gety()-1);
    createPers();
}
void moveDown()
{
    erasePers();
    locate(getx(),gety()+1);
    createPers();
}

int randomNb(int mini, int maxi)
{
    return rand() % max(mini,maxi) + min(mini,maxi);
}

void createPipe(int x, int sizeBas, int ecartTuyau)
{

    int saveX = getx();
    int saveY = gety();
    int y;

    int sizeHaut = 22-ecartTuyau-sizeBas;

    string hautTuyau1("[     ]");
    string hautTuyau2("[-----]");

    string bordDB("");
    for(int i = 0; i < sizeBas-1; i++)
    {
        bordDB += "]";
    }
    string bordGB("");
    for(int i = 0; i < sizeBas-1; i++)
    {
        bordGB += "[";
    }

    string bordDH("");
    for(int i = 0; i < sizeHaut-1; i++)
    {
        bordDH += "]";
    }
    string bordGH("");
    for(int i = 0; i < sizeHaut-1; i++)
    {
        bordGH += "[";
    }




    int tempX = x;
    int finalHautSize = 7;
    while(tempX + 6 > 49)
    {
        tempX--;
        finalHautSize --;
    }
    if(finalHautSize >= 0)
    {
        hautTuyau1 = hautTuyau1.substr(0,finalHautSize);
        hautTuyau2 = hautTuyau2.substr(0, finalHautSize);
    }
        // tuyau du bas
    y = 28;

    if(x < 48)
        writeVertical(bordGB, x, y-sizeBas);
    if(x+4<48)
        writeVertical(bordDB, x+4, y-sizeBas);

    if(finalHautSize >=0)
    {
        locate(x-1,y-sizeBas-1);
        cout << hautTuyau1;
        locate(x-1,y-sizeBas-2);
        cout << hautTuyau2;
    }

    // tuyau du haut
    y = 4;

    if(x < 48)
        writeVertical(bordGH, x, y);
    if(x+4<48)
        writeVertical(bordDH, x+4, y);

    if(finalHautSize >=0)
    {
        locate(x-1,y+sizeHaut-2);
        cout << hautTuyau1;
        locate(x-1,y+sizeHaut-1);
        cout << hautTuyau2;
    }


    locate(saveX,saveY);
}

void erasePipe(int x, int sizeBas, int ecartTuyau)
{

    int saveX = getx();
    int saveY = gety();
    int y;

    int sizeHaut = 22-ecartTuyau-sizeBas;

    string hautTuyau1("       ");
    string hautTuyau2("       ");

    string bordDB("");
    for(int i = 0; i < sizeBas-1; i++)
    {
        bordDB += " ";
    }
    string bordGB("");
    for(int i = 0; i < sizeBas-1; i++)
    {
        bordGB += " ";
    }

    string bordDH("");
    for(int i = 0; i < sizeHaut-1; i++)
    {
        bordDH += " ";
    }
    string bordGH("");
    for(int i = 0; i < sizeHaut-1; i++)
    {
        bordGH += " ";
    }

    int tempX = x;
    int finalHautSize = 7;
    while(tempX + 6 > 49)
    {
        tempX--;
        finalHautSize --;
    }
    if(finalHautSize >= 0)
    {
        hautTuyau1 = hautTuyau1.substr(0,finalHautSize);
        hautTuyau2 = hautTuyau2.substr(0, finalHautSize);
    }
        // tuyau du bas
    y = 28;

    if(x < 48)
        writeVertical(bordGB, x, y-sizeBas);
    if(x+4<48)
        writeVertical(bordDB, x+4, y-sizeBas);

    if(finalHautSize >=0)
    {
        locate(x-1,y-sizeBas-1);
        cout << hautTuyau1;
        locate(x-1,y-sizeBas-2);
        cout << hautTuyau2;
    }

    // tuyau du haut
    y = 4;
    if(x < 48)
        writeVertical(bordGH, x, y);
    if(x+4<48)
        writeVertical(bordDH, x+4, y);

    if(finalHautSize >=0)
    {
        locate(x-1,y+sizeHaut-2);
        cout << hautTuyau1;
        locate(x-1,y+sizeHaut-1);
        cout << hautTuyau2;
    }


    locate(saveX,saveY);
}

void beepingFonc(void *arg)
{
    Beep(1800,50);
    Beep(2400,500);

    (void) arg;
    pthread_exit(NULL);
}

void writeScreen(int tuyau[], int ecartTuyau, int saveX, int saveY)
{
    for(int i = 4; i < 26; i++)
    {
        locate(1,i);
        cout << "                                                ";
    }
    writeVertical("||||||||||||||||||||||||||||||||||",0,0);
    writeVertical("||||||||||||||||||||||||||||||||||",49,0);

    if(tuyau[0] < 48 && tuyau[0] > 1)
        createPipe(tuyau[0],tuyau[1],ecartTuyau);
    if(tuyau[2] < 48 && tuyau[2] > 1)
        createPipe(tuyau[2],tuyau[3],ecartTuyau);
    if(tuyau[4] < 48 && tuyau[4] > 1)
        createPipe(tuyau[4],tuyau[5],ecartTuyau);

    locate(saveX,saveY);
    createPers();
}




int main()
{
    srand(time(NULL));

    bool run(true);
    bool paused(false);
    bool godMod(false);
    bool cursor(disableCursor());
    bool waitingForFirstTap(true);
    bool waitingTapFromCmd(false);
    bool cheatMove(false);
    bool isCollided(false);
    bool debugInfo(false);

    int saveX, saveY;
    int screenSpeedMs(80);
    int c;
    int ecartTuyau = 8;
    int entreDeux = 18;
    int score = 0;
    int slowAll = 0;
    const int constMaxArg = 5;
    const int constMinArg = 3;

    double vitesseApresJump = 0.95;
    double vitesse = -1;
    double enPlus = 0;
    double enMoins = 0;



    string cmd;

    int tuyau[6]; // [x1,sizeBas1,x2,sizeBas2

    auto waitingFly = chrono::steady_clock::now();
    auto refreshDisplay = chrono::steady_clock::now();
    auto avanceEcran = chrono::steady_clock::now();
    auto now = chrono::steady_clock::now();
    auto move = chrono::steady_clock::now();
    auto lowDisplay = chrono::steady_clock::now();

    loadFirstScreen(false);

    tuyau[0] = 49;
    tuyau[1] = randomNb(constMinArg,22-ecartTuyau-constMaxArg);
    tuyau[2] = 49 + entreDeux + 6;
    tuyau[3] = randomNb(constMinArg,22-ecartTuyau-constMaxArg);
    tuyau[4] = 49 + 2*entreDeux + 2*6;
    tuyau[5] = randomNb(constMinArg,22-ecartTuyau-constMaxArg);

    while(run)
    {
        now = chrono::steady_clock::now();

        if(!paused )
        {
            saveX = getx();
            saveY = gety();
        }
        //TOUCHES ---
        if(kbhit())
        {
            c = getch();
            if(c == 224 && !paused) // movements in game
            {
                if(waitingForFirstTap)
                    waitingForFirstTap = false;
                if(waitingTapFromCmd)
                    waitingTapFromCmd = false;
                c = getch();
                if(!cheatMove) // no cheat, regular movement
                {
                    if(c == 72) // haut  = 0
                    {
                        vitesse = vitesseApresJump;
                    }
                }
                else // cheat, make player able to move char by char
                {
                    if(c == 72)
                    {
                        saveY-=1;
                        moveUp();
                    }
                    else if(c==80)
                    {
                        saveY+=1;
                        moveDown();
                    }

                }
            }
            else if(c == 224 && paused && !isCollided) // movements in menu
            {
                c = getch();
                if(c == 72) // haut
                {
                    if(gety() != 27)
                    {
                        locate(getx()-1,gety());
                        cout << " ";
                        locate(getx()-1,gety()-1);
                        cout << "*";
                    }
                    else if(gety()==27)
                    {
                        locate(getx()-1,gety());
                        cout << " ";
                        locate(getx()-1,31);
                        cout << "*";
                    }
                }
                else if(c == 80) //bas
                {
                    if(gety() != 31)
                    {
                        locate(getx()-1,gety());
                        cout << " ";
                        locate(getx()-1,gety()+1);
                        cout << "*";
                    }
                    else if(gety()==31)
                    {
                        locate(getx()-1,gety());
                        cout << " ";
                        locate(getx()-1,27);
                        cout << "*";
                    }
                }
            }
            else if(c == 224 && paused && isCollided) // movement after loose
            {
                c = getch();
                if(c != 80 && c !=72)
                {
                    if(getx() == 28)
                    {
                            locate(27,13);
                            cout << " ";
                            locate(20,13);
                            cout << "*";
                    }
                    else if(getx() == 21)
                    {
                        locate(20,13);
                        cout << " ";
                        locate(27,13);
                        cout << "*";
                    }
                }
            }

            else if(c == 99) // c : toggle cursor visibility
            {
                locate(1,28);
                if(cursor)
                {
                    cursor = disableCursor();
                    if(debugInfo) cout << "c = false";
                }
                else
                {
                    cursor = enableCursor();
                    if(debugInfo) cout << "c = true ";
                }
                locate(saveX,saveY);
            }
            else if(c == 114) // r : clear screen in order to refresh it + de-activate godMod, re-hide Cursor and resize window
            {
                for(int i = 0; i < 25; i++)
                {
                    locate(0,i);
                    cout << "                                                                                \n";
                }
                loadFirstScreen(debugInfo);
                godMod = false;
                cursor = disableCursor();
                cheatMove = false;
                tuyau[0] = 49;
                tuyau[1] = randomNb(constMinArg,22-ecartTuyau-constMaxArg);
                tuyau[2] = 49 + entreDeux + 6;
                tuyau[3] = randomNb(constMinArg,22-ecartTuyau-constMaxArg);
                tuyau[4] = 49 + 2*entreDeux + 2*6;
                tuyau[5] = randomNb(constMinArg,22-ecartTuyau-constMaxArg);
                score = 0;
                waitingForFirstTap = true;
                isCollided = false;
                paused = false;
                locate(15,14);
            }
            else if(c == 103) // g : toggle god-mod = can move throught walls
            {
                locate(1,29);
                if(godMod)
                {
                    godMod = false;
                    if(debugInfo) cout << "gm = false";
                }
                else
                {
                    godMod = true;
                    if(debugInfo) cout << "gm = true ";
                }
                locate(saveX,saveY);
            }
            else if(c ==  'm') // m : toggle cheatMove : walk pixel by pixel
            {
                locate(1,31);
                if(cheatMove)
                {
                    cheatMove = false;
                    if(debugInfo) cout << "cm = false";
                }
                else
                {
                    cheatMove = true;
                    if(debugInfo) cout << "cm = true ";
                }
                locate(saveX,saveY);
            }
            /*else if(c == 'a')
            {
                int a[2];
                a[0] = getx();
                a[1] = gety();
                writeScreen(tuyau, ecartTuyau, saveX, saveY);
                locate(a[0],a[1]);
            }*/
            else if(c ==  'd') // d : hide/show debug info
            {
                if(debugInfo)
                {
                    debugInfo = false;
                    for(int i = 27; i <=31; i++)
                    {
                        locate(1,i);
                        cout << "                                ";
                    }
                    locate(saveX,saveY);
                }
                else
                {
                    debugInfo = true;
                    locate(1,27);
                    cout << "15/14";
                    locate(1,28);
                    if(cursor) { cout << "c = true"; } else {cout << "c = false";}
                    locate(1,29);
                    if(godMod) { cout << "gm = true"; } else {cout << "gm = false";}
                    locate(1,30);
                    cout << "speed = " << vitesse;
                    locate(1,31);
                    if(cheatMove) { cout << "cm = true"; } else {cout << "cm = false";}
                    locate(19,27);
                    cout << "e+ = " << enPlus;
                    locate(19,28);
                    cout << "e- = " << enMoins;
                    locate(19,29);
                    cout << "[" << tuyau[0] << "/" << tuyau[1] << "]  ";
                    locate(19,30);
                    cout << "[" << tuyau[2] << "/" << tuyau[3] << "]  ";
                    locate(19,31);
                    cout << "[" << tuyau[4] << "/" << tuyau[5] << "]  ";

                    locate(saveX,saveY);

                }
            }
            else if(c == 27) // esc : pause, player cannot move and cursor goes to the menu on the right
            {
                if(!paused)
                {
                    paused = true;
                    locate(21,13);
                    cout << "/------\\";
                    locate(21,14);
                    cout << "|PAUSED|";
                    locate(21,15);
                    cout << "\\------/";
                    locate(35,27);
                    cout << "*";
                }
                else
                {
                    paused = false;
                    locate(getx()-1,gety());
                    cout << " ";
                    locate(21,13);
                    cout << "        ";
                    locate(21,14);
                    cout << "        ";
                    locate(21,15);
                    cout << "        ";

                    locate(saveX,saveY);

                    //forceRefresh = true;
                }
            }
            else if(c == 13) // enter : select in menu
            {
                if(gety() == 27) // continue
                {
                    paused = false;
                    locate(getx()-1,gety());
                    cout << " ";
                    locate(21,13);
                    cout << "        ";
                    locate(21,14);
                    cout << "        ";
                    locate(21,15);
                    cout << "        ";
                    locate(saveX,saveY);
                }
                else if(gety() == 28) // cheatCode
                {
                    for(int i = 4; i < 26; i++)
                    {
                        locate(1,i);
                        cout << "                                                ";
                    }

                    locate(1,4);
                    cout << "Commande :";

                    /*locate(34,27);
                    cout << " Commande :    ";
                    locate(34,28);
                    cout << "               ";
                    locate(34,29);
                    cout << "               ";
                    locate(34,30);
                    cout << "               ";
                    locate(34,31);
                    cout << "               ";*/
                    locate(1,5);
                    enableCursor();
                    cin >> cmd;
                    for(auto& x: cmd)
                        x = toupper(x);

                    if(cmd == "ECARTTUYAU")
                    {
                        locate(1,6);
                        cout << "value : ";
                        while(!(cin >> ecartTuyau))
                        {
                            locate(1,6);
                            cout << "INVALIDE                                        ";
                            for(int i = 7; i < 26; i++)
                            {
                                locate(1,i);
                                cout << "                                                ";
                            }
                            writeVertical("||||||||||||||||||||||||||||||||||",0,0);
                            writeVertical("||||||||||||||||||||||||||||||||||",49,0);
                            Sleep(500);
                            locate(1,6);
                            cout << "value : ";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            locate(9,6);
                        }
                    }
                    else if(cmd == "VITESSEECRAN")
                    {
                        locate(1,6);
                        cout << "value : ";
                        while(!(cin >> screenSpeedMs))
                        {
                            locate(1,6);
                            cout << "INVALIDE                                        ";
                            for(int i = 7; i < 26; i++)
                            {
                                locate(1,i);
                                cout << "                                                ";
                            }
                            writeVertical("||||||||||||||||||||||||||||||||||",0,0);
                            writeVertical("||||||||||||||||||||||||||||||||||",49,0);
                            Sleep(500);
                            locate(1,6);
                            cout << "value : ";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            locate(9,6);
                        }
                    }
                    else if(cmd == "LIST" || cmd == "HELP")
                    {
                        vector<string> listeCommande;

                        listeCommande.push_back("ECARTTUYAU");
                        listeCommande.push_back("VITESSEECRAN");
                        listeCommande.push_back("JUMPFORCE");
                        listeCommande.push_back("ENTREDEUX");
                        listeCommande.push_back("SLOWALL");
                        listeCommande.push_back("WAITFORTAP / WAITTAP");
                        listeCommande.push_back("LIST / HELP");
                        listeCommande.push_back("RESETPARAM");
                        listeCommande.push_back("DEFAULT");
                        listeCommande.push_back("");
                        listeCommande.push_back("Appuyez sur une touche pour continuer...");

                        for(int i = 4; i < 26; i++)
                        {
                            locate(1,i);
                            cout << "                                                ";
                        }

                        for(int i = 0; i < listeCommande.size(); i++)
                        {
                            locate(1,i+4);
                            cout << listeCommande[i];
                        }
                        c = 0;
                        while(c=0);
                        {
                            Sleep(10);
                            c = getch();
                        }
                        locate(34,32);
                    }
                    else if(cmd == "DEFAULT")
                    {
                        vector<string> listeCommande;

                        listeCommande.push_back("ECARTTUYAU   = 8");
                        listeCommande.push_back("VITESSEECRAN = 80");
                        listeCommande.push_back("JUMPFORCE    = 0.95");
                        listeCommande.push_back("ENTREDEUX    = 18");
                        listeCommande.push_back("SLOWALL      = 0");
                        listeCommande.push_back("");
                        listeCommande.push_back("Appuyez sur une touche pour continuer...");

                        for(int i = 4; i < 26; i++)
                        {
                            locate(1,i);
                            cout << "                                                ";
                        }

                        for(int i = 0; i < listeCommande.size(); i++)
                        {
                            locate(1,i+4);
                            cout << listeCommande[i];
                        }
                        c = 0;
                        while(c=0);
                        {
                            Sleep(10);
                            c = getch();
                        }
                        locate(34,32);
                    }
                    else if(cmd == "FORCEJUMP" || cmd == "JUMPFORCE")
                    {
                        locate(1,6);
                        cout << "value : ";
                        while(!(cin >> vitesseApresJump))
                        {
                            locate(1,6);
                            cout << "INVALIDE                                        ";
                            for(int i = 7; i < 26; i++)
                            {
                                locate(1,i);
                                cout << "                                                ";
                            }
                            writeVertical("||||||||||||||||||||||||||||||||||",0,0);
                            writeVertical("||||||||||||||||||||||||||||||||||",49,0);
                            Sleep(500);
                            locate(1,6);
                            cout << "value : ";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            locate(9,6);
                        }
                    }
                    else if(cmd == "ENTREDEUX")
                    {
                        locate(1,6);
                        cout << "value : ";
                        while(!(cin >> entreDeux))
                        {
                            locate(1,6);
                            cout << "INVALIDE                                        ";
                            for(int i = 7; i < 26; i++)
                            {
                                locate(1,i);
                                cout << "                                                ";
                            }
                            writeVertical("||||||||||||||||||||||||||||||||||",0,0);
                            writeVertical("||||||||||||||||||||||||||||||||||",49,0);
                            Sleep(500);
                            locate(1,6);
                            cout << "value : ";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            locate(9,6);
                        }
                    }
                    else if(cmd == "SLOWALL")
                    {
                        locate(1,6);
                        cout << "value : ";
                        while(!(cin >> slowAll))
                        {
                            locate(1,6);
                            cout << "INVALIDE                                        ";
                            for(int i = 7; i < 26; i++)
                            {
                                locate(1,i);
                                cout << "                                                ";
                            }
                            writeVertical("||||||||||||||||||||||||||||||||||",0,0);
                            writeVertical("||||||||||||||||||||||||||||||||||",49,0);
                            Sleep(500);
                            locate(1,6);
                            cout << "value : ";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            locate(9,6);
                        }

                    }
                    else if(cmd == "RESETPARAM")
                    {
                        slowAll = 0;
                        screenSpeedMs = 80;
                        ecartTuyau = 8;
                        entreDeux = 18;
                        vitesseApresJump = 0.95;

                    }
                    else if(cmd == "WAITFORTAP" || cmd ==  "WAITTAP")
                    {
                        waitingTapFromCmd = true;
                    }
                    disableCursor();
                    locate(34,27);
                    cout << "  continue     ";
                    locate(34,28);
                    cout << "  cheatcode    ";
                    locate(34,29);
                    cout << "  options      ";
                    locate(34,30);
                    cout << "  help         ";
                    locate(34,31);
                    cout << "  exit         ";

                    writeScreen(tuyau, ecartTuyau, saveX, saveY);
                    locate(21,13);
                    cout << "/------\\";
                    locate(21,14);
                    cout << "|PAUSED|";
                    locate(21,15);
                    cout << "\\------/";
                    locate(35,28);
                    cout << "*";
                }
                else if(gety() == 29) // options
                {
                    locate(34,27);
                    cout << " Options :     ";
                    locate(34,28);
                    cout << "               ";
                    locate(34,29);
                    cout << " Pas d\'option ";
                    locate(34,30);
                    cout << "    desole     ";
                    locate(34,31);
                    cout << "               ";
                    Sleep(2500);
                    locate(34,27);
                    cout << "  continue     ";
                    locate(34,28);
                    cout << "  cheatcode    ";
                    locate(34,29);
                    cout << "  options      ";
                    locate(34,30);
                    cout << "  help         ";
                    locate(34,31);
                    cout << "  exit         ";
                    locate(35,29);
                    cout << "*";
                }
                else if(gety() == 30) // help
                {
                    locate(34,27);
                    cout << " Aide :        ";
                    locate(34,28);
                    cout << "Fleche du haut ";
                    locate(34,29);
                    cout << "pour sauter et ";
                    locate(34,30);
                    cout << "eviter les     ";
                    locate(34,31);
                    cout << "tuyaux !       ";
                    Sleep(3500);
                    locate(34,27);
                    cout << "  continue     ";
                    locate(34,28);
                    cout << "  cheatcode    ";
                    locate(34,29);
                    cout << "  options      ";
                    locate(34,30);
                    cout << "  help         ";
                    locate(34,31);
                    cout << "  exit         ";
                    locate(35,30);
                    cout << "*";

                }
                else if(gety() == 31) // exit
                {
                    run = false;
                }
                else if(gety() == 13) // menu game over
                {
                    if(getx() == 21) // respawn
                    {
                        for(int i = 0; i < 25; i++)
                        {
                            locate(0,i);
                            cout << "                                                                                \n";
                        }
                        loadFirstScreen(debugInfo);
                        godMod = false;
                        cursor = disableCursor();
                        cheatMove = false;
                        tuyau[0] = 49;
                        tuyau[1] = randomNb(constMinArg,22-ecartTuyau-constMaxArg);
                        tuyau[2] = 49 + entreDeux + 6;
                        tuyau[3] = randomNb(constMinArg,22-ecartTuyau-constMaxArg);
                        tuyau[4] = 49 + 2*entreDeux + 2*6;
                        tuyau[5] = randomNb(constMinArg,22-ecartTuyau-constMaxArg);
                        score = 0;
                        waitingForFirstTap = true;
                        isCollided = false;
                        paused = false;
                        locate(15,14);
                    }
                    else if(getx() == 28) // quit
                    {
                        run = false;
                    }
                }
            }
        }
        //----

        //TIMERS ----
        if(chrono::duration <double, milli> (now - refreshDisplay).count() > 50 && !paused)
        {
            if(debugInfo) displayPos();
            createPers();
            refreshDisplay = chrono::steady_clock::now();

        }
        if(chrono::duration <double, milli> (now - move).count() > 20 + slowAll && !paused && !waitingForFirstTap && !waitingTapFromCmd && !cheatMove)
        {
            if(vitesse > -0.8)
                vitesse -= 0.05;
            move = chrono::steady_clock::now();
            locate(1,30);
            if(debugInfo) cout << "speed = " << fixed << setprecision(2) << vitesse << " ";
            locate(saveX,saveY);

            if(vitesse >=0.5)
            {
                enPlus += 0.7;
            }
            else if(vitesse < 0.5 && vitesse >=0.04)
            {
                enPlus += 0.2;
            }
            else if(vitesse < 0.04 && vitesse >= -0.15)
            {}
            else if(vitesse < -0.15 && vitesse >= -0.37)
            {
                enMoins += 0.08;
            }
            else if(vitesse < -0.37 && vitesse >= -0.8)
            {
                enMoins += 0.28;
            }
            else if(vitesse < -0.8)
            {
                enMoins += 0.5;
            }

            locate(19,27);
            if(debugInfo) cout << "e+ = " << enPlus;
            locate(19,28);
            if(debugInfo) cout << "e- = " << enMoins;
            locate(saveX,saveY);

            if(enPlus != 0)
            {
                if(enPlus >= 2)
                {
                    enPlus = 0;
                }
                else if(enPlus >= 1 && enPlus < 2 && (saveY > 5 || godMod))
                {
                    moveUp();
                    enPlus = 0;
                }
            }
            if(enMoins != 0)
            {
                if(enMoins >= 2)
                {
                    enMoins = 0;
                }
                else if(enMoins >= 1 && enMoins < 2 && (saveY < 25 || godMod))
                {
                    moveDown();
                    enMoins = 0;
                }
            }

        }
        if(chrono::duration <double, milli> (now - lowDisplay).count() > 500)
        {
            /*locate(90,0);
            cout << "verif actif : x = " << saveX << "  ";
            for(int i = 0;i < queue.size(); i++)
            {
                locate(90,4+i);
                cout << queue[i] << "  ";
            }
            locate(saveX,saveY);*/
            lowDisplay = chrono::steady_clock::now();
        }
        if(chrono::duration <double, milli> (now - waitingFly).count() > 400 && !paused && (waitingForFirstTap || waitingTapFromCmd))
        {
            waitingFly = chrono::steady_clock::now();
            if(gety() <= 14)
                moveDown();
            else
                moveUp();
        }
        if(chrono::duration <double, milli> (now - avanceEcran).count() > screenSpeedMs + slowAll && !paused && !waitingForFirstTap)
        {
            avanceEcran = chrono::steady_clock::now();
            // effacer les anciens tuyaux
            if(tuyau[0] < 48 && tuyau[0] > 1)
                erasePipe(tuyau[0],tuyau[1],ecartTuyau);
            if(tuyau[2] < 48 && tuyau[2] > 1)
                erasePipe(tuyau[2],tuyau[3],ecartTuyau);
            if(tuyau[4] < 48 && tuyau[4] > 1)
                erasePipe(tuyau[4],tuyau[5],ecartTuyau);

            // faire avancer les tuyau (x -1 <=> décalé vers la gauche)
            tuyau[0] -= 1;
            tuyau[2] -= 1;
            tuyau[4] -= 1;

            // afficher les tuyaux, si ils sont visibles
            if(tuyau[0] < 48 && tuyau[0] > 1)
                createPipe(tuyau[0],tuyau[1],ecartTuyau);
            if(tuyau[2] < 48 && tuyau[2] > 1)
                createPipe(tuyau[2],tuyau[3],ecartTuyau);
            if(tuyau[4] < 48 && tuyau[4] > 1)
                createPipe(tuyau[4],tuyau[5],ecartTuyau);

            // créer les nouveaux tuyaux
            if(tuyau[0] <= 0)
            {
                tuyau[0] = tuyau[4] + entreDeux + 6;
                tuyau[1] = randomNb(constMinArg,22-constMaxArg-ecartTuyau);
            }
            if(tuyau[2] <= 0)
            {
                tuyau[2] = tuyau[0] + entreDeux + 6;
                tuyau[3] = randomNb(constMinArg,22-constMaxArg-ecartTuyau);
            }
            if(tuyau[4] <= 0)
            {
                tuyau[4] = tuyau[2] + entreDeux + 6;
                tuyau[5] = randomNb(constMinArg,22-constMaxArg-ecartTuyau);
            }

            locate(19,29);
            if(debugInfo) cout << "[" << tuyau[0] << "/" << tuyau[1] << "]  ";
            locate(19,30);
            if(debugInfo) cout << "[" << tuyau[2] << "/" << tuyau[3] << "]  ";
            locate(19,31);
            if(debugInfo) cout << "[" << tuyau[4] << "/" << tuyau[5] << "]  ";

            // score + son si un jour fix
            if(tuyau[0] == 10 || tuyau[2] == 10 || tuyau[4] == 10)
            {
                //CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) beepingFonc, 0, 0,NULL);
                score += 1;
                locate(41,2);
                cout << score;
                locate(saveX,saveY);
            }

            // collisions
            if(!godMod)
            {
                if(tuyau[0] <= 14 && tuyau[0] >= 6)
                {
                    if(saveY >= 26 - tuyau[1])
                    {
                        paused = true;
                        isCollided = true;
                    }
                    if(saveY <= 1 + 25-ecartTuyau-tuyau[1])
                    {
                        paused = true;
                        isCollided = true;
                    }
                }
                if(tuyau[2] <= 14 && tuyau[2] >= 6)
                {
                    if(saveY >= 26 - tuyau[3])
                    {
                        paused = true;
                        isCollided = true;
                    }
                    if(saveY <= 1 + 25-ecartTuyau-tuyau[3])
                    {
                        paused = true;
                        isCollided = true;
                    }
                }
                if(tuyau[4] <= 14 && tuyau[4] >= 6)
                {
                    if(saveY >= 26 - tuyau[5])
                    {
                        paused = true;
                        isCollided = true;
                    }
                    if(saveY <= 1 + 25-ecartTuyau-tuyau[5])
                    {
                        paused = true;
                        isCollided = true;
                    }
                }
                if(saveY == 25)
                {
                    paused = true;
                    isCollided = true;
                }
            }
            if(!isCollided)
            {
                locate(saveX,saveY);

            }
            else
            {
                locate(saveX,saveY);
                locate(getx()-4 , gety()-1);
                cout << "/@x-";
                locate(getx()-4 , gety()+1);
                cout << "<_/ ";

                locate(16,10);
                cout << "/-----------------\\";
                locate(16,11);
                cout << "|      PERDU      |";
                locate(16,12);
                cout << "|    Rejouer ?    |";
                locate(16,13);
                cout << "|    oui    non   |";
                locate(16,14);
                cout << "\\-----------------/";
                locate(20,13);
                cout << "*";

            }
        }


        //----
    }

}

