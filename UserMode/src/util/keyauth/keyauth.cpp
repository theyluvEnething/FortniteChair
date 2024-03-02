#include "keyauth.h"
#include <Windows.h>
#include "auth.hpp"
#include <string>
#include "utils.hpp"
#include "../skStr.h"

std::string tm_to_readable_time(tm ctx);
static std::time_t string_to_timet(std::string timestamp);
static std::tm timet_to_tm(time_t timestamp);
const std::string compilation_date = (std::string)skCrypt(__DATE__);
const std::string compilation_time = (std::string)skCrypt(__TIME__);

using namespace KeyAuth;

auto name = skCrypt("renewablefn");
auto ownerid = skCrypt("A2NYsvYxMc");
auto secret = skCrypt("7b13d4d04afa332f4c0e2bf6e3f8452110298b6c6680ca125ef5b5948421b3ee");
auto version = skCrypt("1.0");
auto url = skCrypt("https://keyauth.win/api/1.2/"); // change if you're self-hosting

void printLog(const std::string text);
void printLogWithoutLn(const std::string text);

api KeyAuthApp(name.decrypt(), ownerid.decrypt(), secret.decrypt(), version.decrypt(), url.decrypt());

void keyauth::check()
{
    // Freeing memory to prevent memory leak or memory scraping
    name.clear(); ownerid.clear(); secret.clear(); version.clear(); url.clear();

    printLog(skCrypt("connecting...").decrypt());
    KeyAuthApp.init();
    if (!KeyAuthApp.data.success)
    {

        printLogWithoutLn(skCrypt("status: ").decrypt());
        std::cout << KeyAuthApp.data.message << std::endl;
        Sleep(1500);
        exit(1);
    }

    if (std::filesystem::exists(KeyPath)) //change test.txt to the path of your file :smile:
    {
        std::string key = ReadFromJson(KeyPath, "license");
        KeyAuthApp.license(key);
#
        if (!KeyAuthApp.data.success)
        {
            std::remove(KeyPath);
            printLogWithoutLn(skCrypt("status: ").decrypt());
            std::cout << KeyAuthApp.data.message << std::endl;
            Sleep(1500);
            exit(1);
        }

        keyauth::KeyAuthCheckPassed = KeyAuthApp.data.success;

        printLog(skCrypt("successfully automatically logged in").decrypt());
    }
    else
    {
        std::string key;

        printLogWithoutLn(skCrypt("enter license: ").decrypt());
        std::cin >> key;
        KeyAuthApp.license(key);

 

        if (!KeyAuthApp.data.success)
        {
            printLogWithoutLn(skCrypt("status: ").decrypt());
            std::cout << KeyAuthApp.data.message << std::endl;
            Sleep(1500);
            exit(1);
        }

        keyauth::KeyAuthCheckPassed = KeyAuthApp.data.success;
        WriteToJson(KeyPath, "license", key, false, "", "");
        //std::cout << skCrypt("Successfully Created File For Auto Login");
       
    }

    /*std::cout << skCrypt("\n User data:");
    std::cout << skCrypt("\n Username: ") << KeyAuthApp.data.username;
    std::cout << skCrypt("\n IP address: ") << KeyAuthApp.data.ip;
    std::cout << skCrypt("\n Hardware-Id: ") << KeyAuthApp.data.hwid;
    std::cout << skCrypt("\n Create date: ") << tm_to_readable_time(timet_to_tm(string_to_timet(KeyAuthApp.data.createdate)));
    std::cout << skCrypt("\n Last login: ") << tm_to_readable_time(timet_to_tm(string_to_timet(KeyAuthApp.data.lastlogin)));
    std::cout << skCrypt("\n Subscription(s): ");*/

    /*for (int i = 0; i < KeyAuthApp.data.subscriptions.size(); i++) {
        auto sub = KeyAuthApp.data.subscriptions.at(i);
        std::cout << skCrypt("\n name: ") << sub.name;
        std::cout << skCrypt(" : expiry: ") << tm_to_readable_time(timet_to_tm(string_to_timet(sub.expiry)));
    }*/

    std::cout << std::endl;
}

std::string tm_to_readable_time(tm ctx) {
    char buffer[80];

    strftime(buffer, sizeof(buffer), "%a %m/%d/%y %H:%M:%S %Z", &ctx);

    return std::string(buffer);
}

static std::time_t string_to_timet(std::string timestamp) {
    auto cv = strtol(timestamp.c_str(), NULL, 10); // long

    return (time_t)cv;
}

static std::tm timet_to_tm(time_t timestamp) {
    std::tm context;

    localtime_s(&context, &timestamp);

    return context;
}
