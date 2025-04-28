#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
using namespace std;

namespace fs = filesystem;


struct Line{
    int number = 0;
    string content = "";
};

unordered_map<string, vector<Line>> files;
unordered_set<string> options;
string pattern;


bool isLetter(char c){
    if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return true;
    return false;
}
void getLines(const fs::path& filePath, const string& pathName) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return;
    }
    string line;
    int lineNumber = 1;
    while (getline(file, line)) {
        files[pathName + "/" + filePath.filename().string()].push_back({lineNumber, line});
        ++lineNumber;
    }
    file.close();
}


void search(){
    for(const auto& entry : files){
        bool isInverted = (options.find("-v") != options.end());
        const vector<Line>& lines = entry.second;
        for(const Line& line : lines){
            if((line.content.find(pattern) != string::npos) != isInverted){
                cout << entry.first << ":";
                if(!options.empty() && options.find("-n") != options.end())
                    cout << line.number << ":";
                cout << line.content << "\n";
            }
        }
    }
}
void traverseDirectory(const fs::path& directoryPath, const fs::path& pathSoFar) {
    try {
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (entry.is_directory()) {
                traverseDirectory(entry.path(), pathSoFar / entry.path());
            } else if (entry.is_regular_file()) {
                string ext = entry.path().extension().string();
                if (ext == ".txt") {
                    getLines(entry.path(), pathSoFar.string());
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Filesystem error: " << e.what() << endl;
    } catch (const exception& e) {
        cerr << "General error: " << e.what() << endl;
    }
}

int main(int argc, char* argv[]) {
    fs::path currentPath;
    fs::path emptyPath;

    string help_option = argv[1];
    if(argc < 3 && help_option != "--help")
        throw invalid_argument("Usage: grep [OPTION]... PATTERNS [FILE]...");
    unordered_set<string> valid_options{
        "-v",
        "-n",
        "-r",
        "-i"
    };

    for(int i = 1 ; i < argc ; ++i){
        string argument = argv[i];

        
        if(argument == "--help"){
            cout << R"(Usage: grep [OPTION]... PATTERNS [FILE]...
Search for PATTERNS in each FILE.
Example: grep -i 'hello world' menu.h main.c
PATTERNS can contain multiple patterns separated by newlines

Pattern selection and interpretation:
  -i,        ignore case distinctions in patterns and data
Miscellaneous:
  -v,        select non-matching lines
  --help     display this help text and exit
Output control:
  -n,        print line number with output lines
  -r,        like --directories=recurse
  .)";
            return 0;
        }


        if(argument[0] == '-'){
            if((int)argument.size() == 1){
                throw invalid_argument("missing option option -- (" + argument + 
                ")\nUsage: grep [OPTION]... PATTERNS [FILE]...");
            }
            if((int)argument.size() == 2){
                if(valid_options.find(argument) == valid_options.end()){
                    throw invalid_argument("invalid option -- (" + argument + 
                            ")\nUsage: grep [OPTION]... PATTERNS [FILE]...");
                }
            }

            if((int)argument.size() > 2){
                for(int c = 1 ; c < (int)argument.size();  ++c){
                    string current_option = string("-") + argument[c];
                    if(!isLetter(argument[c]) || (valid_options.find(current_option) == valid_options.end())){
                        throw invalid_argument("invalid option -- (" + argument + 
                        ")\nUsage: grep [OPTION]... PATTERNS [FILE]...");
                    }
                    options.insert(current_option);
                }
            }
            else
                options.insert(argument);
        }
        else{
            if(!pattern.empty() && !currentPath.empty()){
                throw invalid_argument("invalid option -- (" + argument + 
                ")\nUsage: grep [OPTION]... PATTERNS [FILE]...");
            }
            if(pattern.empty()){
                pattern = argument;
            }
            else{
                currentPath = argument;
            }
        }
    }

    if(currentPath.empty())
        currentPath = fs::current_path();
    if(options.find("-r") == options.end() && fs::is_directory(currentPath))
        throw invalid_argument(currentPath.string() + " is a directory");
    
    if (!fs::exists(currentPath)) {
        cerr << "Path does not exist: " << currentPath << endl;
        return 1;
    }

    if(fs::is_regular_file(currentPath)){
        getLines(currentPath, emptyPath);
        search();
    }
    else{
        traverseDirectory(currentPath, emptyPath);
        search();
    }

    return 0;
}