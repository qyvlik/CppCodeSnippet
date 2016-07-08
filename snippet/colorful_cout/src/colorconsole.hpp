#ifndef COLORCONSOLE_HPP
#define COLORCONSOLE_HPP

#include <iostream>

#if defined (_WIN32) || defined (_WIN64)
    #include <windows.h>
#endif

namespace qyvlik {

class ColorConsole {

public:
    enum ConsoleFontColor {
        Default,
        Green,
        Blue,
        Red,
        White = Default,      // Default
    };

    ColorConsole():
        mFontColor(Default)
    {}

    void setFontColor(ConsoleFontColor fontColor) {
        mFontColor = fontColor;
    }

    ConsoleFontColor fontColor() const {
        return mFontColor;
    }

    void print(const std::string& str)  {
        if(mFontColor != Default) {
#if defined (_WIN32) || defined (_WIN64)
            HANDLE hOut;

            //  获取输出流的句柄
            hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            //            SetConsoleTextAttribute(hOut,
            //                                    FOREGROUND_GREEN |      // 前景色_绿色
            //                                    FOREGROUND_INTENSITY ); // 前景色_加强

            switch(mFontColor)
            {
            case Green:
                SetConsoleTextAttribute(hOut, FOREGROUND_GREEN );
                break;
            case Blue:
                SetConsoleTextAttribute(hOut, FOREGROUND_BLUE );

                break;
            case Red:
                SetConsoleTextAttribute(hOut, FOREGROUND_RED );

                break;
            case White:
                SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE  );
            default:
                break;
            }
#else
            // echo "\033[字背景颜色;字体颜色m字符串\033[0m"
            switch(mFontColor)
            {
            case Green:
                std::cout << "\033[32m";
                break;
            case Blue:
                std::cout << "\033[34m";
                break;
            case Red:
                std::cout << "\033[31m";
                break;
            case White:
                std::cout << "\033[0m";
            default:
                break;
            }
#endif
            std::cout << str ;

#if defined (_WIN32) || defined (_WIN64)
            SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
            std::cout << "\033[0m";         // 关闭所有设置
#endif
            mFontColor = Default;
        } else {
            std::cout << str ;
        }
    }

private:
    ConsoleFontColor mFontColor;
};


}

#endif // COLORCONSOLE_HPP

