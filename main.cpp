#include <args.hxx>
#include <iostream>

int main(int argc, char **argv) {
    args::ArgumentParser parser("vcpkg repositories stats loader");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::Positional<std::string> vcpkg_root(parser, "vcpkg_root", "where README.md is :)");

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

    if (!vcpkg_root) {
        std::cout << "No vcpkg_root specified, sorry" << std::endl;
        return 3;
    }

    return 0;
}
