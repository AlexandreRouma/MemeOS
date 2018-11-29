#include <stdint.h>
#include <libs/std/string.h>
#include <libs/std/stream.h>

class Terminal_Class {
    public:
        void Init(uint8_t width, uint8_t height);
        void setColor(char color);
        void putcar(char c, uint8_t x, uint8_t y);
        void print(char* str);
        void println(char* str);
        void print(string str);
        void println(string str);
        void setCursor(uint8_t x, uint8_t y);
        void clear();
        void scrollUp(uint8_t n);
        void scrollDown(uint8_t n);
        void newLine();
        void showCursor(uint8_t thickness);
        void hideCursor();
        void OK(bool newLine = true);
        void WARN(bool newLine = true);
        void FAILED(bool newLine = true);
        uint8_t getWidth();
        uint8_t getHeight();
        uint8_t getCursorX();
        uint8_t getCursorY();
        string readLine();
        
        stream<char, Terminal_Class> outStream;
private:
    static void _streamHandler(char*, int, Terminal_Class*);
};

extern Terminal_Class Terminal;