#include"Libers.h"

class My_Exceptions
{
private:
    string m_err;
public:
    My_Exceptions(string err) { m_err = err; }
    ~My_Exceptions() {}
    string getproblem() { return m_err; }
};

class Remap
{
private:
    HANDLE Pacient;
    DWORD Current_Position;                              // Current position in file;
    DWORD Buffer_Size;                                   // Size of buffer for reading;
    BYTE* Buffer;                                        // Buffer for reading;
    BOOL Read_Result;                                    // Result of reading;  
    DWORD Bytes_Readed;                                  // Bytes returned after reading;
    LONG Repeat = 10000;                                 // Number of iterations while trying call remap;
    DWORD Position_Now;                                   // Current position in file;
public:
    Remap()
    {
        Pacient = INVALID_HANDLE_VALUE;
        Current_Position = NULL;                               // Current position in file;
        Buffer_Size = NULL;                                    // Size of buffer for reading;
        Buffer = NULL;                                         // Buffer for reading;
        Read_Result = NULL;                                    // Result of reading;  
        Bytes_Readed = NULL;                                   // Bytes returned after reading;
    }
    Remap(HANDLE Device, BYTE* Buff, DWORD Buff_Size, DWORD Curr_Position )
    {
        Pacient = Device;
        Current_Position = Curr_Position;                      // Current position in file;
        Buffer_Size = Buff_Size;                               // Size of buffer for reading;
        Buffer = Buff;                                         // Buffer for reading;
        Read_Result = NULL;                                    // Result of reading;  
        Bytes_Readed = NULL;                                   // Bytes returned after reading;
    }
    ~Remap()
    {}
    int start()
    {
        DWORD Position_Now;                                        // Current position in file;
        while (Repeat != NULL)
        {
            Position_Now = SetFilePointer(Pacient, 0, NULL, FILE_CURRENT);                // Check current position;
            try
            {
                Read_Result = ReadFile(Pacient, Buffer, Buffer_Size, &Bytes_Readed, NULL);     // Read one sector;
                if (!Read_Result) throw My_Exceptions("ERROR! Problems while reading disk!");
            }
            catch (My_Exceptions& tmp)
            {
                cout << endl << tmp.getproblem() << endl;
                cout << "Error code: " << GetLastError() << endl;
                delete[] Buffer;
                Buffer = NULL;
                CloseHandle(Pacient);
                return 0;
            }
            SetFilePointer(Pacient, Current_Position, NULL, FILE_BEGIN);              // Set file position at back to read current sector again;
            Repeat--;
        }
        return 1;
    }
};

class Diagnostic
{
private:
    HANDLE Pacient;
    DWORD Current_Position;                              // Current position in file;
    ULONGLONG Distance_To_Move;                          // Distance witch we should pass;
    DWORD Buffer_Size;                                   // Size of buffer for reading;
    BYTE* Buffer;                                        // Buffer for reading;
    DOUBLE Seconds;                                      // Current time;
    DOUBLE Time_Out;                                     // How much seconds are passed;
    BOOL Read_Result;                                    // Result of reading;  
    DWORD Bytes_Readed;                                  // Bytes returned after reading;
    ULONGLONG Checked_Sectors;                           // Count of checked sectors;
    BOOL Remap_Result;                                   // Result of remap operation
    ULONGLONG Size;                                      // Size of disk to monitor reading operation
    Remap Remap_Device;
public:
    Diagnostic()
    {
        Pacient = INVALID_HANDLE_VALUE;
        Current_Position = NULL;                               // Current position in file;
        Distance_To_Move = NULL;                               // Distance witch we should pass;
        Buffer_Size = NULL;                                    // Size of buffer for reading;
        Buffer = NULL;                                         // Buffer for reading;
        Seconds = NULL;                                        // Current time;
        Time_Out = NULL;                                       // How much seconds are passed;
        Read_Result = NULL;                                    // Result of reading;  
        Bytes_Readed = NULL;                                   // Bytes returned after reading;
        Checked_Sectors = NULL;                                // Count of checked sectors;
        Remap_Result = NULL;                                   // Result of remap operation
        Size = NULL;                                           // Size of disk to monitor reading operation
        Remap_Device = Remap();
    }
    Diagnostic(HANDLE Device, ULONGLONG Device_Size)
    {
        Pacient = Device;
        Current_Position = NULL;                               // Current position in file;
        Distance_To_Move = NULL;                               // Distance witch we should pass;
        Buffer_Size = NULL;                                    // Size of buffer for reading;
        Buffer = NULL;                                         // Buffer for reading;
        Seconds = NULL;                                        // Current time;
        Time_Out = NULL;                                       // How much seconds are passed;
        Read_Result = NULL;                                    // Result of reading;  
        Bytes_Readed = NULL;                                   // Bytes returned after reading;
        Checked_Sectors = NULL;                                // Count of checked sectors;
        Remap_Result = NULL;                                   // Result of remap operation
        Size = Device_Size;                                    // Size of disk to monitor reading operation
        Remap_Device = Remap();
    }
    ~Diagnostic()
    {}
    void Put_Media(int x, int y, unsigned char* ch, int n, int color)// Function to print each checked sector;
    {
        DWORD r;
        CONSOLE_SCREEN_BUFFER_INFO Console;
        static HANDLE h;
        if (!h) h = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(h, &Console);
        Console.dwCursorPosition.X = x;
        Console.dwCursorPosition.Y = y;
        WriteConsoleOutputCharacterA(h, (char*)ch, n, Console.dwCursorPosition, &r);
        FillConsoleOutputAttribute(h, color, n, Console.dwCursorPosition, &r);
    }
    int Get_Status()                                                // This function response-simulator;
    {
        srand((unsigned)time(NULL));
        int response = 0 + rand() % (1000 + 1);
        return response;
    }
    DOUBLE App_Seconds()                                            // Function for detect time
    {
        static DWORD InitialTime = timeGetTime();
        static DOUBLE TimeCounter = 0.0;
        DWORD NewTime = timeGetTime();
        TimeCounter += (NewTime - InitialTime) * (1. / 1000.);
        InitialTime = NewTime;
        return TimeCounter;
    }
    int Start()
    {
        int Input = NULL;

        system("CLS");
        while (1)
        {
            cout << "                               CHOOSE MODE                                       " << endl;
            cout << " _______________________________________________________________________________ " << endl;
            cout << "|                                                                               |" << endl;
            cout << "| Choose diagnostic mode:                                                       |" << endl;
            cout << "| 1 - Check each 512 bytes (sector)                                             |" << endl;
            cout << "| 2 - Check each 4096 bytes (cluster)                                           |" << endl;
            cout << "| (Than more bytes, tham faster diagnostic, but more chance to miss bad blocks) |" << endl;
            cout << "|_______________________________________________________________________________|" << endl;
            cout << " Choose: ";
            try
            {
                cin >> Input;
                if (cin.fail() || Input > 2 || Input < 1)
                {
                    system("CLS");
                    throw My_Exceptions("Watning: input isn't '1' or '2' !! Repeat:");
                }
                else break;
            }
            catch (My_Exceptions& tmp)
            {
                cout << endl << tmp.getproblem() << endl << endl;
                cin.clear();
                rewind(stdin);
            }
        }
        switch (Input)
        {
        case 1:
            Buffer_Size = 512;
            Buffer = new BYTE[Buffer_Size];
            break;
        case 2:
            Buffer_Size = 4096;
            Buffer = new BYTE[Buffer_Size];
            break;
        default:
            cout << "ERROR!" << endl;
            break;
        }
        system("CLS");

        cout << "Checking sectors for response . . ." << endl;

        int col = 0;
        int row = 2;
        int status = NULL;

        Put_Media(60, 0, leg, 11, LIGHTGREEN);
        Put_Media(60, 2, ms3, 5, LIGHTGRAY);                                                // <3 Ms
        Put_Media(60, 3, ms10, sizeof(ms10), LIGHTGRAY);                                    // <10 Ms
        Put_Media(60, 4, ms50, sizeof(ms50), LIGHTGRAY);                                    // <50 Ms
        Put_Media(60, 5, squ, 1, LIGHTGREEN);                                               // <150 Ms
        Put_Media(61, 5, ms150, sizeof(ms150), LIGHTGRAY);
        Put_Media(60, 6, squ, 1, YELLOW);                                                   // <500 Ms
        Put_Media(61, 6, ms500, sizeof(ms500), LIGHTGRAY);
        Put_Media(60, 7, squ, 1, LIGHTRED);                                                 // >500 Ms
        Put_Media(61, 7, ms500_2, sizeof(ms500_2), LIGHTGRAY);

        do
        {
            Current_Position = SetFilePointer(Pacient, 0, NULL, FILE_CURRENT);                 // check current position in file                                                            
            Seconds = App_Seconds();                                                        // start check time
            try
            {
                Read_Result = ReadFile(Pacient, Buffer, Buffer_Size, &Bytes_Readed, NULL);     // Read one sector;
                if (!Read_Result) throw My_Exceptions("ERROR! Problems while reading disk!");
            }
            catch (My_Exceptions& tmp)
            {
                cout << endl << tmp.getproblem() << endl;
                cout << "Error code: " << GetLastError() << endl;
                delete[] Buffer;
                Buffer = NULL;
                CloseHandle(Pacient);
                return 0;
            }
            SetFilePointer(Pacient, Distance_To_Move, NULL, FILE_BEGIN);                       // move pointer back at size of readed bytes
            Current_Position = SetFilePointer(Pacient, 0, NULL, FILE_CURRENT);                 // check current position in file
            Time_Out = App_Seconds() - Seconds;                                             // calculate out time    
            Distance_To_Move += Bytes_Readed;
            Size -= Bytes_Readed;                                                           // calculate bytes are need for check after
            //status = (int)Time_Out;                                                         // get spend time for switch status
            status= Get_Status();                                                         // simulating big time response 

            if (status <= 3)                                                                // switch status
                status = 3;
            else if (status <= 10)
                status = 10;
            else if (status <= 50)
                status = 50;
            else if (status <= 150)
                status = 150;
            else if (status <= 500)
                status = 500;
            else
                status = 999;
            switch (status)
            {
            case 3:
                Put_Media(col, row, B0, 1, LIGHTGRAY);
                break;
            case 10:
                Put_Media(col, row, B1, 1, LIGHTGRAY);
                break;
            case 50:
                Put_Media(col, row, B2, 1, LIGHTGRAY);
                break;
            case 150:                                                               // if more than 149 ms are out, go to try remap sector
                Put_Media(col, row, squ, 1, LIGHTGREEN);
                break;
            case 500:
                Put_Media(col, row, squ, 1, YELLOW);
                try
                {
                    Remap_Device = Remap(Pacient, Buffer,Buffer_Size,Current_Position);
                    Remap_Result = Remap_Device.start();     // Read one sector;
                    if (!Remap_Result) throw My_Exceptions("ERROR! Problems while remap sector!");
                }
                catch (My_Exceptions& tmp)
                {
                    cout << endl << tmp.getproblem() << endl;
                    cout << "Error code: " << GetLastError() << endl;
                    return 0;
                }
                break;
            case 999:
                Put_Media(col, row, squ, 1, LIGHTRED);
                try
                {
                    Remap_Device = Remap(Pacient, Buffer, Buffer_Size, Current_Position);
                    Remap_Result = Remap_Device.start();     // Read one sector;
                    if (!Remap_Result) throw My_Exceptions("ERROR! Problems while remap sector!");
                }
                catch (My_Exceptions& tmp)
                {
                    cout << endl << tmp.getproblem() << endl;
                    cout << "Error code: " << GetLastError() << endl;
                    return 0;
                }
                break;
            default:
                Put_Media(col, row, squ, 1, LIGHTGRAY);
            }
            ++col;
            if (MAX_COLUMNS == col)
            {
                col = 0;
                ++row;
            }
            if (MAX_ROWS == row)
            {
                for (int x = 2; x < MAX_ROWS; ++x)
                {
                    for (int i = 0; i < MAX_COLUMNS; ++i)
                        Put_Media(i, x, space, 1, LIGHTGRAY);
                }
                row = 2;
            }
            SetFilePointer(Pacient, Distance_To_Move, NULL, FILE_BEGIN);            // move pointer forward at size of readed bytes 
            Checked_Sectors++;                                                   // inc count of checked sectors
        } while (Read_Result || Distance_To_Move <= Size);

        CloseHandle(Pacient);
        Put_Media(0, 23, (unsigned char*)" SUCCESFULL!", 14, GREEN);
        cout << "About " << Checked_Sectors << " was checked for response" << endl;
        return 1;
    }
};

class My_Device
{
private:
    HANDLE Device ;                                                 // Your HDD or SSD;
    ULONGLONG Device_Size ;                                         // Size of the drive, in bytes;
    ULONG Bytes_Per_Sector_Count;                                   // Count of bytes per sector;
    LONGLONG Cylinders_Count;                                       // Count of disk cylinders;
    ULONG Tracks_Per_Cylinder_Count;                                // Count of tracks per cylinder;
    ULONG Sectors_Per_Track_Count;                                  // Count of sectors per track;
    LONGLONG Sectors_Count;                                         // Count of sectors;
public:
    Diagnostic Device_Diagnostic;                                                              // Getters
    HANDLE Get_Device() { return Device; }
    ULONGLONG Get_Device_Size() { return Device_Size; }
    ULONG Get_Bytes_Per_Sector_Count() { return Bytes_Per_Sector_Count; }
    LONGLONG Get_Cylinders_Count() {return Cylinders_Count ; }
    LONGLONG Get_Tracks_Per_Cylinder_Count() { return Tracks_Per_Cylinder_Count; }
    LONGLONG Get_Sectors_Per_Track_Count() { return Sectors_Per_Track_Count; }
    LONGLONG Get_Sectors_Count() { return Sectors_Count; }
                                                                    // Setters
    void Set_Device(HANDLE variable) { Device = variable; }
    void Set_Device_Size(ULONGLONG variable) { Device_Size = variable; }
    void Set_Bytes_Per_Sector_Count(ULONG variable) { Bytes_Per_Sector_Count = variable; }
    void Set_Cylinders_Count(LONGLONG variable) { Cylinders_Count = variable; }
    void Set_Tracks_Per_Cylinder_Count(LONGLONG variable) { Tracks_Per_Cylinder_Count = variable; }
    void Set_Sectors_Per_Track_Count(LONGLONG variable) { Sectors_Per_Track_Count = variable; }
    void Set_Sectors_Count(LONGLONG variable) { Sectors_Count = variable; }

    My_Device()                                                       // Constructor;
    {
        Device = INVALID_HANDLE_VALUE;
        Device_Size = NULL;   
        Bytes_Per_Sector_Count = NULL;                                   
        Cylinders_Count = NULL;                                          
        Tracks_Per_Cylinder_Count = NULL;                               
        Sectors_Per_Track_Count = NULL;                                  
        Sectors_Count = NULL;
        Device_Diagnostic = Diagnostic (Device,Device_Size);
    }
    ~My_Device()                                                         // Destructor;
    {
    }

    int Create_Me()                                                 // Function to create disk-file;
    {
        try                                    
        {
            Device = CreateFile                                       // Open disk;
            (
                L"\\\\.\\PhysicalDrive0",
                GENERIC_ALL,                                        // Read-write access;
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                0,
                NULL
            );
            if (Device == INVALID_HANDLE_VALUE) throw My_Exceptions("ERROR! Drive wasn't found!");
        }
        catch (My_Exceptions& tmp)
        {
            cout << endl << tmp.getproblem() << endl;
            cout << "Error code: " << GetLastError() << endl;
            return 0;
        }
        return 1;
    }
    int Get_My_Info()
    {
        PARTITION_INFORMATION Partition_Info = { NULL };            // Disk drive partition structure;
        DISK_GEOMETRY_EX Disk_Geometry = { NULL };                  // Disk drive geometry structure; 
        DWORD Bytes_Returned = NULL;
        BOOL RES = NULL;
        try
        {
            RES = DeviceIoControl                                   // Getting information of the physical disk's geometry;
            (
                Device,                                               // Handle disk;
                IOCTL_DISK_GET_DRIVE_GEOMETRY_EX,                   // Control code for getting geometry;
                NULL,                                               // Pointer to buffer to supply input data;
                0,                                                  // Size of input buffer;
                &Disk_Geometry,                                     // Pointer to buffer to receive output data;
                sizeof(DISK_GEOMETRY_EX),                           // Size of output buffer;
                &Bytes_Returned,                                    // Pointer to variable to receive output byte count;
                (LPOVERLAPPED)NULL                                  // Pointer to overlapped structure for asynchronous operation;
            );
                                             
            if (!RES) throw My_Exceptions("ERROR! Drive geometry info wasn't getted!");
        }
        catch (My_Exceptions& tmp)
        {
            cout << endl << tmp.getproblem() << endl;
            cout << "Error code: " << GetLastError() << endl;
            CloseHandle(Device);
            return 0;
        }   
        RES = NULL;
        try
        {
            RES = DeviceIoControl
            (
                Device,                                             // Handle disk;
                IOCTL_DISK_GET_PARTITION_INFO,                      // Control code for getting partition;
                NULL,                                               // Pointer to buffer to supply input data;
                NULL,                                               // Size of input buffer;
                &Partition_Info,                                    // Pointer to buffer to receive output data;
                sizeof(PARTITION_INFORMATION),                      // Size of output buffer;
                &Bytes_Returned,                                    // Pointer to variable to receive output byte count;   
                (LPOVERLAPPED)NULL                                  // Pointer to overlapped structure for asynchronous operation;
             );

            if (!RES) throw My_Exceptions("ERROR! Drive partition info wasn't getted!");
        }
        catch (My_Exceptions& tmp)
        {
            cout << endl << tmp.getproblem() << endl;
            cout << "Error code: " << GetLastError() << endl;
            CloseHandle(Device);
            return 0;
        }
        Set_Device_Size(Disk_Geometry.DiskSize.QuadPart);
        Set_Bytes_Per_Sector_Count(Disk_Geometry.Geometry.BytesPerSector);
        Set_Cylinders_Count(Disk_Geometry.Geometry.Cylinders.QuadPart);
        Set_Tracks_Per_Cylinder_Count(Disk_Geometry.Geometry.TracksPerCylinder);
        Set_Sectors_Per_Track_Count(Disk_Geometry.Geometry.SectorsPerTrack);
        Set_Sectors_Count ( Disk_Geometry.Geometry.Cylinders.QuadPart * Disk_Geometry.Geometry.TracksPerCylinder
            * Disk_Geometry.Geometry.SectorsPerTrack);

        Device_Diagnostic = Diagnostic(Device, Device_Size);

        cout << "                               DRIVE INFO                                 " << endl;
        cout << " ________________________________________________________________________ "<< endl;
        cout << "|                                                                         "<< endl;
        cout << "| Disk's size in bytes:      " << Device_Size                                << endl;
        cout << "| Cylinders count:           " << Cylinders_Count                          << endl;
        cout << "| Tracks per cylinder:       " << Tracks_Per_Cylinder_Count                << endl;
        cout << "| Sectors per track:         " << Sectors_Per_Track_Count                  << endl;
        cout << "| Bytes per sector:          " << Bytes_Per_Sector_Count                   << endl;
        cout << "| Sectors count:             " << Sectors_Count                            << endl ;
        cout << "|________________________________________________________________________ "<< endl<< endl;
        return 1;
    }

};



