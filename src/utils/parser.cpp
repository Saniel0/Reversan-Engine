/*
    This file is part of Reversan Engine.

    Reversan Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Reversan Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Reversan Engine. If not, see <https://www.gnu.org/licenses/>. 
*/

#include "utils/parser.h"
#include <iostream>

Parser::Parser() : 
    mode(DefaultSettings::MODE),
    style(DefaultSettings::STYLE),
    alg(DefaultSettings::ALG),
    settings(DefaultSettings::SETTINGS)
{}

App::Mode Parser::get_mode() const {return mode;}
UI::UIStyle Parser::get_style() const {return style;}
Engine::Alg Parser::get_alg() const {return alg;}
Engine::Settings Parser::get_settings() const {return settings;}

void Parser::print_help() const {
    std::cout 
        << "Usage: reversan [MODE] [OPTIONS]\n"
        << "\n"
        << "Modes:\n"
        << "--help, -h                                Display this help message.\n"
        << "--play                                    Play against the engine in terminal interface.\n"
        << "--bot-vs-bot                              Start game where the engine plays against itself.\n"
        << "--benchmark                               Run search on pre-defined state.\n"
        << "\n"
        << "Additional Options:\n"
        << "--depth, -d <1 - 49> [10]                           Set the engine's search depth.\n"
        << "--engine, -e <negascout | alphabeta> [negascout]    Choose the tree search algorithm.\n"
        << "--threads, -t, <1 - 8> [1]                          EXPERIMENTAL, negascout only.\n"
        << "--disable-tp                                        Disables transposition tables.\n"
        << "--order, -o <line_by_line | opt1 | opt2> [opt1]     Sets search order of the engine.\n"
        << "--style, -s <basic | solarized | dracula> [basic]   Specify UI style.\n";
}

bool Parser::parse_mode(int argc, char **argv) {
    // check arg count
    if (argc < 2) {
        return false;
    }
    // try to parse mode
    std::string arg = argv[1];
    if (arg == "--play") mode = App::Mode::PLAY;
    else if (arg == "--bot-vs-bot") mode = App::Mode::BOT_VS_BOT;
    else if (arg == "--benchmark") mode = App::Mode::BENCHMARK;
    else return false;
    // return true if mode was parsed
    return true;
}

bool Parser::parse_depth(int argc, char **argv, int &i) {
    if (i + 1 < argc) {
        i++;
        settings.search_depth = std::atoi(argv[i]);
        if (settings.search_depth < 1 || settings.search_depth > 50) {
            std::cout << "Invalid depth. Use --help or -h for usage information.\n";
            return false;
        }
    }
    else {
        std::cout << "Invalid use of depth. Use --help or -h for usage information.\n";
        return false;
    }
    return true;
}

bool Parser::parse_engine(int argc, char **argv, int &i) {
    if (i + 1 < argc) {
        i++;
        std::string arg = argv[i];
        if (arg == "alphabeta") alg = Engine::Alg::ALPHABETA;
        else if (arg == "negascout") alg = Engine::Alg::NEGASCOUT;
        else {
            std::cout << "Invalid search engine. Use --help or -h for usage information.\n";
            return false;
        }
    }
    else {
        std::cout << "Flags --engine and -e require an additional argument. Use --help or -h for usage information.\n";
        return false;
    }
    return true;
}

bool Parser::parse_style(int argc, char **argv, int &i) {
    if (i + 1 < argc) {
        i++;
        std::string arg = argv[i];
        if (arg == "basic") style = UI::UIStyle::BASIC;
        else if (arg == "solarized") style = UI::UIStyle::SOLARIZED;
        else if (arg == "dracula") style = UI::UIStyle::DRACULA;
        else {
            std::cout << "Invalid style. Use --help or -h for usage information.\n";
            return false;
        }
    }
    else {
        std::cout << "Flags --style and -s require an additional argument. Use --help or -h for usage information.\n";
        return false;
    }
    return true;
}

bool Parser::parse_threads(int argc, char **argv, int &i) {
    if (i + 1 < argc) {
        i++;
        settings.thread_count = std::atoi(argv[i]);
        if (settings.thread_count < 1 || settings.thread_count > 8) {
            std::cout << "Invalid thread count. Use --help or -h for usage information.\n";
            return false;
        }
    }
    else {
        std::cout << "Invalid use of thread. Use --help or -h for usage information.\n";
        return false;
    }
    return true;
}

bool Parser::parse_order(int argc, char **argv, int &i) {
    if (i + 1 < argc) {
        i++;
        std::string arg = argv[i];
        if (arg == "line_by_line") settings.order = Move_order::Orders::LINE_BY_LINE;
        else if (arg == "opt1") settings.order = Move_order::Orders::OPTIMIZED;
        else if (arg == "opt2") settings.order = Move_order::Orders::OPTIMIZED2;
        else {
            std::cout << "Invalid order. Use --help or -h for usage information.\n";
            return false;
        }
    }
    else {
        std::cout << "Flags --order and -o require an additional argument. Use --help or -h for usage information.\n";
        return false;
    }
    return true;
}

bool Parser::parse(int argc, char **argv) {
    int idx = 1;
    // parse mode
    if (parse_mode(argc, argv)) {
        idx++;
    }
    // parse options
    for (int i = idx; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--depth" || arg == "-d") {
            if (!parse_depth(argc, argv, i)) return false;
        }
        else if (arg == "--engine" || arg == "-e") {
            if (!parse_engine(argc, argv, i)) return false;
        }
        else if (arg == "--style" || arg == "-s") {
            if (!parse_style(argc, argv, i)) return false;
        }
        else if (arg == "--threads" || arg == "-t") {
            if (!parse_threads(argc, argv, i)) return false;
        }
        else if (arg == "--help" || arg == "-h") {
            print_help();
            return false;
        }
        else if (arg == "--disable-tp") {
            settings.transposition_enable = false;
        }
        else if (arg == "--order" || arg == "-o") {
            if (!parse_order(argc, argv, i)) return false;
        }
        else {
            std::cout << "Invalid option. Use --help or -h for usage information.\n";
            return false;
        }
    }
    // if we got here, everything was correctly parsed
    return true;
}


