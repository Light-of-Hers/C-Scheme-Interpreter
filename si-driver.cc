#include "si-driver.h"
#include "si-error.h"
#include "si-eval-env.h"
#include "si-eval-prim.h"
#include "si-eval.h"
#include "si-global.h"
#include "si-io.h"
#include "si-value.h"
#include <cstdlib>
#include <iostream>

namespace si {

const static String input(">> ");
const static String output("<< ");

void driver() {
Begin:
    initGlobal();
    auto gb = extend_env(nil, nil, nil);
    initPProc(gb);
    Var x;
    std::cout << input;
    while (1) {
        try {
            if (!(std::cin >> x))
                break;
            auto out = eval(x, gb);
            std::cout << output << out << std::endl;
        } catch (Error &e) {
            std::cout << "Error: " << e.msg << std::endl;
            if (e.handle == Error::Handle::REBOOT) {
                std::cout << "Reboot..." << std::endl;
                goto Begin;
            } else if (e.handle == Error::Handle::ABORT) {
                std::cout << "Abort!" << std::endl;
                std::exit(0);
            }
        }
        std::cout << input;
    }
    std::cout << std::endl;
}

}; // namespace si