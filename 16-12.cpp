#include <iostream>
#include <filesystem>
#include <string>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

int main() {
    try {
        string dir_path;
        cout << "Enter the directory path: ";
        getline(cin, dir_path);

        fs::path target_dir = dir_path;

        if (!fs::exists(target_dir) || !fs::is_directory(target_dir)) {
            cerr << "The path does not exist...\n";
            return 1;
        }

        auto now = chrono::system_clock::now();
        auto max_age = chrono::hours(24 * 30);//30days
        cout << "\nDeleted files:\n";

        int deleted_count = 0;

        for (const auto& entry : fs::directory_iterator(target_dir)) {
            if (fs::is_regular_file(entry)) {
                auto ftime = fs::last_write_time(entry);
                auto sctp = chrono::time_point_cast<chrono::system_clock::duration>(
                    ftime - fs::file_time_type::clock::now() + chrono::system_clock::now());

                auto age = now - sctp;

                if (age > max_age) {
                    cout << " Deleted: " << entry.path() << "\n";
                    fs::remove(entry);
                    ++deleted_count;
                }
            }
        }

        if (deleted_count == 0)
            cout << "  All files were recently modified\n";
        else
            cout << "\nTotal files deleted: " << deleted_count << "\n";

    }
    catch (const fs::filesystem_error& e) {
        cerr << "Filesystem error: " << e.what() << '\n';
    }
    catch (const exception& e) {
        cerr << "Exception: " << e.what() << '\n';
    }
}
