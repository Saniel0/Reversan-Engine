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

#include "app.h"
#include "terminal.h"
#include "negascout.h"
#include "alphabeta.h"
#include "parser.h"
#include <signal.h>

// needs to be file-global to be accessible in sig function
static UI *ui = nullptr;
static Engine *engine = nullptr;

// restores terminal state even after ctrl-c or other failure
void handle_sig(int sig) {
    // safely dealocate resources
    if (ui) delete ui;
    if (engine) delete engine;
    exit(sig);
}

int main(int argc, char **argv) {
    // prepare signal handler
    signal(SIGINT, handle_sig);

    // parse arguments
    Parser parser;
    if (!parser.parse(argc, argv)) return 1;

    // initialize engine
    if (parser.get_alg() == Engine::Alg::ALPHABETA) {
        engine = new Alphabeta(parser.get_settings());
    }
    else if (parser.get_alg() == Engine::Alg::NEGASCOUT && parser.get_settings().thread_count > 1) {
        engine = new NegascoutParallel(parser.get_settings());
    }
    else {
        engine = new Negascout(parser.get_settings());
    }

    // initialize terminal
    ui = new Terminal(parser.get_style());

    // initialize app
    App app(parser.get_mode(), ui, engine);
    app.run();

    // dealocate resources and exit
    delete ui;
    delete engine;
    return 0;
}
