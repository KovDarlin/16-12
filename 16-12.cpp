#include <iostream>
#include <filesystem>
#include <set>
#include <string>

using namespace std;
namespace fs = std::filesystem;

int main() {
    try {
        string dir1_path, dir2_path;
        cout << "Enter the path to the 1 directory: ";
        getline(cin, dir1_path);
        cout << "Enter the path to the 2 directory: ";
        getline(cin, dir2_path);

        fs::path dir1 = dir1_path;
        fs::path dir2 = dir2_path;

        if (!fs::exists(dir1) || !fs::is_directory(dir1)) {
            cerr << "Directory 1 does not exist.\n";
            return 1;
        }
        if (!fs::exists(dir2) || !fs::is_directory(dir2)) {
            cerr << "Directory 2 does not exist.\n";
            return 1;
        }

        set<string> files1, files2;

        for (const auto& entry : fs::directory_iterator(dir1)) {
            if (fs::is_regular_file(entry))
                files1.insert(entry.path().filename().string());
        }

        for (const auto& entry : fs::directory_iterator(dir2)) {
            if (fs::is_regular_file(entry))
                files2.insert(entry.path().filename().string());
        }

        cout << "\nFiles that are only in directory 1:\n";
        bool found1 = false;
        for (const auto& f : files1) {
            if (!files2.count(f)) {
                cout << "  " << f << "\n";
                found1 = true;
            }
        }
        if (!found1) cout << "  - equally\n";

        cout << "\nFiles that are only in directory 2:\n";
        bool found2 = false;
        for (const auto& f : files2) {
            if (!files1.count(f)) {
                cout << "  " << f << "\n";
                found2 = true;
            }
        }
        if (!found2) cout << "  - equally\n";

    }
    catch (const fs::filesystem_error& e) {
        cerr << "Filesystem error: " << e.what() << '\n';
    }
    catch (const exception& e) {
        cerr << "Exception: " << e.what() << '\n';
    }
}
