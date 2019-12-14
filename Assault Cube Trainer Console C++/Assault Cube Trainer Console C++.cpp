#include <iostream>
#include <vector>
#include <Windows.h>
#include <ctime>
#include <string>


DWORD FindDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress);
void WriteToMemory(HANDLE hProcHandle);


std::string GameName = "AssaultCube";
LPCSTR LGameWindow = "AssaultCube";
std::string GameStatus;

bool IsGameAvailable;
bool UpdateOnNextRun;

// AMMO VARS

bool AmmoStatus;
BYTE AmmoValue[] = { 0xA3, 0x1C, 0x0, 0x0 };
DWORD AmmoBaseAddress = { 0x0050F4F4 };
DWORD AmmoOffsets[] = { 0x374, 0x14, 0x0 };

// HEALTH VARS

bool HealthStatus;
BYTE HealthValue[] = { 0x39, 0x5, 0x0, 0x0 };
DWORD HealthBaseAddress = { 0x0050F4F4 };
DWORD HealthOffsets[] = { 0xF8 };

// ARMOR VARS

bool ArmorStatus;
BYTE ArmorValue[] = { 0x39, 0x5, 0x0, 0x0 };
DWORD ArmorBaseAddress = { 0x0050F4F4 };
DWORD ArmorOffsets[] = { 0xFC };

// NO RECOIL VARS 

std::string Nops[] =
{
"\x90",
"\x90\x90",
"\x90\x90\x90",
"\x90\x90\x90\x90"
};

bool NoRecoilStatus;
char NoRecoilDefaultOpCode[] = "\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2";
char NoRecoilNOPs[] = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90";
DWORD NoRecoilBaseAddress = { 0x463786 };





int main()
{
    HWND hGameWindow = NULL;
    int TimeSinceLastUpdate = clock();
    int GameAvailTMR = clock();
    int onePressTMR = clock();
    DWORD dwProcID = NULL;
    HANDLE hProcHandle = NULL;
    UpdateOnNextRun = true;
    std::string sAmmoStatus = "OFF";
    std::string sHealthStatus = "OFF";
    std::string sArmorStatus = "OFF";
    std::string sNoRecoilStatus = "OFF";

    while (!GetAsyncKeyState(VK_INSERT))
    {
        if (clock() - GameAvailTMR > 100)
        {
            GameAvailTMR = clock();
            IsGameAvailable = false;

            hGameWindow = FindWindow(NULL, LGameWindow);
            if (hGameWindow)
            {
                GetWindowThreadProcessId(hGameWindow, &dwProcID);
                if (dwProcID != 0)
                {
                    hProcHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcID);
                    if (hProcHandle == INVALID_HANDLE_VALUE || hProcHandle == NULL)
                    {
                        GameStatus = "Failed to open process for valid handle ";
                    }
                    else
                    {
                        GameStatus = "AssaultCube Ready To Hack";
                        IsGameAvailable = true;
                    }
                }
                else
                {
                    GameStatus = " Failed to get processID ";
                }
            }
            else
            {
                GameStatus = " AssaultCube NOT FOUND ";
            }

            if (UpdateOnNextRun || clock() - TimeSinceLastUpdate > 5000)
            {
                system("CLS");
                std::cout << " -------------------------------------------------------" << std::endl;
                std::cout << "      AssaultCube Memore Hacker " << std::endl;
                std::cout << " -------------------------------------------------------" << std::endl << std::endl;
                std::cout << " GAME STATUS " << GameStatus << std::endl << std::endl;
                std::cout << " [F1] Unlimited Ammo -> " << sAmmoStatus << " <- " << std::endl << std::endl;
                std::cout << " [F2] Unlimited Health -> " << sHealthStatus << " <- " << std::endl << std::endl;
                std::cout << " [F3] Unlimited Armor -> " << sArmorStatus << " <- " << std::endl << std::endl;
                std::cout << " [F4] No Recoil -> " << sNoRecoilStatus << " <- " << std::endl << std::endl;
                std::cout << " [INSERT] Exit " << std::endl;
                UpdateOnNextRun = false;
                TimeSinceLastUpdate = clock();
            }

            if (IsGameAvailable)
            {
                WriteToMemory(hProcHandle);
            }


        }

        if (clock() - onePressTMR > 400)
        {
            if (IsGameAvailable)
            {
                if (GetAsyncKeyState(VK_F1))
                {
                    onePressTMR = clock();
                    AmmoStatus = !AmmoStatus;
                    UpdateOnNextRun = true;
                    if (AmmoStatus) sAmmoStatus = "ON";
                    else sAmmoStatus = "OFF";

                }

                else if (GetAsyncKeyState(VK_F2))
                {
                    onePressTMR = clock();
                    HealthStatus = !HealthStatus;
                    UpdateOnNextRun = true;
                    if (HealthStatus) sHealthStatus = "ON";
                    else sHealthStatus = "OFF";

                }

                else if (GetAsyncKeyState(VK_F3))
                {
                    onePressTMR = clock();
                    ArmorStatus = !ArmorStatus;
                    UpdateOnNextRun = true;
                    if (ArmorStatus) sArmorStatus = "ON";
                    else sArmorStatus = "OFF";

                }

                else if (GetAsyncKeyState(VK_F4))
                {
                    onePressTMR = clock();
                    NoRecoilStatus = !NoRecoilStatus;
                    UpdateOnNextRun = true;
                    if (NoRecoilStatus) sNoRecoilStatus = "ON";
                    else sNoRecoilStatus = "OFF";

                }
            }
        }
    }

    CloseHandle(hProcHandle);
    CloseHandle(hGameWindow);

    return ERROR_SUCCESS;


}

DWORD FindDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress)
{

    DWORD pointer = BaseAddress;
    DWORD pTemp;

    DWORD pointerAddr;
    for (int c = 0; c < PointerLevel; c++)
    {
        if (c == 0)
        {
            ReadProcessMemory(hProcHandle, (LPCVOID)pointer, &pTemp, sizeof(pTemp), NULL);
        }

        pointerAddr = pTemp + Offsets[c];
        ReadProcessMemory(hProcHandle, (LPCVOID)pointerAddr, &pTemp, sizeof(pTemp), NULL);
    }
    return pointerAddr;



}



void WriteToMemory(HANDLE hProcHandle)
{
    DWORD AddresToWrite;
    if (AmmoStatus)
    {
        AddresToWrite = FindDmaAddy(3, hProcHandle, AmmoOffsets, AmmoBaseAddress);
        WriteProcessMemory(hProcHandle, (BYTE*)AddresToWrite, &AmmoValue, sizeof(AmmoValue), NULL);
    }

    if (HealthStatus)
    {
        AddresToWrite = FindDmaAddy(1, hProcHandle, HealthOffsets, HealthBaseAddress);
        WriteProcessMemory(hProcHandle, (BYTE*)AddresToWrite, &HealthValue, sizeof(HealthValue), NULL);
    }

    if (ArmorStatus)
    {
        AddresToWrite = FindDmaAddy(1, hProcHandle, ArmorOffsets, ArmorBaseAddress);
        WriteProcessMemory(hProcHandle, (BYTE*)AddresToWrite, &ArmorValue, sizeof(ArmorValue), NULL);
    }

    if (ArmorStatus)
    {
        AddresToWrite = FindDmaAddy(1, hProcHandle, ArmorOffsets, ArmorBaseAddress);
        WriteProcessMemory(hProcHandle, (BYTE*)AddresToWrite, &ArmorValue, sizeof(ArmorValue), NULL);
    }

    if (NoRecoilStatus)
    {
        WriteProcessMemory(hProcHandle, (BYTE*)NoRecoilBaseAddress, NoRecoilNOPs, 10, NULL);
    }
    else
    {
        WriteProcessMemory(hProcHandle, (BYTE*)NoRecoilBaseAddress, NoRecoilDefaultOpCode, 10, NULL);
    }

}

