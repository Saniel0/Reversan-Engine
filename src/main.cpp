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

#include "terminal.h"
#include "app.h"
#include <signal.h>

// needs to be file-global to be accessible in sig function
static Terminal ui(UI::UIStyle::BASIC);

// restores terminal state even after ctrl-c or other failure
void handle_sig(int sig) {
    // since ui is static, it gets automatically deconstructed
    exit(sig);
}

int main(int argc, char **argv) {
    // prepare signal handler
    signal(SIGINT, handle_sig);
    
    // initialize the app
    App app(&ui);
    
    // try to parse settings and run the app
    if (app.parse_settings(argc, argv)) {
        app.run();
    }
    
    return 0;
}
