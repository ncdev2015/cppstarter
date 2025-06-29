#ifndef COLORS_HPP
#define COLORS_HPP

#include <string>

namespace colors {
    // Text colors
    constexpr char BLACK[]   = "\033[30m";
    constexpr char RED[]     = "\033[31m";
    constexpr char GREEN[]   = "\033[32m";
    constexpr char YELLOW[]  = "\033[33m";
    constexpr char BLUE[]    = "\033[34m";
    constexpr char MAGENTA[] = "\033[35m";
    constexpr char CYAN[]    = "\033[36m";
    constexpr char WHITE[]   = "\033[37m";

    // Background colors
    constexpr char BG_BLACK[]   = "\033[40m";
    constexpr char BG_RED[]     = "\033[41m";
    constexpr char BG_GREEN[]   = "\033[42m";
    constexpr char BG_YELLOW[]  = "\033[43m";
    constexpr char BG_BLUE[]    = "\033[44m";
    constexpr char BG_MAGENTA[] = "\033[45m";
    constexpr char BG_CYAN[]    = "\033[46m";
    constexpr char BG_WHITE[]   = "\033[47m";

    // Text styles
    constexpr char BOLD[]      = "\033[1m";
    constexpr char UNDERLINE[] = "\033[4m";
    constexpr char INVERT[]    = "\033[7m";

    // Reset to default color
    constexpr char RESET[] = "\033[0m";

    // Apply one color code to a string
    inline std::string applyColor(const std::string& text, const char* colorCode) {
        return std::string(colorCode) + text + RESET;
    }

    // Apply two color codes (e.g., text color + background)
    inline std::string applyColor(const std::string& text, const char* colorCode1, const char* colorCode2) {
        return std::string(colorCode1) + colorCode2 + text + RESET;
    }

    // Apply three color codes (e.g., text color + background + style)
    inline std::string applyColor(const std::string& text, const char* colorCode1, const char* colorCode2, const char* colorCode3) {
        return std::string(colorCode1) + colorCode2 + colorCode3 + text + RESET;
    }
}

#endif // COLORS_HPP
