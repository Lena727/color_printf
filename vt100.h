#ifndef _VT100_INITIALIZER_H
#define _VT100_INITIALIZER_H

#ifdef _WIN32
#include<Windows.h>

class VT100Initalizer {
public:
	VT100Initalizer() {
		EnableVT100();
	}

	~VT100Initalizer() {
		DisableVT100();
	}

	operator bool() const {
		return IsEnabled;
	}

private:
	static bool IsEnabled;
	static HANDLE hConsole;

	static void EnableVT100() {
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hConsole == INVALID_HANDLE_VALUE) {
			return;
		}

		DWORD Modes;
		if (!GetConsoleMode(hConsole, &Modes)) {
			return;
		}
		if (Modes & ENABLE_VIRTUAL_TERMINAL_PROCESSING) {
			return;
		}

		Modes |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		if (!SetConsoleMode(hConsole, Modes)) {
			return;
		}
		IsEnabled = true;
	}

	static void DisableVT100() {
		if (!IsEnabled || hConsole == INVALID_HANDLE_VALUE) return;

		DWORD Modes;
		if (!GetConsoleMode(hConsole, &Modes)) {
			return;
		}
		if (!(Modes & ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
			return;
		}

		Modes &= ~ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		if (!SetConsoleMode(hConsole, Modes)) {
			return;
		}
		
		IsEnabled = false;
	}
};

bool VT100Initalizer::IsEnabled = false;
HANDLE VT100Initalizer::hConsole = INVALID_HANDLE_VALUE;

static VT100Initalizer vt100;
#else
static constexpr bool vt100 = true
#endif

#endif