#include"vt100.h"
#include"asprintf.h"
#include"cprintf.h"

const char* fgColorTemplate = "\x1b[38;2;%hhu;%hhu;%hhum";
const char* bgColorTemplate = "\x1b[48;2;%hhu;%hhu;%hhum";
#define MAX_COLORCODE_LENGHT 20
typedef char ColorCode[MAX_COLORCODE_LENGHT];

#define DEFAULT_COLOR {'\x1b', '[', '3', '8', ';', '2', ';', '2', '5', '5', ';', '2', '5', '5', ';', '2', '5', '5', 'm', '\0'}
constexpr ColorCode default_color = DEFAULT_COLOR;

typedef char alias_t[16];

struct Color {
	alias_t name;
	unsigned char r, g, b;
};

const Color AliasColorTable[] = {
	//     name     r     g     b
		{"current",0x00, 0x00, 0x00},
		{"prev",   0x00, 0x00, 0x00},
		{"red",    0xff, 0x00, 0x00},
		{"orange", 0xff, 0xa5, 0x00},
		{"yellow", 0xff, 0xff, 0x00},
		{"green",  0x00, 0xff, 0x00},
		{"cyan",   0x00, 0xff, 0xff},
		{"blue",   0x00, 0x00, 0xff},
		{"purple", 0xa0, 0x20, 0xf0},
		{"white",  0xff, 0xff, 0xff},
		{"gray",   0x7f, 0x7f, 0x7f}
};

enum {
	CURRENT = 0,
	PREVIOUS,
	END_HISTORY = 7
};

static ColorCode history[END_HISTORY + 1] = {
		DEFAULT_COLOR, DEFAULT_COLOR,
		DEFAULT_COLOR, DEFAULT_COLOR,
		DEFAULT_COLOR, DEFAULT_COLOR,
		DEFAULT_COLOR, DEFAULT_COLOR
};
static ColorCode last = DEFAULT_COLOR;

inline void UpdateHistory(unsigned char r, unsigned char g, unsigned char b) {
	for (int i = END_HISTORY - 1; i >= CURRENT; --i) {
		strncpy_s(history[i + 1], history[i], MAX_COLORCODE_LENGHT);
	}
	snprintf(history[CURRENT], MAX_COLORCODE_LENGHT, fgColorTemplate, r, g, b);
	strncpy_s(last, history[CURRENT], MAX_COLORCODE_LENGHT);
}

int cprintf(const char* __fmt, ...) {

	char* formatted = nullptr;
	va_list args;
	va_start(args, __fmt);
	int flen = vasprintf(&formatted, __fmt, args);
	if (flen == -1 || !formatted) return -1;
	va_end(args);

	printf("%s", last);
	char* cursor = formatted;
	while (*cursor) {
		if (*(cursor) == '$' && *(cursor + 1) == '#') {
			putchar('#'); cursor += 2;
		}
		else if (*(cursor) == '#') {
			cursor++;
			Color alias;
			size_t aliasLen = 0;
			size_t remainLen = strlen(cursor);
			int tableLen = sizeof(AliasColorTable) / sizeof(Color);

			for (int ialias = 0; ialias < tableLen; ++ialias) {
				alias = AliasColorTable[ialias];
				aliasLen = strlen(alias.name);
				if (remainLen < aliasLen || strncmp(alias.name, cursor, aliasLen) != 0) {
					aliasLen = -1;
					continue;
				}

				if (ialias > 1) {
					UpdateHistory(alias.r, alias.g, alias.b);
					printf("%s", history[CURRENT]);
					break;
				}

				int extra, extraLen = 2;
				if (sscanf_s(cursor, "prev+%d", &extra) != 1) {
					extra = 0;
					extraLen = 0;
				}

				int hindex = ialias + extra;
				aliasLen += extraLen;
				printf("%s", history[hindex]);
				strncpy_s(last, history[hindex], MAX_COLORCODE_LENGHT);
				break;
			}

			if (aliasLen != -1) {
				cursor += aliasLen;
				continue;
			}

			// hex processing
			int hexval = 0, i = 0;
			for (; i < 6 && i < remainLen; ++i) {
				char ch = *(cursor + i);
				int value = isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
				if (value >= 0 && value <= 15) {
					hexval = (hexval << 4) | value;
				}
			}
			alias.r = (hexval >> 16) & 0xFF;
			alias.g = (hexval >> 8) & 0xFF;
			alias.b = hexval & 0xFF;
			
			UpdateHistory(alias.r, alias.g, alias.b);
			printf("%s", history[CURRENT]);
			cursor += i;
		}
		else {
			putchar(*(cursor++));
		}
	}
	printf("%s", default_color);

	free(formatted);
	return flen;
}
