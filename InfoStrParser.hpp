#pragma once
#include <string>
#include "argparse/include/argparse/argparse.hpp"

struct Args : public argparse::Args {
        /* FIXME: change this to also work with a file that only contains the
         * release string for example /etc/redhat-release */
        std::string& verinfo = kwarg("ver", "The location of the os-release file")
            .set_default(std::string("/etc/os-release"));
};
