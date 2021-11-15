#pragma once

#include <kern.hpp>

namespace hw::uart {

    void init();

    void write(char c);
    void write(const char *string);

}