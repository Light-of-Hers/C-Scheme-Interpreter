#include "si-driver.h"
#include "si-error.h"
#include "si-eval-env.h"
#include "si-eval-prim.h"
#include "si-eval.h"
#include "si-global.h"
#include "si-io.h"
#include "si-regs.h"
#include "si-stack.h"
#include "si-value.h"
#include <cstdlib>
#include <iostream>

namespace si {

const static String input(">> ");
const static String output("<< ");

void driver() {
Begin:
    initGlobal();
    initStack();
    initRegs();
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
            std::cerr << "Error: " << e.msg << std::endl;
            if (e.handle == Error::Handle::REBOOT) {
                std::cerr << "Reboot..." << std::endl;
                goto Begin;
            } else if (e.handle == Error::Handle::ABORT) {
                std::cerr << "Abort!" << std::endl;
                std::exit(0);
            }
        }
        std::cout << input;
    }
    std::cout << std::endl;
}

}; // namespace si