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

#ifndef DEVELOPERCONSOLE_H
#define DEVELOPERCONSOLE_H

#include <functional>
#include <utility>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include <Logger.h>

/**
 * \class DeveloperConsole
 * \brief Defines command interface between in-game console and game engine.
 *
 * DeveloperConsole singleton is desined to be used as a command interpreter for in-game console.
 * Any command entered in a game console should be passed to this interpreter for execution.
 * DeveloperConsole add a set of predefined variables and a couple of commands:
 * unbindall               ; resets key bindings
 * echo "<string>"         ; outputs string value to a console, useful for command batches
 * bind "<key>" "<action>" ; defines a key binding
 */
class DeveloperConsole {
public:
    typedef std::function<std::string(const std::vector<std::string>& args)> Callback;

    /**
     * \fn addVariable
     * \brief Insert new variable.
     *
     * DeveloperConsole::addVariable() method inserts new variable that can be later accessed with
     * DeveloperConsole::getVariable(), DeveloperConsole::setVariable() methods or by executing variable's
     * name in a game console. This method is templated with typename T specifies variable type.
     *
     * \param[in] name variable's name.
     * \param[in] defaultValue variable's default value.
     * \param[in] isPrivate defines if variable can be accessed from a game console.
     * \return true if variable successfully inserted, false if variable already exists.
     */
    template<typename T>
    bool addVariable(const std::string& name, const T& defaultValue, bool isPrivate) {
        std::stringstream value;
        value << defaultValue;

        Variable var;
        var.isPrivate = isPrivate;
        value >> var.value;

        if (!this->variables.insert(std::make_pair(name, var)).second) {
            Logger::getInstance().log(Logger::LOG_ERROR, "Variable `%s' already exists", name.c_str());
            return false;
        }

        Logger::getInstance().log(Logger::LOG_INFO, "Added variable %s = %s", name.c_str(), var.value.c_str());
        return true;
    }

    /**
     * \fn addCommand
     * \brief Install new command.
     *
     * DeveloperConsole::addCommand() method installs new command and registers it's callback. Installed commands
     * can later be executed in a game console by printing their names followed by command arguments.
     *
     * \param[in] name command name
     * \param[in] callback command callback. This can be eighter a lambda or a std::function object.
     * \return true if command successfully installed, false if command already exists.
     */
    bool addCommand(const std::string& name, const Callback& callback);

    /**
     * \fn getVariable
     * \brief Get variable value.
     *
     * DeveloperConsole::getVariable() method selects previosly inserted valuable's value or passed default
     * value if variable doesn't exist. This method is templated with typename T specifies variable type.
     *
     * \param[in] name variable's name.
     * \param[in] defaultValue return value if variable doesn't exist.
     * \return variable's value if variable exists, passed defaultValue otherwise.
     */
    template<typename T>
    T getVariable(const std::string& name, const T& defaultValue) {
        auto variable = this->variables.find(name);
        std::stringstream stringValue;

        if (variable != this->variables.end()) {
            stringValue << variable->second.value;
        } else {
            stringValue << defaultValue;
            Logger::getInstance().log(Logger::LOG_WARNING, "No `%s' variable found, defaulting to %s",
                    name.c_str(), stringValue.str().c_str());
        }

        T value;
        stringValue >> value;
        return value;
    }

    /**
     * \fn setVariable
     * \brief Set variable value.
     *
     * DeveloperConsole::setVariable() method modifies previosly inserted valuable's value or does nothing if
     * variable doesn't exist. This method is templated with typename T specifies variable type.
     *
     * \param[in] name variable's name.
     * \param[in] value variable's new value.
     * \return true if variable exists and new values is set, false otherwise.
     */
    template<typename T>
    bool setVariable(const std::string& name, const T& value) {
        auto variable = this->variables.find(name);

        if (variable != this->variables.end()) {
            std::stringstream stringValue;
            stringValue << value;
            variable->second.value = stringValue.str();
            return true;
        }

        Logger::getInstance().log(Logger::LOG_ERROR, "No `%s' variable found", name.c_str());
        return false;
    }

    /**
     * \fn execute
     * \brief Execute console command.
     *
     * DeveloperConsole::execute() method executes string command passed as argument. If command doesn't exist
     * but a respective variable is found than DeveloperConsole::execute() returnes variable's value.
     * This method accepts ; symbol to indicate a comment.
     *
     * \param[in] commandLine command to execute.
     * \return command's output or variable's value.
     */
    std::string execute(const std::string& commandLine);

    /**
     * \fn executeBatch
     * \brief Execute commands batch.
     *
     * DeveloperConsole::executeBatch() method executes commands batch from a file. Batch file can contain
     * one of the registered commands, variable name or ; symbol with randon text afterwards as a comment.
     *
     * \param[in] name path to a batch file.
     * \return command batch output.
     */
    std::string executeBatch(const std::string& name);

private:
    struct Variable {
        std::string value;
        bool isPrivate;     // cannot be updated from game console
    };

    std::map<std::string, Variable> variables;
    std::map<std::string, Callback> commands;

    DeveloperConsole();
    DeveloperConsole(const DeveloperConsole&) = delete;
    DeveloperConsole& operator =(const DeveloperConsole&) = delete;

    void addDefaultVariables();
    void addDefaultCommands();
};

#endif  // DEVELOPERCONSOLE_H
