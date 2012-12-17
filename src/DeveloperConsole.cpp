/*
 * Copyright (c) 2012 Pavlo Lavrenenko, Kirill Bogdanenko
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <algorithm>
#include <iterator>
#include <fstream>
#include <vector>

#include <DeveloperConsole.h>
#include <CommandParser.h>

DeveloperConsole::DeveloperConsole() {
    this->addDefaultVariables();
    this->addDefaultCommands();
}

bool DeveloperConsole::addCommand(const std::string& name, const Callback& callback) {
    if (!this->commands.insert(std::make_pair(name, callback)).second) {
        Logger::getInstance().log(Logger::LOG_ERROR, "Command `%s' already exists", name.c_str());
        return false;
    }

    Logger::getInstance().log(Logger::LOG_INFO, "Added command `%s'", name.c_str());
    return true;
}

std::string DeveloperConsole::execute(const std::string& commandLine) {
    CommandParser parser;
    std::vector<std::string> tokens;

    if (parser.parse(commandLine, tokens)) {
        return parser.getErrorMessage();
    }

    std::string commandName = tokens[0];
    tokens.erase(tokens.begin());

    std::stringstream output;
    auto command = this->commands.find(commandName);

    if (command == this->commands.end()) {
        auto variable = this->variables.find(commandName);

        if (variable == this->variables.end() || variable->second.isPrivate) {
            output << "Unknown command or variable name `" << commandName << "'";
        } else {
            if (tokens.empty()) {
                // No more arguments left, read variable value
                output << variable->second.value;
            } else if (tokens.size() == 1) {
                // One token left, set variable's value
                variable->second.value = tokens[0];
            } else {
                // But wait, there is more!
                output << "Multiple tokens found, but `" << commandName << "' id not a command";
            }
        }

        return output.str();
    }

    return command->second(tokens);
}

std::string DeveloperConsole::executeBatch(const std::string& name) {
    std::fstream file(name.c_str(), std::ios::in);
    if (!file.good()) {
        Logger::getInstance().log(Logger::LOG_ERROR, "Cannot open file %s", name.c_str());
        return "";
    }

    std::stringstream output;
    std::string batchLine;
    while (!file.eof()) {
        std::getline(file, batchLine);
        output << this->execute(batchLine) << '\n';
    }

    file.close();
    output.get();  // remove trailing newline
    return output.str();
}

// gl_ prefix for graphics variables
// bind_ prefix for control bindings
// pl_ prefix for player specific variables
// en_ prefix for engine specific variables
void DeveloperConsole::addDefaultVariables() {
    this->addVariable<int>("gl_width", 800, false);
    this->addVariable<int>("gl_height", 600, false);
    this->addVariable<int>("gl_fov", 70, false);
    this->addVariable<int>("gl_vsync", 0, false);
    this->addVariable<int>("gl_msaa", 0, false);
    this->addVariable<int>("gl_anisotropy", 0, false);
    this->addVariable<bool>("gl_fullscreen", false, false);

    // TODO these should be keys with respective translation to keycodes
    this->addVariable<char>("bind_forward", 0, true);
    this->addVariable<char>("bind_backward", 0, true);
    this->addVariable<char>("bind_stepleft", 0, true);
    this->addVariable<char>("bind_stepright", 0, true);
    this->addVariable<char>("bind_crouch", 0, true);
    this->addVariable<char>("bind_jump", 0, true);
    this->addVariable<char>("bind_exit", 0, true);

    this->addVariable<float>("pl_forwardspeed", 0.0f, false);
    this->addVariable<float>("pl_backwardspeed", 0.0f, false);
    this->addVariable<float>("pl_stepspeed", 0.0f, false);
    this->addVariable<float>("pl_mousespeed", 0.0f, false);

    this->addVariable<int>("en_verbose", 0, false);
}

void DeveloperConsole::addDefaultCommands() {
    // unbindall
    this->addCommand("unbindall", [this] (const std::vector<std::string>& args) -> std::string {
        if (!args.empty()) {
            return "usage: unbindall";
        }

        for(auto &variable: this->variables) {
            if (variable.first.find("bind_")) {
                this->setVariable<char>(variable.first, 0);
            }
        }

        return "";
    });

    // echo "<string>"
    this->addCommand("echo", [this] (const std::vector<std::string>& args) -> std::string {
        if (args.size() != 1) {
            return "usage: bind \"<string>\"";
        }

        return args[0];
    });

    // bind "<key>" "<action>"
    this->addCommand("bind", [this] (const std::vector<std::string>& args) -> std::string {
        if (args.size() != 2) {
            return "usage: bind \"<action>\" \"<key>\"";
        }

        char key;
        std::stringstream argument;
        argument.str(args[1]);
        argument >> key;

        this->setVariable<char>("bind_" + args[0], key);
        return "";
    });
}
