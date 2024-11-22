#pragma once
#include <string>
#include "argparse/include/argparse/argparse.hpp"

struct Args : public argparse::Args {
        /* FIXME: change this to also work with a file that only contains the
         * release string for example /etc/redhat-release */
        std::string& verinfo = kwarg("r,release", "The location of the "
                                     "os-release / os name file")
            .set_default(std::string("/etc/os-release"));

        std::string& layout = kwarg("l,layout", "The ordering and layout of"
                                    " information printed to the screen.")
            .set_default(std::string(""));

        std::string& script = kwarg("s,script", "Input of a custom feature"
                                    "in the form of a lua script filename")
            .set_default(std::string(""));

};
