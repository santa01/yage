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

#include <CommandParser.h>

bool CommandParser::parse(const std::string& input, std::vector<CommandParser::Token>& output) {
    this->inputSequence.clear();
    this->lastError.str("");

    if (!this->parseLexical(input) || !this->parseSyntax()) {
        return false;
    }

    output.clear();
    for (auto& token: this->inputSequence) {
        // Separators are needed for syntax analysis only
        if (token.type != CommandParser::TokenType::TYPE_SEPARATOR) {
            output.push_back(token);
        }
    }

    return true;
}

CommandParser::SymbolClass CommandParser::getSymbolClass(char symbol) const {
    switch (symbol) {
        case '_':
            return CommandParser::SymbolClass::CLASS_UNDERSCORE;

        case '.':
            return CommandParser::SymbolClass::CLASS_DOT;

        case '"':
            return CommandParser::SymbolClass::CLASS_QUOTE;

        case ';':
            return CommandParser::SymbolClass::CLASS_SEMICOLON;

        case ' ':
        case '\t':
            return CommandParser::SymbolClass::CLASS_SEPARATOR;

        case '+':
        case '-':
            return CommandParser::SymbolClass::CLASS_SIGN;

        default:
            if ((symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z')) {
                return CommandParser::SymbolClass::CLASS_LETTER;
            }

            if (symbol >= '0' && symbol <= '9') {
                return CommandParser::SymbolClass::CLASS_DIGIT;
            }

            if (symbol >= 33 && symbol <= 126) {
                return CommandParser::SymbolClass::CLASS_DIGIT;
            }
    }

    return CommandParser::SymbolClass::CLASS_UNKNOWN;
}

bool CommandParser::parseLexical(const std::string& input) {
    std::string inputLine(input);

    CommandParser::TokenType tokenType = TYPE_UNKNOWN;
    std::stringstream token;
    bool quoteOpened = false;

    for (size_t i = 0; i < inputLine.length(); i++) {
        char symbol = inputLine[i];
        CommandParser::SymbolClass symbolClass = this->getSymbolClass(symbol);

        switch (symbolClass) {
            case CommandParser::SymbolClass::CLASS_SEMICOLON:
            case CommandParser::SymbolClass::CLASS_SEPARATOR:
                if (quoteOpened) {
                    token << symbol;
                    break;
                }

                if (token.str().length() != 0) {
                    this->pushToken(CommandParser::TokenType::TYPE_IDENTIFIER, token.str());
                    token.str("");
                }

                if (symbolClass == CommandParser::SymbolClass::CLASS_SEMICOLON) {
                    return true;
                }

                if (this->inputSequence.size() > 0 &&
                        this->inputSequence.rbegin()->type != CommandParser::TokenType::TYPE_SEPARATOR) {
                    this->pushSpecialToken(CommandParser::TokenType::TYPE_SEPARATOR);
                }

                break;

            case CommandParser::SymbolClass::CLASS_QUOTE:
                if (quoteOpened) {
                    this->pushToken(CommandParser::TokenType::TYPE_LITERAL, token.str());
                    token.str("");
                }

                quoteOpened = !quoteOpened;
                break;

            case CommandParser::SymbolClass::CLASS_LITERAL:
                token << symbol;
                break;

            default:
                this->lastError << "Illegal character `" << symbol << "' at position " << i;
                return false;
        }
    }

    return true;
}

bool CommandParser::parseSyntax() {
    CommandParser::TokenType previousTokenType = CommandParser::TokenType::TYPE_UNKNOWN;

    for (auto& token: this->inputSequence) {
        switch (token.type) {
            case CommandParser::TokenType::TYPE_LITERAL:
            case CommandParser::TokenType::TYPE_DECIMAL:
            case CommandParser::TokenType::TYPE_FLOAT:
                if (previousTokenType != CommandParser::TokenType::TYPE_SEPARATOR) {
                    this->lastError << "Illegal token `" << token.value << "'";
                    return false;
                }

                break;

            case CommandParser::TokenType::TYPE_IDENTIFIER:
                if (previousTokenType != CommandParser::TokenType::TYPE_UNKNOWN ||
                        previousTokenType != CommandParser::TokenType::TYPE_SEPARATOR) {
                    this->lastError << "Illegal token `" << token.value << "'";
                    return false;
                }

                break;

            default:
                break;
        }

        previousTokenType = token.type;
    }

    return true;
}
