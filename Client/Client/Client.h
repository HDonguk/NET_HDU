#pragma once
//#include "stdafx.h"
//
//#define WM_CHANGE_CURSOR (WM_USER + 1)
//
//class Client {
//public:
//    Client();
//    ~Client();
//
//    void Create(HWND hWnd);
//    void Update(float frameTime);
//    void Draw(HDC hdc);
//    void OnKeyBoardProcessing(UINT iMessage, WPARAM wParam, LPARAM lParam);
//    void OnMouseProcessing(UINT iMessage, WPARAM wParam, LPARAM lParam);
//    void Clear();
//
//private:
//    void InitializeDoubleBuffering(HDC hdc);
//    void CleanupDoubleBuffering();
//
//    HWND m_hWnd;
//    HDC m_hdcBackBuffer;
//    HBITMAP m_hBitmap;
//    HBITMAP m_hOldBitmap;
//
//    CImage mapImage;
//    Player* player; // Player °´Ã¼ Ãß°¡
//    Camera* camera; // Camera °´Ã¼ Ãß°¡
//
//    CImage cursorImage;
//    CImage clickImage;
//    bool showClickImage;
//    float clickImageTimer;
//};
