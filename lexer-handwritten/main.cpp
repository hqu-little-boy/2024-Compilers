//
// Created by zpf on 25-3-29.
//
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <format>
#include <unordered_map>
#include <unordered_set>

enum class TokenType
{
    TOKEN_EOF, // EOF 是一个预定义的宏，更换为 TOKEN_EOF
    UNKNOWN,   // 未知，用于错误处理
    // Keywords
    DOT, // 点
    POS, // 正
    NEG, // 负
    ID,  // 标识符
    INT, // 整数
    WS,  // 空白
    // Operators
    EQ, // 等于
    NE, // 不等于
    LT, // 小于
    LE, // 小于等于
    GT, // 大于
    GE, // 大于等于

    REAL, // 实数
    SCI,  // 科学计数法
};

std::pmr::unordered_map<std::string, TokenType> keyWords{
    // {"pos", TokenType::POS},
    // {"neg", TokenType::NEG},
};
std::map<TokenType, std::string> tokenTypeMap{
    {TokenType::TOKEN_EOF, "EOF"},
    {TokenType::UNKNOWN, "UNKNOWN"},
    {TokenType::DOT, "DOT"},
    {TokenType::POS, "POS"},
    {TokenType::NEG, "NEG"},
    {TokenType::ID, "ID"},
    {TokenType::INT, "INT"},
    {TokenType::WS, "WS"},
    {TokenType::EQ, "EQ"},
    {TokenType::NE, "NE"},
    {TokenType::LT, "LT"},
    {TokenType::LE, "LE"},
    {TokenType::GT, "GT"},
    {TokenType::GE, "GE"},
    {TokenType::REAL, "REAL"},
    {TokenType::SCI, "SCI"},
};

class Token
{
public:
    explicit Token(TokenType type)
        : type(type)
    {
    }

    Token(TokenType type, const std::string& value)
        : type(type)
          , value(value)
    {
    }

    [[nodiscard]] std::string ToString() const
    {
        return std::format("Token({0}, {1})", tokenTypeMap[type], value);
    }

    [[nodiscard]] TokenType GetType() const
    {
        return type;
    }

    [[nodiscard]] const std::string& GetValue() const
    {
        return value;
    }

private:
    TokenType   type;
    std::string value;
};

class Character
{
public:
    static bool IsWhitespace(char value)
    {
        return value == ' ' || value == '\t' || value == '\n' || value == '\r';
    }

    static bool IsDigit(char value)
    {
        return value >= '0' && value <= '9';
    }

    static bool IsAlpha(char value)
    {
        return (value >= 'a' && value <= 'z') || (value >= 'A' && value <= 'Z');
    }

    static bool IsAlphaNumeric(char value)
    {
        return IsAlpha(value) || IsDigit(value);
    }

    static bool IsDot(char value)
    {
        return value == '.';
    }

    static bool IsPlus(char value)
    {
        return value == '+';
    }

    static bool IsMinus(char value)
    {
        return value == '-';
    }

    static bool IsExponent(char value)
    {
        return value == 'e' || value == 'E';
    }

    static bool IsEqual(char value)
    {
        return value == '=';
    }

    static bool IsLessThan(char value)
    {
        return value == '<';
    }

    static bool IsGreaterThan(char value)
    {
        return value == '>';
    }

    static bool IsNot(char value)
    {
        return value == '!';
    }

    static bool IsEOF(char value)
    {
        return value == '\0';
    }

    static bool IsNewLine(char value)
    {
        return value == '\n';
    }

    static bool IsCarriageReturn(char value)
    {
        return value == '\r';
    }

    static bool IsTab(char value)
    {
        return value == '\t';
    }

    static bool IsSpace(char value)
    {
        return value == ' ';
    }

    static bool IsComma(char value)
    {
        return value == ',';
    }

    static bool IsSemicolon(char value)
    {
        return value == ';';
    }

    static bool IsColon(char value)
    {
        return value == ':';
    }

    static bool IsLeftParen(char value)
    {
        return value == '(';
    }

    static bool IsRightParen(char value)
    {
        return value == ')';
    }

    static char MinValue()
    {
        return '\0';
    }
};

class Lexer
{
public:
    explicit Lexer(const std::string& text)
        : text(text)
          , pos(0)
          , currentChar(text[pos])
    {
    }

    virtual Token nextToken() = 0;

    void advance()
    {
        ++this->pos;
        if (this->pos < this->text.size())
        {
            this->currentChar = this->text[this->pos];
        }
        else
        {
            this->currentChar = Character::MinValue();
        }
    }

    void reset(int pos)
    {
        this->pos         = pos;
        this->currentChar = this->text[pos];
    }

protected:
    std::string text;
    size_t      pos;
    char        currentChar;
};

class DroganLexer : public Lexer
{
public:
    DroganLexer(const std::string& text)
        : Lexer(text)
          , lastMatchedPos(0)
          , longestValidPrefixPos(0)
          , longestValidPrefixTokenType(TokenType::UNKNOWN)
    {
    }

    Token nextToken() override
    {
        if (this->pos >= this->text.size())
        {
            return Token(TokenType::TOKEN_EOF);
        }

        auto token{Token(TokenType::UNKNOWN)};
        if (Character::IsWhitespace(this->currentChar))
        {
            token = this->WS();
        }
        else if (Character::IsAlpha(this->currentChar))
        {
            token = this->ID();
        }
        else if (Character::IsDigit(this->currentChar))
        {
            token = this->NUMBER();
        }
        else if (Character::IsEqual(this->currentChar))
        {
            token = {TokenType::EQ, "="};
            this->advance();
        }
        else if (Character::IsGreaterThan(this->currentChar))
        {
            this->advance();
            if (Character::IsEqual(this->text[this->pos]))
            {
                token = {TokenType::GE, ">="};
                this->advance();
            }
            else
            {
                token = {TokenType::GT, ">"};
            }
        }
        else if (Character::IsLessThan(this->currentChar))
        {
            this->advance();
            if (Character::IsEqual(this->text[this->pos]))
            {
                token = {TokenType::LE, "<="};
                this->advance();
            }
            else
            {
                token = {TokenType::LT, "<"};
            }
        }
        else if (Character::IsDot(this->currentChar))
        {
            token = {TokenType::DOT, "."};
            this->advance();
        }
        else if (Character::IsPlus(this->currentChar))
        {
            token = {TokenType::POS, "+"};
            this->advance();
        }
        else if (Character::IsMinus(this->currentChar))
        {
            token = {TokenType::NEG, "-"};
            this->advance();
        }
        else
        {
            token = {TokenType::UNKNOWN, std::string(1, this->currentChar)};
            this->advance();
        }
        this->lastMatchedPos = this->pos;
        return token;
    }

private:
    Token WS()
    {
        while (Character::IsWhitespace(this->currentChar))
        {
            this->advance();
        }
        return Token(TokenType::WS);
    }

    Token ID()
    {
        std::string result;
        do
        {
            result += this->currentChar;
            this->advance();
        } while (Character::IsAlphaNumeric(this->currentChar));
        if (keyWords.contains(result))
        {
            return {keyWords[result], result};
        }
        return {TokenType::ID, result};
    }

    Token NUMBER()
    {
        int state{13};
        // std::string result;
        this->advance();
        // Token token{TokenType::UNKNOWN};
        for (;;)
        {
            switch (state)
            {
            case 13:
            {
                this->longestValidPrefixPos       = this->pos;
                this->longestValidPrefixTokenType = TokenType::INT;
                if (Character::IsDigit(this->currentChar))
                {
                    this->advance();
                    state = 13;
                }
                else if (Character::IsDot(this->currentChar))
                {
                    this->advance();
                    state = 14;
                }
                else if (Character::IsExponent(this->currentChar))
                {
                    this->advance();
                    state = 16;
                }
                else
                {
                    return this->BackToLastMatchedPos();
                }
                break;
            }
            case 14:
            {
                if (Character::IsDigit(this->currentChar))
                {
                    this->advance();
                    state = 15;
                }
                else
                {
                    return this->BackToLastMatchedPos();
                }
                break;
            }
            case 15:
            {
                this->longestValidPrefixPos       = this->pos;
                this->longestValidPrefixTokenType = TokenType::REAL;

                if (Character::IsDigit(this->currentChar))
                {
                    this->advance();
                    state = 15;
                }
                else if (Character::IsExponent(this->currentChar))
                {
                    this->advance();
                    state = 16;
                }
                else
                {
                    return this->BackToLastMatchedPos();
                }
                break;
            }
            case 16:
            {
                if (Character::IsPlus(this->currentChar) || Character::IsMinus(this->currentChar))
                {
                    this->advance();
                    state = 17;
                }
                else if (Character::IsDigit(this->currentChar))
                {
                    this->advance();
                    state = 18;
                }
                else
                {
                    return this->BackToLastMatchedPos();
                    // state = 20;
                }
                break;
            }
            case 17:
            {
                if (Character::IsDigit(this->currentChar))
                {
                    this->advance();
                    state = 18;
                }
                else
                {
                    return this->BackToLastMatchedPos();
                }
                break;
            }
            case 18:
            {
                this->longestValidPrefixPos       = this->pos;
                this->longestValidPrefixTokenType = TokenType::SCI;
                if (Character::IsDigit(this->currentChar))
                {
                    this->advance();
                }
                else
                {
                    return this->BackToLastMatchedPos();
                }
                break;
            }
            default:
            {
                throw std::runtime_error("Invalid state");
            }
            }
        }
    }

    Token INT()
    {
        std::string result;
        do
        {
            result += this->currentChar;
            this->advance();
        } while (Character::IsDigit(this->currentChar));
        return {TokenType::INT, result};
    }

    Token BackToLastMatchedPos()
    {
        Token token{this->longestValidPrefixTokenType,
                    this->text.substr(this->lastMatchedPos,
                                      this->longestValidPrefixPos - this->lastMatchedPos)};
        if (this->longestValidPrefixPos < this->text.size())
        {
            this->reset(this->longestValidPrefixPos);
        }
        return token;
    }

private:
    size_t    lastMatchedPos;
    size_t    longestValidPrefixPos;
    TokenType longestValidPrefixTokenType;
};

int main()
{
    std::string   testFilePath{"/home/zpf/git/2024-Compilers/lexer-handwritten/txt/dragon0.txt"};
    std::ifstream file(testFilePath);
    if (!file.is_open())
    {
        std::cerr << "Could not open file: " << testFilePath << std::endl;
        return 1;
    }
    std::string text;
    std::string line;
    while (std::getline(file, line))
    {
        text += line + '\n';
    }
    file.close();

    auto lexer{DroganLexer(text)};
    // auto lexer{DroganLexer("12.34")};
    auto token{lexer.nextToken()};
    while (token.GetType() != TokenType::TOKEN_EOF)
    {
        if (token.GetType() != TokenType::WS)
        {
            std::cout << token.ToString() << std::endl;
        }
        token = lexer.nextToken();
    }
    return 0;
}
