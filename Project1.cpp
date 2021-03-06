#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <process.h>

// Forward declarations
void emitter(void *param);
LRESULT CALLBACK listener(int nCode, WPARAM wParam, LPARAM lParam);

// Globals
int enabled = 0;


/**
 * Entry point
 */
int main()
{
	// Create emitter
	printf("Starting emitter.\n", "%s");
	_beginthread(emitter, 0, NULL);

	// Create listener
	printf("Starting listener.\n", "%s");
	HHOOK KeyboardHook = SetWindowsHookEx(
		WH_KEYBOARD_LL, listener, GetModuleHandle(NULL), NULL
	);
	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return 0;
}


/**
 * Emits key events
 * http://www.philipstorr.id.au/pcbook/book3/scancode.htm
 */
void emitter(void *param)
{
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.time = 0;
	ip.ki.wVk = 0; // Use scan codes instead of vks
	ip.ki.dwExtraInfo = 0;

	while (1)
	{
		if (enabled)
		{
			// SHIFT UP
			ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
			SendInput(1, &ip, sizeof(INPUT));
			printf("Shift up\n", "%s");

			// WAIT
			Sleep(rand() % 100); // short

			// 5 DOWN
			ip.ki.dwFlags = KEYEVENTF_SCANCODE;
			ip.ki.wScan = 0x06;  //Set a unicode character to use (A)
			SendInput(1, &ip, sizeof(INPUT));
			printf("Shift down\n", "%s");

			// WAIT
			Sleep(rand() % 100); // short

			// 5 UP
			ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
			SendInput(1, &ip, sizeof(INPUT));
			printf("5 up\n", "%s");

			// WAIT
			Sleep(rand() % 100); // short

			// W UP
			ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
			SendInput(1, &ip, sizeof(INPUT));
			printf("W up\n", "%s");

			// WAIT
			Sleep(rand() % 100); // short

			// W DOWN
			ip.ki.dwFlags = KEYEVENTF_SCANCODE;
			ip.ki.wScan = 0x2A; // SHIFT
			SendInput(1, &ip, sizeof(INPUT));
			printf("W down\n", "%s");

			// WAIT
			Sleep(rand() % 100); // short

			// SHIFT - DOWN
			ip.ki.dwFlags = KEYEVENTF_SCANCODE;
			ip.ki.wScan = 0x2A; // SHIFT
			SendInput(1, &ip, sizeof(INPUT));
			printf("Shift down\n", "%s");

			// WAIT
			Sleep(rand() % 10 * 1000); // long
		}

		// WAIT
		Sleep(rand() % 100); // short
	}
}


/**
* Listens for key events
* DLL Hook
*/
LRESULT CALLBACK listener(int nCode, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;
	if (nCode == HC_ACTION) // Check if this is a keyboard message
	{
		if (wParam == WM_SYSKEYDOWN || wParam == WM_KEYDOWN) // Detect keydown
		{
			if (p->vkCode == VK_OEM_3) // VK_OEM_3 == `
			{
				enabled = !enabled;
				printf("Listener enabled: %d\n", enabled);
			}
		}
	}
	// Next
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}
