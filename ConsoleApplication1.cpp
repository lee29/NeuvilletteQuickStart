#include <iostream>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <Shellapi.h>

bool IsRunAsAdministrator() {
    BOOL fIsRunAsAdmin = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    PSID pAdministratorsGroup = NULL;

    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0, &pAdministratorsGroup))
    {
        if (!CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin))
        {
            dwError = GetLastError();
        }

        FreeSid(pAdministratorsGroup);
    }

    return fIsRunAsAdmin != 0;
}

void RestartAsAdministrator() {
    SHELLEXECUTEINFO sei = { sizeof(sei) };
    sei.lpVerb = L"runas";
    sei.lpFile = GetCommandLine();
    sei.nShow = SW_NORMAL;

    if (!ShellExecuteEx(&sei)) {
        std::cerr << "Failed to restart as administrator." << std::endl;
    }
}

int main() {
    if (!IsRunAsAdministrator()) {
        std::cerr << "Detected that the program is not run as administrator. Please run the program as an administrator." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    std::cout << "Press the '~' key to start simulating mouse clicks. Written by Morax" << std::endl;

    while (true) {
        if (GetKeyState(VK_OEM_3) < 0) {
            POINT p;
            GetCursorPos(&p);
            mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}
