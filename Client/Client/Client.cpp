//Client.cpp
#include "Client.h"
#include "GameFramework.h"
#include "resource.h"
#include "Common.h"

#include "Packet.h"
#include "ErrDisplay.h"

#define CLIENT_WIDTH    800
#define CLIENT_HEIGHT   600

#define GAME_TIMER      1    
#define GAME_FRAME_RATE 16

SOCKET sock;
char buf[BUFSIZE + 1];
size_t retval = 0;

int screenWidth = GetSystemMetrics(SM_CXSCREEN);  // 화면 너비
int screenHeight = GetSystemMetrics(SM_CYSCREEN); // 화면 높이

int wIndowxPos = (screenWidth - CLIENT_WIDTH) / 2;
int windowyPos = (screenHeight - CLIENT_HEIGHT) / 2;

HINSTANCE hInst;
LPCTSTR lpszClass = L"Winapi Term Project";
LPCTSTR lpszWindowName = L"Winapi Term Project";

static GameFramework gameframework;

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// 대화상자로 닉네임을 받아오고 전송
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
HWND hSendButton;   // 확인버튼
HWND hEdit1; // 애디트 컨트롤
HANDLE hDialogEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

Client client;



int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;
    DWORD frameStartTime{};
    DWORD frameEndTime{};
    const DWORD frameDuration = 1000 / 60;
    float frameTime{};

    // client name을 받아올 대화상자
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
    // winsock
    {
        // 소켓 생성
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET) err_quit("socket");
        if (sock == INVALID_SOCKET) {
            std::cerr << "Failed to create socket. Error: " << WSAGetLastError() << std::endl;
            MessageBox(NULL, L"Failed to create server socket", L"Socket Error", MB_ICONERROR | MB_OK);

        }
            // connect()
        struct sockaddr_in serveraddr;
        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
        serveraddr.sin_port = htons(SERVERPORT);
        retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
        if (retval == SOCKET_ERROR) err_quit("connect");


        // 닉네임 전송
        retval = send(sock, client.name, sizeof(client.name), 0);
        if (retval == SOCKET_ERROR) err_display("send - clientName");

        // ID 할당 받기
        retval = recv(sock, (char*)&client.ID, sizeof(client.ID), 0);
        if (retval == SOCKET_ERROR) err_display("receive - clientID");
    }

    while (true)
    {
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        else
        {
            frameStartTime = GetTickCount64();  // 프레임 시작 시간

            // 게임 프레임 업데이트
            frameTime = 1.0f / 60.0f;  // 약 60FPS로 가정
            gameframework.Update(frameTime, sock);
            InvalidateRect(GetActiveWindow(), NULL, FALSE);

            // 프레임이 너무 빨리 그려지는 경우 대기
            frameEndTime = GetTickCount64();
            DWORD frameElapsedTime = frameEndTime - frameStartTime;
            if (frameElapsedTime < GAME_FRAME_RATE)
            {
                Sleep(GAME_FRAME_RATE - frameElapsedTime);
            }
        }
    }

    gameframework.Clear();

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = lpszClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    HWND hWnd = CreateWindowW(
        lpszClass,
        lpszWindowName,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, // 이 스타일로 윈도우 크기 조절 비활성화
        wIndowxPos,     // 생성하는 윈도우가 화면의 중앙에 위치
        windowyPos,
        CLIENT_WIDTH,
        CLIENT_HEIGHT,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // 클라이언트 영역에 들어오면 커서를 숨기고, 나가면 보이도록 설정
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(tme);
    tme.dwFlags = TME_LEAVE;
    tme.hwndTrack = hWnd;
    TrackMouseEvent(&tme);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        gameframework.Create(hWnd);
        SetTimer(hWnd, GAME_TIMER, GAME_FRAME_RATE, NULL);
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        gameframework.Draw(hdc);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_TIMER:
        gameframework.Update(0.016f,sock); // 약 60FPS로 가정 (1초 / 60프레임)
        InvalidateRect(hWnd, NULL, FALSE);
        break;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            gameframework.TogglePause();
            break;
        }
    case WM_KEYUP:
        gameframework.OnKeyBoardProcessing(message, wParam, lParam, sock);
        break;

    case WM_LBUTTONDOWN:
    case WM_MOUSEMOVE:
        gameframework.OnMouseProcessing(message, wParam, lParam);
        ShowCursor(FALSE); // 클라이언트 영역에서는 커서를 숨김
        break;

    case WM_MOUSELEAVE:
        ShowCursor(TRUE); // 클라이언트 영역을 벗어나면 커서를 보임
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


//대화상자 프로시저
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_INITDIALOG:
    {
        hEdit1 = GetDlgItem(hDlg, IDC_EDIT2);
        SendMessage(hEdit1, EM_SETLIMITTEXT, 20, 0);
        return TRUE;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
            char buf[20];
            GetDlgItemTextA(hDlg, IDC_EDIT2, buf, sizeof(buf));
            strncpy_s(client.name, buf, sizeof(client.name) - 1); // 받아온 문자열을 client객체에 저장
            client.name[sizeof(client.name) - 1] = '\0'; // NULL 문자 보장
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

