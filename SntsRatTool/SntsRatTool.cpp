#include <iostream>
#include <Windows.h>
#include <vector>
#include <queue>
#include <chrono>

TCHAR MEMORY_MAPPED_FILE_NAME[] = TEXT("SNTS_TOOL_SHARED_FILE");

typedef struct {
    bool isHexingPantsEquipped;
    bool isInRift;
    bool hasMaxEssence;
    bool hasTarget;
    int monsterPriority;
    int targetX;
    int targetY;
    int skeletonMageTimeLeft;
    int secondsLeftToRecastMage;
    int secondsMageDuration;
    int numberOfSkeletonMages;
    int numberOfMonsters;
} SharedFileContents;

std::queue<std::chrono::system_clock::time_point> _skeletonMageSpawnTimes;

BOOL CALLBACK enumWindowsCallback(HWND hwnd, LPARAM lParam) {
    const DWORD TITLE_SIZE = 1024;
    WCHAR windowTitle[TITLE_SIZE];
    GetWindowTextW(hwnd, windowTitle, TITLE_SIZE);
    std::wstring title(&windowTitle[0]);
    //std::wcout << title << std::endl;
    if (title.size() > 0 && title == L"Diablo III") {
        std::vector<HWND>& windowHandles =
            *reinterpret_cast<std::vector<HWND>*>(lParam);
        windowHandles.push_back(hwnd);
    }
    return TRUE;
}

std::vector<HWND> findD3Windows() {
    std::vector<HWND> windowHandles;
    EnumWindows(enumWindowsCallback, reinterpret_cast<LPARAM>(&windowHandles));
    return windowHandles;
}

int main()
{
    HANDLE hMapFile;

    hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 256, MEMORY_MAPPED_FILE_NAME);

    if (hMapFile == NULL) {
        std::cout << "CreateFileMapping failed" << std::endl;
        throw GetLastError();
        return -1;
    }
    HANDLE lpMapAddress = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (lpMapAddress == NULL) {
        std::cout << "MapViewOfFile failed" << std::endl;
        throw GetLastError();
        return -1;
    }

    std::vector<HWND> winHandles = findD3Windows();
    HWND handle = winHandles.at(0);
    RECT rect;
    GetClientRect(handle, &rect);

    int i = 0;
    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;
    std::cout << windowWidth << std::endl;
    std::cout << windowHeight << std::endl;

    int midX = 0.50 * windowWidth;
    int midY = 0.47 * windowHeight;
    int d = 10;

    // wiggle
    while (true) {
        SharedFileContents* contents = (SharedFileContents*)lpMapAddress;
        if (contents->isInRift) {

            // wiggle
            if (contents->isHexingPantsEquipped && !(GetKeyState(' ') & 0x8000)) {
                SendMessage(handle, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(midX + d, midY));
                SendMessage(handle, WM_LBUTTONUP, 0, MAKELPARAM(midX + d, midY));
                Sleep(100);
                SendMessage(handle, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(midX - d, midY));
                SendMessage(handle, WM_LBUTTONUP, 0, MAKELPARAM(midX - d, midY));
            }

            // auto aim cast mages
            if (contents->hasMaxEssence) {
                // dont recast on trash if mages up but spam on elite
                bool hasEliteTarget = contents->monsterPriority >= 2 && contents->hasTarget;
                bool hasAllMagesUp = contents->numberOfSkeletonMages >= 10;
                bool needsRefresh = false;
                if (!_skeletonMageSpawnTimes.empty()) {
                    std::chrono::system_clock::time_point timestampOfMageFirstCasted = _skeletonMageSpawnTimes.front();
                    auto secondsSinceFirstActiveMageCasted = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - timestampOfMageFirstCasted).count();

                    needsRefresh = (contents->secondsMageDuration - secondsSinceFirstActiveMageCasted) < contents->secondsLeftToRecastMage;
                }
                if (hasEliteTarget || !hasAllMagesUp || needsRefresh) {
                    _skeletonMageSpawnTimes.push(std::chrono::system_clock::now());
                    if (_skeletonMageSpawnTimes.size() > 10) {
                        _skeletonMageSpawnTimes.pop();
                    }

                    SendMessage(handle, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(contents->targetX, contents->targetY));
                    SendMessage(handle, WM_RBUTTONUP, 0, MAKELPARAM(contents->targetX, contents->targetY));
                }
            }

            Sleep(100);
        }
        else {
            Sleep(500);
        }
    }


    UnmapViewOfFile(hMapFile);

    CloseHandle(hMapFile);
}