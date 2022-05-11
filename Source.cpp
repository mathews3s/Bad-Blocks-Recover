#include "Libers.h"
#include "Classes.h"

int main(int argc, char** argv)
{
    Message_Logo();
    LPCTSTR Drive_Name;
    Device_Master Device;
    char User_Answer = NULL;
    if (argc <= 1)
    {
        cout << "Attention: any number of physical drive wasn't specified." << endl;
        cout << "'PhysicalDrive0' will be used for diagnostic." << endl;
        Drive_Name = L"\\\\.\\PhysicalDrive0";
        if (!Device.Create_Device(Drive_Name))
            return 0;
    }
    else
    {
        string Name_Path = "\\\\.\\PhysicalDrive";
        string Number_Path = argv[1];
        string Full_Name = Name_Path + Number_Path;
        CA2T temp(Full_Name.c_str());
        Drive_Name = temp;
        cout << "'PhysicalDrive"<< argv[1] << "' will be used for diagnostic." << endl;
        if (!Device.Create_Device(Drive_Name))
            return 0;
    }
    Message_Start();
    system("CLS");
    while (1)
    {
        Message_Logo();
        if (!Device.Get_Device_Info())
            return 0;
        Message_Diagnostic();
        try
        {
            cin >> User_Answer;
            if (cin.fail() || (User_Answer != 'y' && User_Answer != 'n'))
            {
                system("CLS");
                throw Exceptions("Warning: input isn't 'y' or 'n' !! \n(Push Enter and repeat)");
            }
            else break;
        }
        catch (Exceptions& Error)
        {
            cout << Error.Get_Problem() << endl;
            system("PAUSE");
            system("CLS");
            cin.clear();
            rewind(stdin);
        }
    }
    switch (User_Answer)
    {
    case 'y':
        system("CLS");
        Device.Start_Diagnostic_Device();
        break;
    case 'n':
        system("CLS");
        Message_Exit();
        return 0;
        break;
    } 
    Message_Exit();
    return 0;
}

void Message_Diagnostic()
{
    cout << "Do tou want to diagnostic your disk? (y/n)" << endl;
    cout << "Attention!! After agreement your disk will be checked for response and bad sectors" << endl;
    cout << "Choice: ";
}

void Message_Start()
{
    int index = NULL;
    cout << endl;
    char string[] = "Getting info about your drive . . . . . . ";
    while (string[index] != '\0')
    {
        cout << string[index];
        index++;
        Sleep(100);
    }
}

void Message_Exit()
{
    Message_Logo();
    int index = 0;
    char string[] = "Exiting . . .";
    while (string[index] != '\0')
    {
        cout << string[index];
        index++;
        Sleep(100);
    }
}
