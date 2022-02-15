#include <args.hxx>
#include <filesystem>
#include <iostream>

int main(int argc, char **argv) {
    args::ArgumentParser parser("vcpkg repositories stats loader");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::Positional<std::filesystem::path> vcpkg_root_arg(parser, "vcpkg_root_arg", "where README.md is :)");

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help &) {
        std::cout << parser;
        return 0;
    } catch (args::ParseError &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    } catch (args::ValidationError &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 2;
    }

    if (!vcpkg_root_arg) {
        std::cerr << "No vcpkg_root_arg specified, sorry" << std::endl;
        return 3;
    }

    const auto vcpkg_root = args::get(vcpkg_root_arg);

    using namespace std::filesystem;

    if (!exists(vcpkg_root)) {
        std::cerr << vcpkg_root << " does not exist!" << std::endl;
        return 4;
    }

    const auto ports_path = canonical(vcpkg_root) / "ports";

    if (!exists(ports_path)) {
        std::cerr << "Something wrong with your copy of vcpkg - "
                  << "it does not contain `ports` subdirectory" << std::endl;
        return 5;
    }

    return 0;
}
