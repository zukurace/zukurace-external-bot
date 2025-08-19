

/*

ZUKURACE EXTERNAL BOT SDK
Author: ZukuRace dev



SUPPORT:
To support this project, you can buy the ZukuRace game for yourself or as a gift for your friends.
You can try ZukuRace External Bot in the game Demo version for free.




WHAT DOES THIS CODE DO:
- Receives position, rotation, ans speed magnitude of the car from the game (10 times/s).
- Sends input data (throttle, braking, steering) to the game back.
Data transmission is implemented with shared memory and semaphores IPC.

To enable external control press F5 in game once you launched it.
It will create all necessay handles for IPC.




DEDPENDENCIES: > GLM <
Download or clone GLM library https://github.com/g-truc/glm
Properties -> VC++ Directories -> \path\to\glm;$(IncludePath)




At this moment:
- It works only on Windows.
- The game does not send nearest race car positions (will be fixed first).
- The bot connection works only with RR01 and Night City track.
- RR01 car is unbalanced: slow speed understeer, high speed oversteer.
All this will be fixed later.




ADDITIONAL NOTES:
The bot code in the game is almost the same (using glm library instead of Unreal)
Feel free make the bot better and compete with other developers!

*/

#include <chrono>
#include <conio.h> // _kbhit, getch
#include <cstdio>
#include <thread>

#include <windows.h>

#include "RaceBotAI.h"
#include "RaceCommon.h"

#define LOGE(text, ...)                            \
    fwprintf(stderr, L##text "\n", ##__VA_ARGS__); \
    fflush(stderr);

static const wchar_t* SEMAPHORE_GAME_NAME = L"Local\\ZukuRace_Semaphore_UpdateGame";
static const wchar_t* SEMAPHORE_BOT_NAME = L"Local\\ZukuRace_Semaphore_UpdateBot";
static const wchar_t* SHARED_MEM_NAME = L"Local\\ZukuRace_Data";
static constexpr int SHARED_MEM_SIZE = sizeof(GameData);

#define SPAWN_TERMINAL_ON_SECONDARY_WINDOW 0

#if SPAWN_TERMINAL_ON_SECONDARY_WINDOW
BOOL CALLBACK MonitorEnumProc(HMONITOR hMon, HDC, LPRECT, LPARAM data)
{
    static int index = 0;
    int target = (int)data;
    if (index == target) {
        MONITORINFO mi { sizeof(mi) };
        GetMonitorInfo(hMon, &mi);

        HWND hwnd = GetConsoleWindow();
        if (hwnd) {
            RECT r = mi.rcWork;
            int width = 800, height = 600;
            int x = r.left + 50, y = r.top + 50;
            MoveWindow(hwnd, x, y, width, height, TRUE);
        }
        return FALSE;
    }
    ++index;
    return TRUE;
}
#endif

int main()
{
    //
    // Console window things
    //

    HWND hwnd = GetConsoleWindow();
    if (hwnd) { // HWND_TOPMOST = make window always on top
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    }

    SetConsoleTitle(L"External ZukuRace bot");

#if SPAWN_TERMINAL_ON_SECONDARY_WINDOW
    EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, (LPARAM)1); // spawn on monitor 1
#endif

    //
    // IPC things
    //

    HANDLE handleMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0,
        SHARED_MEM_SIZE, SHARED_MEM_NAME);

    if (!handleMapFile) {
        LOGE("CreateFileMapping failed: %d", GetLastError());
        return 1;
    }

    const bool alreadyExists = GetLastError() == ERROR_ALREADY_EXISTS;

    GameData* gameData = (GameData*)MapViewOfFile(handleMapFile, FILE_MAP_ALL_ACCESS, 0, 0, SHARED_MEM_SIZE);
    if (!gameData) {
        LOGE("MapViewOfFile failed: %d", GetLastError());
        CloseHandle(handleMapFile);
        return 1;
    }

    if (!alreadyExists) {
        memset(gameData, 0, SHARED_MEM_SIZE);
    }

    HANDLE semaphoreGame = CreateSemaphore(nullptr, 0, 1, SEMAPHORE_GAME_NAME);
    if (!semaphoreGame) {
        LOGE("CreateSemaphore RaceDataUpdated failed: %d", GetLastError());
        UnmapViewOfFile(gameData);
        CloseHandle(handleMapFile);
        return 1;
    }

    HANDLE semaphoreBot = CreateSemaphore(nullptr, 0, 1, SEMAPHORE_BOT_NAME);
    if (!semaphoreBot) {
        LOGE("CreateSemaphore CarInputUpdated failed: %d", GetLastError());
        UnmapViewOfFile(gameData);
        CloseHandle(handleMapFile);
        return 1;
    }

    printf("Waiting for game data, press 'q' to exit:\n");
    printf("Closing this window without 'q' car continues moving with last input data.\n");

    //
    // Wait for exit loop things
    //

    std::atomic<bool> mustLoop(true), isCarAttached(false);

    std::thread t1([&]() {
        while (mustLoop) {
            if (_kbhit() && _getch() == 'q')
                break;

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        ReleaseSemaphore(semaphoreGame, 1, nullptr);
        mustLoop = false;
    });

    //
    // Main things
    //

    RaceBotAI bot;

    gameData->msgBot = MsgBot::Start;
    ReleaseSemaphore(semaphoreBot, 1, nullptr);

    // To enable external control press F5 in game once you launched it.
    // It will create all necessay handles for IPC.

    while (mustLoop) {
        DWORD waitResult = WaitForSingleObject(semaphoreGame, INFINITE);
        if (mustLoop && waitResult == WAIT_OBJECT_0) {
            if (gameData->msgCar == MsgCar::Update) {
                auto& ci = gameData->carInput;

                ci = bot.Calculate(gameData->carData);
                printf("throttle %.2f, braking %.2f, steering %.2f\n",
                    ci.throttle, ci.braking, ci.steering);

                // Make some AAAAAAA!!1
                // ci.braking = float(rand()) / RAND_MAX;
                // ci.throttle = float(rand()) / RAND_MAX;
                // ci.steering = (float(rand()) / RAND_MAX) * 2 - 1;

                gameData->msgBot = MsgBot::Update; // set car input
                ReleaseSemaphore(semaphoreBot, 1, nullptr);

            } else if (gameData->msgCar == MsgCar::Possessed) {
                gameData->msgBot = MsgBot::Start; // switch_on car to bot control
                ReleaseSemaphore(semaphoreBot, 1, nullptr);
                bot.reset();
                LOGE("Car started");

            } else if (gameData->msgCar == MsgCar::Unpossessed) {
                gameData->msgBot = MsgBot::Stop; // switch_off car to bot control
                ReleaseSemaphore(semaphoreBot, 1, nullptr);

                LOGE("Car stopped");
            }
        }
    }

    gameData->msgBot = MsgBot::Stop;
    ReleaseSemaphore(semaphoreBot, 1, nullptr);

    //
    // Close all things
    //

    t1.join();
    CloseHandle(semaphoreGame);
    CloseHandle(semaphoreBot);
    UnmapViewOfFile(gameData);
    CloseHandle(handleMapFile);

    return 0;
}
