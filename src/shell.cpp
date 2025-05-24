#include<iostream>
#include<string>
#include "shell.hpp"

void Shell::run(){
    std::string input;
    while(true){
        printPrompt();
        std::getline(std::cin, input);

        if(std::cin.eof()){
            std::cout << '\n';
            break;
        }

        if(input.empty())continue;

        std::cout << "You Typed: " << input << std::endl;
    }
}

void Shell::printPrompt()const{
    std::cout << "ninxsh > " << std::flush;
}