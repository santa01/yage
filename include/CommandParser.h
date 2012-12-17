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

#ifndef COMMANDPARSER_H
#define	COMMANDPARSER_H

#include <sstream>
#include <string>
#include <vector>

class CommandParser {
public:
    enum TokenType {
        TYPE_UNKNOWN,
        TYPE_IDENTIFIER,    // [_a-zA-Z]+[_-a-zA-Z0-9]*
        TYPE_LITERAL,       // "[\s\t\ ]*"
        TYPE_DECIMAL,       // [\+\-]?\d+
        TYPE_FLOAT,         // [\+\-]?\d+(\.\d+)?
        TYPE_SEPARATOR      // [\t ]+
    };

    struct Token {
        std::string value;
        TokenType type;
    };

    bool parse(const std::string& input, std::vector<std::string>& output);
    
    std::string getErrorMessage() const {
        return this->lastError.str();
    }

private:
    enum SymbolClass {
        CLASS_UNKNOWN,
        CLASS_UNDERSCORE,   // [_]
        CLASS_DOT,          // [.]
        CLASS_QUOTE,        // ["]
        CLASS_SEMICOLON,    // [;]
        CLASS_SEPARATOR,    // [\t ]
        CLASS_SIGN,         // [\+\-]
        CLASS_DIGIT,        // [0-9]
        CLASS_LETTER,       // [a-zA-Z]
        CLASS_SYMBOL        // [\s]
    };

    std::vector<CommandParser::Token> inputSequence;
    std::stringstream lastError;

    void pushToken(CommandParser::TokenType type, const std::string& value) {
        CommandParser::Token token = { value, type };
        this->inputSequence.push_back(token);
    }

    void pushSpecialToken(CommandParser::TokenType type) {
        this->pushToken(type, "");
    }

    CommandParser::SymbolClass getSymbolClass(char symbol) const;
    bool parseLexical(const std::string& input);
    bool parseSyntax();
};

#endif	// COMMANDPARSER_H
