#pragma once

#include <iostream>
#include <string>

HANDLE hConsole2 = GetStdHandle(STD_OUTPUT_HANDLE);

void printCenteredColoredText(const std::string text, int colorCode) {
    // Berechnung der Anzahl von Leerzeichen, um den Text mittig auszurichten
    int leftPadding = (120 - text.length()) / 2;

    // Ausgabe von Leerzeichen für die Ausrichtung
    for (int i = 0; i < leftPadding; ++i) {
        std::cout << " ";
    }
    SetConsoleTextAttribute(hConsole2, colorCode);
    // Setzen der Textfarbe
    std::cout << text << std::endl;
}

void printLogWithoutLn(const std::string text)
{
    SetConsoleTextAttribute(hConsole2, 8);
    std::cout << "[";

    SetConsoleTextAttribute(hConsole2, 9);
    std::cout << ">";

    SetConsoleTextAttribute(hConsole2, 8);
    std::cout << "] ";

    SetConsoleTextAttribute(hConsole2, 15);
    std::cout << text;
}

void printLog(const std::string text)
{
    SetConsoleTextAttribute(hConsole2, 8);
    std::cout << "[";

    SetConsoleTextAttribute(hConsole2, 9);
    std::cout << ">";

    SetConsoleTextAttribute(hConsole2, 8);
    std::cout << "] ";

    SetConsoleTextAttribute(hConsole2, 15);
    std::cout << text << std::endl;
}

void printError(const std::string text) {
    SetConsoleTextAttribute(hConsole2, 8);
    std::cout << "[";

    SetConsoleTextAttribute(hConsole2, FOREGROUND_RED);
    std::cout << "!";

    SetConsoleTextAttribute(hConsole2, 8);
    std::cout << "] ";

    SetConsoleTextAttribute(hConsole2, 15);
    std::cout << text << std::endl;
}