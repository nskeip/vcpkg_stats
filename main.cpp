#include <args.hxx>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <json/json.h>
#include <optional>

std::optional<std::string> read_homepage_from_json(const std::filesystem::path &json_path) {
    using namespace std::filesystem;

    if (!exists(json_path)) {
        std::cerr << json_path << "does not exist!" << std::endl;
        return std::nullopt;
    }

    std::fstream ifs(json_path, std::ios::in);

    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    if (!Json::parseFromStream(builder, ifs, &root, &errs)) {
        std::cerr << errs << std::endl;
        return std::nullopt;
    }

    ifs.close();

    const auto homepage = root["homepage"];
    if (!homepage) {
        return std::nullopt;
    } else {
        return homepage.asString();
    }
}

const char *GITHUB_PREFIX = "https://github.com/";
const size_t GITHUB_PREFIX_LEN = strlen(GITHUB_PREFIX);

bool is_github_address(const std::string &url) {
    return url.compare(0, GITHUB_PREFIX_LEN, GITHUB_PREFIX) == 0;
}

std::string github_address_to_rest_api_address(const std::string &homepage) {
    // https://github.com/john/example_project -> https://api.github.com/repos/john/example_project
    return "https://api.github.com/repos/" + homepage.substr(GITHUB_PREFIX_LEN);
}

void process_ports_directory(const std::filesystem::path &ports_path) {
    for (auto const &dir_entry: std::filesystem::directory_iterator{ports_path}) {
        if (!dir_entry.is_directory()) {
            continue;
        }

        const auto dir_path = dir_entry.path();
        const auto project_json_file = dir_path / "vcpkg.json";

        const auto maybe_homepage = read_homepage_from_json(project_json_file);
        if (!maybe_homepage) {
            continue;
        }

        const auto &homepage = maybe_homepage.value();

        if (!is_github_address(homepage)) {
            continue;
        }

        const auto project_name = dir_path.filename().string();
        std::cout << project_name << ": " << github_address_to_rest_api_address(homepage) << '\n';
    }
}

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

    process_ports_directory(ports_path);

    return 0;
}
