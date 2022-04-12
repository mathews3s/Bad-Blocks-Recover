#include "Libers.h"
#include "Classes.h"

int main()
{
    system("PAUSE");
    system("CLS");
    int Succes = NULL;
    char Answer = NULL;
    My_Device Disk;
    Succes = Disk.Create_Me();
    if (!Succes)
        return 0;
    else 
        Succes = NULL;
    Succes = Disk.Get_My_Info();
    if (!Succes)
        return 0;
    else
        Succes = NULL;
    while (1)
    {
        cout << "Do tou want to diagnostic your disk? (y/n)" << endl;
        cout << "Attention!! After agreement your disk will be checked for response and bad sectors" << endl;
        cout << "Choice: ";
        try
        {
            cin >> Answer;
            if (cin.fail() || (Answer != 'y' && Answer != 'n'))
            {
                system("CLS");
                throw My_Exceptions("Warning: input isn't 'y' or 'n' !! Repeat:");
            }
            else break;
        }
        catch (My_Exceptions& tmp)
        {
            cout << endl << tmp.getproblem() << endl;
            cin.clear();
            rewind(stdin);
        }
    }
    switch (Answer)
    {
    case 'y':
        system("CLS");
        system("PAUSE");
        system("CLS");
        Succes = Disk.Device_Diagnostic.Start();

        break;
    case 'n':
        system("CLS");
        int i = 0;
        char str[] = "Exiting . . .";
        while (str[i] != '\0')
        {
            cout << str[i];
            i++;
            Sleep(100);
        }
        return 0;
        break;
    } 
    return 0;
}


