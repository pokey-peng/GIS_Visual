#ifndef _MessageHandler_H
#define _MessageHandler_H

///����������Ϣ���
LRESULT handleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

///����˵���Ϣ
LRESULT handleCommandMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

///��������Ϣ
LRESULT handleKeyMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

///���������Ϣ
LRESULT handleMouseMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

///���������Ϣ
LRESULT handlePaintMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif




