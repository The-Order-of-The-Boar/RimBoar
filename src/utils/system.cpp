// header
#include "system.hpp"

// builtin
#include <filesystem>



std::filesystem::path get_binary_path()
{
#ifdef __linux__

    return std::filesystem::read_symlink("/proc/self/exe");

#else

    #error unsupported platform

#endif
}
