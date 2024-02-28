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

auto name = skCrypt("RenewableFortnite"); // application name. right above the blurred text aka the secret on the licenses tab among other tabs
auto ownerid = skCrypt("qdPVeuvG2s"); // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
auto secret = skCrypt("792fa26d46a0cad7a742e894a9a9678cc1cf86ec537b8eaf257000c8a5921383"); // app secret, the blurred text on licenses tab and other tabs
auto version = skCrypt("1.0"); // leave alone unless you've changed version on website
auto url = skCrypt("https://keyauth.win/api/1.2/"); // change if you're self-hosting

api KeyAuthApp(name.decrypt(), ownerid.decrypt(), secret.decrypt(), version.decrypt(), url.decrypt());

void keyauth::check()
{
    // Freeing memory to prevent memory leak or memory scraping
    name.clear(); ownerid.clear(); secret.clear(); version.clear(); url.clear();

    std::cout << skCrypt("\n\n Connecting..");
    KeyAuthApp.init();
    if (!KeyAuthApp.data.success)
    {
        std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
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
            std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
            Sleep(1500);
            exit(1);
        }

        keyauth::KeyAuthCheckPassed = KeyAuthApp.data.success;
        std::cout << skCrypt("\n\n Successfully Automatically Logged In\n");
    }
    else
    {
        std::string key;

        std::cout << skCrypt("\n Enter license: ");
        std::cin >> key;
        KeyAuthApp.license(key);

 

        if (!KeyAuthApp.data.success)
        {
            std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
            Sleep(1500);
            exit(1);
        }

        keyauth::KeyAuthCheckPassed = KeyAuthApp.data.success;
        WriteToJson(KeyPath, "license", key, false, "", "");
        std::cout << skCrypt("Successfully Created File For Auto Login");
       
    }

    std::cout << skCrypt("\n User data:");
    std::cout << skCrypt("\n Username: ") << KeyAuthApp.data.username;
    std::cout << skCrypt("\n IP address: ") << KeyAuthApp.data.ip;
    std::cout << skCrypt("\n Hardware-Id: ") << KeyAuthApp.data.hwid;
    std::cout << skCrypt("\n Create date: ") << tm_to_readable_time(timet_to_tm(string_to_timet(KeyAuthApp.data.createdate)));
    std::cout << skCrypt("\n Last login: ") << tm_to_readable_time(timet_to_tm(string_to_timet(KeyAuthApp.data.lastlogin)));
    std::cout << skCrypt("\n Subscription(s): ");

    for (int i = 0; i < KeyAuthApp.data.subscriptions.size(); i++) {
        auto sub = KeyAuthApp.data.subscriptions.at(i);
        std::cout << skCrypt("\n name: ") << sub.name;
        std::cout << skCrypt(" : expiry: ") << tm_to_readable_time(timet_to_tm(string_to_timet(sub.expiry)));
    }

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
