#include <iostream>
#include <fstream>
#include <filesystem>
#include <random>
#include <string>

using namespace std;
namespace fs = std::filesystem;

string random_content(size_t length = 55) {
    static const string chars = "abcdefghijklmnopqrstuvwxyz0123456789";
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dis(0, chars.size() - 1);
    string result;

    for (size_t i = 0; i < length; i++)
        result += chars[dis(gen)];
    return result;
}

int main() {
    try {
        string base_dir_name;
        cout << "Enter the directory name - ";
        getline(cin, base_dir_name);

        fs::path base_dir = fs::current_path() / base_dir_name;
        fs::path hard_link = base_dir / "hard_links";
        fs::path sym_link = base_dir / "sym_links";

        fs::create_directories(hard_link);
        fs::create_directories(sym_link);

        const int file_count = 5;
        vector<fs::path> created_files;

        for (int i = 1; i <= file_count; i++) {
            fs::path file_path = base_dir / ("file" + to_string(i) + ".txt");
            ofstream out(file_path);
            if (!out)
                throw ios_base::failure("Failed to create file: " + file_path.string());
            out << random_content(110); 
            out.close();
            created_files.push_back(file_path);
        }

        for (const auto& file : created_files) {
            fs::path hard_path = hard_link / file.filename();
            fs::create_hard_link(file, hard_path);
        }

        for (const auto& file : created_files) {
            fs::path sym_path = sym_link / file.filename();
            fs::create_symlink(file, sym_path);
        }

        cout << "\nInformation:\n";
        for (const auto& file : created_files) {
            cout << "File: " << file << "\n";
            cout << "-Hard link: " << (hard_link / file.filename()) << "\n";
            cout << "-Symbolic link: " << (sym_link / file.filename()) << "\n\n";
        }
        cout << "All operations completed!\n";
    }
    catch (const fs::filesystem_error& e) {
        cerr << "Filesystem error: " << e.what() << '\n';
    }
    catch (const exception& e) {
        cerr << "Exception: " << e.what() << '\n';
    }
}
