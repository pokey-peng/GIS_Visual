#ifndef _MessageHandler_H
#define _MessageHandler_H

///处理所有消息入口
LRESULT handleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

///处理菜单消息
LRESULT handleCommandMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

///处理按键消息
LRESULT handleKeyMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

///处理鼠标消息
LRESULT handleMouseMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

///处理绘制消息
LRESULT handlePaintMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif




