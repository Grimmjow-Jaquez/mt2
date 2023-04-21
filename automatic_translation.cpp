#include <iostream>
#include <filesystem>
#include <fstream>
#include <map>

std::string g_currentPath = std::filesystem::current_path().string();

int main()
{
    std::cout << "Location current " << g_currentPath.c_str() << "\n";

    std::cout << "Hello! What do you want to do?\n";
    std::cout << "You can translate locale_game/interface.txt, itemdesc.txt, item_names.txt\n";
    std::cout << "Requirement: Add file which you want to translate in same directory as executable.\n";
    
    std::cout << "\nInsert name of the file text which you want to be translate..\n";

    std::string stNameToTranslate;
    std::cin >> stNameToTranslate;

    std::cout << "\nInsert name of the file text from which to get translations..\n";

    std::string stNameGetTranslate;
    std::cin >> stNameGetTranslate;

    std::cout << "\n1. Replace not founded texts with: NOT_TRANSLATED_TEXT\n";
    std::cout << "\n2. Let Text original\n";

    int option_replace = 1;
    std::cin >> option_replace;

    if (stNameToTranslate == stNameGetTranslate)
        return 0; // what u wanna do ?

    std::map<std::string, std::string> map_toTranslate;

    std::string stPathIntegral = g_currentPath + '/' + stNameToTranslate;
    std::ifstream fileTranslate(stPathIntegral.c_str());

    if (!fileTranslate.is_open())
    {
        std::cout << "File: " << stPathIntegral.c_str() << " don't exist.\n";
        return 0;
    }

    std::string line;
    while(getline(fileTranslate, line))
    {
        std::stringstream ss(line);
        std::vector<std::string> tokens;

        while (getline(ss, line, '\t'))
        {
            if (line.size())
                tokens.push_back(line);
        }
        
        if (tokens.size() < 2)
            continue;

        if (option_replace == 1)
            tokens[1] = "NOT_TRANSLATED_TEXT";

        map_toTranslate[tokens[0]] = tokens[1];
    }

    fileTranslate.close();

    stPathIntegral = "gen_" + stNameToTranslate;
    std::ofstream outputFile(stPathIntegral.c_str());

    stPathIntegral = g_currentPath + '/' + stNameGetTranslate;
    std::ifstream fileGetTranslate(stPathIntegral.c_str());

    if (!fileGetTranslate.is_open())
    {
        std::cout << "File: " << stPathIntegral.c_str() << " don't exist.\n";
        outputFile.close();
        return 0;
    }

    line = "";
    while (getline(fileGetTranslate, line))
    {
        std::stringstream ss(line);
        std::vector<std::string> tokens;

        while (getline(ss, line, '\t'))
        {
            if (line.size())
                tokens.push_back(line);
        }

        if (tokens.size() < 2)
            continue;

        auto iter = map_toTranslate.find(tokens[0]);
        if (iter != map_toTranslate.end())
            iter->second = tokens[1];
    }

    for (auto textes : map_toTranslate)
    {
        outputFile << textes.first << "\t" << textes.second.c_str() << "\n";
	    
        std::cout << "1->>>" << textes.first.c_str() << "\n";
        std::cout << "2->>>" << textes.second.c_str() << "\n";
    }

    fileGetTranslate.close();
    outputFile.close();

    std::cout << "File was generate with succes!";
    std::cin >> stNameToTranslate;

    return 1;
}
