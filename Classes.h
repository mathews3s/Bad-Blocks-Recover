#include"Libers.h"

class Exceptions
{
private:
    string Error_Name;
public:
    Exceptions(string Error) { Error_Name = Error; }
    ~Exceptions() {}
    string Get_Problem() { return Error_Name; }
};

class Sector_Remapper
{
private:
    HANDLE Pacient;
    DWORD Current_Position;                              
    DWORD Buffer_Size;                                   
    BYTE* Buffer;                                        
    BOOL Read_Result;                                      
    DWORD Bytes_Readed;                                  
    LONG Repeat_Count;                                 
    DWORD Position_Now;                                   
public:
    Sector_Remapper()
    {
        Pacient = INVALID_HANDLE_VALUE;
        Current_Position = NULL;                               
        Buffer_Size = NULL;                                    
        Buffer = NULL;                                         
        Read_Result = NULL;                                      
        Bytes_Readed = NULL;                                   
        Position_Now = NULL;
        Repeat_Count = 10000;
    }
    Sector_Remapper(HANDLE Device, BYTE* Buffer_Param, DWORD Buffer_Size_Param, DWORD Current_Position_Param )
    {
        Pacient = Device;
        Current_Position = Current_Position_Param;                      
        Buffer_Size = Buffer_Size_Param;                               
        Buffer = Buffer_Param;                                        
        Read_Result = NULL;                                    
        Bytes_Readed = NULL;                                   
        Position_Now = NULL;
        Repeat_Count = 10000;
    }
    ~Sector_Remapper()
    {}
    int Start_Remaping()
    {
                                   // Current position in file;
        while (Repeat_Count != NULL)
        {
            Position_Now = SetFilePointer(Pacient, 0, NULL, FILE_CURRENT);                // Check current position;
            try
            {
                Read_Result = ReadFile(Pacient, Buffer, Buffer_Size, &Bytes_Readed, NULL);     // Read one sector;
                if (!Read_Result) throw Exceptions("ERROR! Problems while reading disk!");
            }
            catch (Exceptions& tmp)
            {
                cout << endl << tmp.Get_Problem() << endl;
                cout << "Error code: " << GetLastError() << endl;
                delete[] Buffer;
                Buffer = NULL;
                CloseHandle(Pacient);
                return 0;
            }
            SetFilePointer(Pacient, Current_Position, NULL, FILE_BEGIN);              // Set file position at back to read current sector again;
            Repeat_Count--;
        }
        return 1;
    }
};

class Diagnostic_Controller
{
private:
    HANDLE Pacient;
    DWORD Current_Position;                              
    LONG Distance_To_Move;                               
    DWORD Buffer_Size;                                   
    BYTE* Buffer;                                        
    DOUBLE Seconds;                                      
    DOUBLE Time_Out;                                     
    BOOL Read_Result;                                    
    DWORD Bytes_Readed;                                  
    LONGLONG Checked_Sectors;                            
    BOOL Remap_Result;                                   
    ULONGLONG Size;                                      
    Sector_Remapper Remap_Device;
    LONGLONG Bads_Count;
public:
    Diagnostic_Controller()
    {
        Pacient = INVALID_HANDLE_VALUE;
        Current_Position = NULL;                               
        Distance_To_Move = NULL;                               
        Buffer_Size = NULL;                                    
        Buffer = NULL;                                         
        Seconds = NULL;                                        
        Time_Out = NULL;                                       
        Read_Result = NULL;                                    
        Bytes_Readed = NULL;                                   
        Checked_Sectors = NULL;                                
        Remap_Result = NULL;                                   
        Size = NULL;                                           
        Bads_Count = NULL;
        Remap_Device = Sector_Remapper();
    }
    Diagnostic_Controller(HANDLE Device, ULONGLONG Device_Size)
    {
        Pacient = Device;
        Current_Position = NULL;                               
        Distance_To_Move = NULL;                               
        Buffer_Size = NULL;                                    
        Buffer = NULL;                                         
        Seconds = NULL;                                        
        Time_Out = NULL;                                       
        Read_Result = NULL;                                    
        Bytes_Readed = NULL;                                   
        Checked_Sectors = NULL;                                
        Remap_Result = NULL;                                   
        Size = Device_Size;                                    
        Bads_Count = NULL;
        Remap_Device = Sector_Remapper();
    }
    ~Diagnostic_Controller()
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
    DOUBLE App_Seconds()                                            
    {
        static DWORD InitialTime = timeGetTime();
        static DOUBLE TimeCounter = 0.0;
        DWORD NewTime = timeGetTime();
        TimeCounter += (NewTime - InitialTime) * (1. / 1000.);
        InitialTime = NewTime;
        return TimeCounter;
    }
    void Out_Legend()
    {
        cout << "Checking sectors for response . . ." << endl;
        cout << "(Press 'S' to view current results or 'E' to exit)" << endl;
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
    }
    void Out_Results(LONGLONG Sectors, int Precents)
    {
        system("CLS");
        Precents = Precent_Calculate(Sectors, Checked_Sectors);
        cout << "STOPED:" << endl;
        cout << "                               RESULTS:                                          " << endl;
        cout << " _______________________________________________________________________________ " << endl;
        cout << "|                                                                                " << endl;
        cout << "| Count of sectors   - " << Sectors << endl;
        cout << "| Sectors checked    - " << Checked_Sectors << endl;
        cout << "| Bad sectors found  - " << Bads_Count << endl;
        cout << "| Process            - " << Precents << "%" << endl;
        cout << "|                                                                                " << endl;
        cout << "|_______________________________________________________________________________ " << endl;
        system("pause");
        system("CLS");
    }
    int Precent_Calculate(LONGLONG Sectors, LONGLONG Checked_Sectors)
    {
        LONGLONG Result = Checked_Sectors * 100;
        int Precent = Result / Sectors;
        return Precent;
    }
    int Start_Diagnostic(LONGLONG Sectors)
    {
        int Input = NULL;
        int Columns = 0;
        int Rows = 2;
        int Status = NULL;
        int Precents = NULL;
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
                    throw Exceptions("Watning: input isn't '1' or '2' !! Repeat:");
                }
                else break;
            }
            catch (Exceptions& tmp)
            {
                cout << endl << tmp.Get_Problem() << endl << endl;
                system("PAUSE");
                system("CLS");
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
        Out_Legend();
        do
        {
            if (GetKeyState('S') & 0x8000)
            {
                Out_Results(Sectors, Precents);
                Out_Legend();
            }
            if (GetKeyState('E') & 0x8000)
            {
                system("CLS");
                cout << "[Forced exit]"<< endl;
                return 0;
            }
            Current_Position = SetFilePointer(Pacient, 0, NULL, FILE_CURRENT);                                                                           
            Seconds = App_Seconds();                                                        
            try
            {
                Read_Result = ReadFile(Pacient, Buffer, Buffer_Size, &Bytes_Readed, NULL);     // Read one sector;
                if (!Read_Result) throw Exceptions("ERROR! Problems while reading disk!");
            }
            catch (Exceptions& tmp)
            {
                cout << endl << tmp.Get_Problem() << endl;
                cout << "Error code: " << GetLastError() << endl;
                delete[] Buffer;
                Buffer = NULL;
                CloseHandle(Pacient);
                return 0;
            }
            Time_Out = App_Seconds() - Seconds;
            Status = (int)Time_Out;
            SetFilePointer(Pacient, Distance_To_Move, NULL, FILE_BEGIN);                       
            Current_Position = SetFilePointer(Pacient, 0, NULL, FILE_CURRENT);                                                            
            Distance_To_Move += Bytes_Readed;
                                                                                                                 
            //Status= Get_Status();                                                         // simulating big time response 

            if (Status <= 3)                                                                
                Status = 3;
            else if (Status <= 10)
                Status = 10;
            else if (Status <= 50)
                Status = 50;
            else if (Status <= 150)
                Status = 150;
            else if (Status <= 500)
                Status = 500;
            else
                Status = 999;
            switch (Status)
            {
            case 3:
                Put_Media(Columns, Rows, B0, 1, LIGHTGRAY);
                break;
            case 10:
                Put_Media(Columns, Rows, B1, 1, LIGHTGRAY);
                break;
            case 50:
                Put_Media(Columns, Rows, B2, 1, LIGHTGRAY);
                break;
            case 150:                                                               
                Put_Media(Columns, Rows, squ, 1, LIGHTGREEN);
                break;
            case 500:
                Put_Media(Columns, Rows, squ, 1, YELLOW);
                try
                {
                    Bads_Count++;
                    Remap_Device = Sector_Remapper(Pacient, Buffer,Buffer_Size,Current_Position);
                    Remap_Result = Remap_Device.Start_Remaping();    
                    if (!Remap_Result) throw Exceptions("ERROR! Problems while remap sector!");
                }
                catch (Exceptions& tmp)
                {
                    cout << endl << tmp.Get_Problem() << endl;
                    cout << "Error code: " << GetLastError() << endl;
                    return 0;
                }
                break;
            case 999:
                Put_Media(Columns, Rows, squ, 1, LIGHTRED);
                try
                {
                    Bads_Count++;
                    Remap_Device = Sector_Remapper(Pacient, Buffer, Buffer_Size, Current_Position);
                    Remap_Result = Remap_Device.Start_Remaping();     
                    if (!Remap_Result) throw Exceptions("ERROR! Problems while remap sector!");
                }
                catch (Exceptions& tmp)
                {
                    cout << endl << tmp.Get_Problem() << endl;
                    cout << "Error code: " << GetLastError() << endl;
                    return 0;
                }
                break;
            default:
                Put_Media(Columns, Rows, squ, 1, LIGHTGRAY);
            }
            ++Columns;
            if (MAX_COLUMNS == Columns)
            {
                Columns = 0;
                ++Rows;
            }
            if (MAX_ROWS == Rows)
            {
                for (int x = 2; x < MAX_ROWS; ++x)
                {
                    for (int i = 0; i < MAX_COLUMNS; ++i)
                        Put_Media(i, x, space, 1, LIGHTGRAY);
                }
                Rows = 2;
            }
            SetFilePointer(Pacient, Distance_To_Move, NULL, FILE_BEGIN);            
            Checked_Sectors++;                                                      
        } while (Read_Result || Distance_To_Move <= Size);
        CloseHandle(Pacient);
        Put_Media(0, 23, (unsigned char*)" SUCCESFULL!", 14, GREEN);
        Out_Results(Sectors, Precents);
        return 1;
    }
};

class Device_Master
{
private:
    HANDLE Device;                                                 
    ULONGLONG Device_Size;                                          
    ULONG Bytes_Per_Sector_Count;                                   
    LONGLONG Cylinders_Count;                                       
    LONGLONG Tracks_Per_Cylinder_Count;                             
    LONGLONG Sectors_Per_Track_Count;                               
    LONGLONG Sectors_Count;                                         
    Diagnostic_Controller Device_Diagnostic;                        
public:                                                             
    HANDLE Get_Device() { return Device; }                          
    ULONGLONG Get_Device_Size() { return Device_Size; }
    ULONG Get_Bytes_Per_Sector_Count() { return Bytes_Per_Sector_Count; }
    LONGLONG Get_Cylinders_Count() {return Cylinders_Count ; }
    LONGLONG Get_Tracks_Per_Cylinder_Count() { return Tracks_Per_Cylinder_Count; }
    LONGLONG Get_Sectors_Per_Track_Count() { return Sectors_Per_Track_Count; }
    LONGLONG Get_Sectors_Count() { return Sectors_Count; }
                                                                    
    void Set_Device(HANDLE variable) { Device = variable; }
    void Set_Device_Size(ULONGLONG variable) { Device_Size = variable; }
    void Set_Bytes_Per_Sector_Count(ULONG variable) { Bytes_Per_Sector_Count = variable; }
    void Set_Cylinders_Count(LONGLONG variable) { Cylinders_Count = variable; }
    void Set_Tracks_Per_Cylinder_Count(LONGLONG variable) { Tracks_Per_Cylinder_Count = variable; }
    void Set_Sectors_Per_Track_Count(LONGLONG variable) { Sectors_Per_Track_Count = variable; }
    void Set_Sectors_Count(LONGLONG variable) { Sectors_Count = variable; }

    Device_Master()                                                       // Constructor;
    {
        Device = INVALID_HANDLE_VALUE;
        Device_Size = NULL;   
        Bytes_Per_Sector_Count = NULL;                                   
        Cylinders_Count = NULL;                                          
        Tracks_Per_Cylinder_Count = NULL;                               
        Sectors_Per_Track_Count = NULL;                                  
        Sectors_Count = NULL;
        Device_Diagnostic = Diagnostic_Controller();
    }
    ~Device_Master()                                                         // Destructor;
    {
    }
    ULONGLONG To_GB(ULONGLONG Size)
    {
        ULONGLONG GB = Size / Bytes_In_GB;
        return GB;
    }
    int Create_Device(LPCTSTR drive)                                                 
    {
        try                                    
        {
            Device = CreateFile                                       
            (
                drive,
                GENERIC_ALL,                                        
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                0,
                NULL
            );
            if (Device == INVALID_HANDLE_VALUE) throw Exceptions("ERROR! Drive wasn't found!");
        }
        catch (Exceptions& tmp)
        {
            cout << endl << tmp.Get_Problem() << endl;
            cout << "Error code: " << GetLastError() << endl;
            return 0;
        }
        return 1;
    }
    void Out_Device_Info()
    {
        ULONGLONG GB = NULL;
        GB = To_GB(Device_Size);
        cout << "                               DRIVE INFO                                 " << endl;
        cout << " ________________________________________________________________________ " << endl;
        cout << "|                                                                         " << endl;
        cout << "| Disk's size in bytes:      " << Device_Size << " (" << GB << " GB)" << endl;
        cout << "| Cylinders count:           " << Cylinders_Count << endl;
        cout << "| Tracks per cylinder:       " << Tracks_Per_Cylinder_Count << endl;
        cout << "| Sectors per track:         " << Sectors_Per_Track_Count << endl;
        cout << "| Bytes per sector:          " << Bytes_Per_Sector_Count << endl;
        cout << "| Sectors count:             " << Sectors_Count << endl;
        cout << "|________________________________________________________________________ " << endl << endl;
    }
    int Get_Device_Info()
    {
        PARTITION_INFORMATION Partition_Info = { NULL };            
        DISK_GEOMETRY_EX Disk_Geometry = { NULL };                  
        DWORD Bytes_Returned = NULL;
        BOOL RES = NULL;
        try
        {
            RES = DeviceIoControl                                   
            (
                Device,                                               
                IOCTL_DISK_GET_DRIVE_GEOMETRY_EX,                   
                NULL,                                               
                0,                                                  
                &Disk_Geometry,                                     
                sizeof(DISK_GEOMETRY_EX),                           
                &Bytes_Returned,                                    
                (LPOVERLAPPED)NULL                                  
            );
                                             
            if (!RES) throw Exceptions("ERROR! Drive geometry info wasn't getted!");
        }
        catch (Exceptions& tmp)
        {
            cout << endl << tmp.Get_Problem() << endl;
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

            if (!RES) throw Exceptions("ERROR! Drive partition info wasn't getted!");
        }
        catch (Exceptions& tmp)
        {
            cout << endl << tmp.Get_Problem() << endl;
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

        Out_Device_Info();

        Device_Diagnostic = Diagnostic_Controller(Device, Device_Size);          // Creating diagnostic class with 

        return 1;
    }
    void Start_Diagnostic_Device()
    {
        Device_Diagnostic.Start_Diagnostic(Sectors_Count);
    }
};

