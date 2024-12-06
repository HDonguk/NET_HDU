#pragma once

// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg);


// 소켓 함수 오류 출력
void err_display(const char* msg);

// 소켓 함수 오류 출력
void err_display(int errcode);