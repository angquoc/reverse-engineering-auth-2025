#include <iostream>
#include <string>
#include <locale>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

using namespace std;

/*Hàm tính tổng các ký tự có dấu*/
int calculateS(const wchar_t* username, int length) {
    int S = 0;

    for (int i = 0; i < length; ++i) {
        wchar_t c = username[i];
        S += (c > 127) ? (c - 256) : c;
    }

    return S;
}

/*Hàm phát sinh key từ username*/
wstring generateKey(const wstring& username) {
    int L = static_cast<int>(username.length());

    if (L <= 4) {
        return L"Không phát sinh key vì độ dài chuỗi username <= 4";
    }

    int S = calculateS(username.c_str(), L);
    int L3 = L * L * L;
    int P = S ^ L3;
    int firstCharValue = (username[0] > 127) ? (username[0] - 256) : username[0];
    int lastCharValue = (username[L - 1] > 127) ? (username[L - 1] - 256) : username[L - 1];
    long long temp = static_cast<long long>(firstCharValue) * lastCharValue;
    long long Q = (temp * temp) ^ 0xB221;

    if (P == 0) {
        return L"Lỗi: P = 0";
    }

    long long R = Q / P;

    return to_wstring(R) + L"-" + to_wstring(Q);
}

int main() {
#ifdef _WIN32
    //Bật chế độ Unicode cho console trên Windows
    if (_setmode(_fileno(stdin), _O_U16TEXT) == -1) {
        return 1;
    }

    if (_setmode(_fileno(stdout), _O_U16TEXT) == -1) {
        return 1;
    }
#else
    //Linux/macOS: Bật locale hệ thống (UTF-8)
    setlocale(LC_ALL, "");
#endif

    wstring username;
    wcout << L"Nhập username: ";
    getline(wcin, username);

    wstring key = generateKey(username);
    wcout << L"Key: " << key << L"\n";

    return 0;
}