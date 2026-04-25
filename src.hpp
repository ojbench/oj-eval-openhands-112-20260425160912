#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <functional>
// WARNING: NO more headers allowed!

using std::pair;
using std::string;
using std::vector;
using std::map;
using std::function;
namespace final {
    class arguments {
    private:
        // WARNING: You cannot add more member variables.
        int _argc;
        char** _argv;
    public:
        arguments() : _argc(0), _argv(nullptr) {}
        explicit arguments(const string& cmd) {
            // Parse the command string to extract arguments
            _argc = 0;
            _argv = nullptr;
            
            if (cmd.empty()) {
                return;
            }
            
            // Count the number of arguments
            int count = 0;
            bool in_arg = false;
            for (size_t i = 0; i < cmd.length(); i++) {
                if (cmd[i] != ' ') {
                    if (!in_arg) {
                        count++;
                        in_arg = true;
                    }
                } else {
                    in_arg = false;
                }
            }
            
            _argc = count;
            
            if (_argc == 0) {
                return;
            }
            
            // Allocate memory for argv array
            _argv = new char*[_argc];
            
            // Extract each argument
            int arg_idx = 0;
            size_t start = 0;
            in_arg = false;
            
            for (size_t i = 0; i <= cmd.length(); i++) {
                if (i < cmd.length() && cmd[i] != ' ') {
                    if (!in_arg) {
                        start = i;
                        in_arg = true;
                    }
                } else {
                    if (in_arg) {
                        // Extract the argument from start to i-1
                        size_t len = i - start;
                        _argv[arg_idx] = new char[len + 1];
                        for (size_t j = 0; j < len; j++) {
                            _argv[arg_idx][j] = cmd[start + j];
                        }
                        _argv[arg_idx][len] = '\0';
                        arg_idx++;
                        in_arg = false;
                    }
                }
            }
        }
        ~arguments() {
            // Free all allocated memory
            if (_argv != nullptr) {
                for (int i = 0; i < _argc; i++) {
                    delete[] _argv[i];
                }
                delete[] _argv;
            }
        }
        // WARNING: You cannot modify the following functions
        int argc() const { return _argc; }
        char** argv() const { return _argv; }
    };

    // You don't need to modify shell.
    class shell {
    private:
        map<int, arguments> running_list;
    public:
        shell() = default;

        void run(int pid, const string& cmd, const function<void(int, char**)>& invoked) {
            running_list.emplace(pid, cmd);
            invoked(running_list[pid].argc(), running_list[pid].argv());
        }

        int subprocessExit(int pid, int return_value) {
            running_list.erase(pid);
            return return_value;
        }
        vector<int> getRunningList() const {
            vector<int> rt;
            for (auto& pair:running_list)rt.push_back(pair.first);
            return rt;
        }
    };
}
