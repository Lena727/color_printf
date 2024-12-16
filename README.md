# cprintf Function: Enhanced Formatted Output with Color Support

## Overview

`cprintf` is a custom C function designed to provide enhanced formatted output to the console. It extends the standard `printf` by adding support for colored text using ANSI escape sequences. Colors can be specified inline within the format string using simple aliases, hexadecimal color codes, or history-based references.

## Features

- **Inline Color Specification**: Specify text colors directly within the format string using:
  - Color aliases (e.g., `#red`, `#blue`)
  - Hexadecimal RGB values (e.g., `#FF00FF`)
  - History-based references (e.g., `#current`, `#prev`)
- **ANSI Escape Sequences**: Outputs ANSI color codes for seamless integration with VT100-compatible terminals.
- **Color History Management**: Maintains a history of previously used colors for quick reuse.

## Syntax

```c
int cprintf(const char* __fmt, ...);
```

### Parameters

- `__fmt`: A format string similar to `printf`, with additional syntax for specifying colors.
- `...`: Variadic arguments for the format string.

### Return Value

The function returns the number of characters printed, or `-1` in case of an error.

## Color Syntax

### 1. **Color Aliases**

The following predefined aliases can be used:

| Alias      | RGB Value       |
| ---------- | --------------- |
| `#current` | `0, 0, 0`       |
| `#prev`    | `0, 0, 0`       |
| `#red`     | `255, 0, 0`     |
| `#blue`    | `0, 0, 255`     |
| `#green`   | `0, 255, 0`     |
| `#cyan`    | `0, 255, 255`   |
| `#orange`  | `255, 165, 0`   |
| `#yellow`  | `255, 255, 0`   |
| `#purple`  | `160, 32, 240`  |
| `#white`   | `255, 255, 255` |
| `#gray`    | `127, 127, 127` |

### 2. **Hexadecimal Colors**

Specify colors in `#RRGGBB` format. For example, `#FF00FF` sets the text color to magenta.

### 3. **History-Based References**

Use history-based references to reuse colors:

- `#current`: The current used color.
- `#prev`: The most recently used color.
- `#prev+N`: A color used `N` (0 < `N` < 7) steps back in the history.

## Example Usage

```c
cprintf("Hello, #redWorld#white!\n");
cprintf("This is a custom #00FF60green #whitetext example.\n");
cprintf("Reusing colors: #prevHello again!\n");
cprintf("Another color: #blueBlue text here.#white\n");
```

![result](https://github.com/Lena727/color_printf/blob/main/test_result.png)

## How It Works

### Core Logic

1. **History Management**: Maintains an array of ANSI color codes representing the history of used colors. Each **new** color updates the history.
2. **Color Parsing**: Detects and processes inline color syntax:
   - Matches predefined aliases.
   - Parses `#RRGGBB` for custom colors.
   - Handles `#prev` and `#prev+N` for history-based references.
3. **ANSI Code Generation**: Constructs appropriate ANSI color codes based on RGB values.

## Notes

- Ensure your terminal supports ANSI escape sequences for proper color rendering.
- Avoid nesting or overlapping color specifications.

## License

This code is distributed under the MIT License.

---

### Special Thanks

Thanks to this [thread](https://stackoverflow.com/questions/40159892/using-asprintf-on-windows/46480387#46480387) for asprintf.h

