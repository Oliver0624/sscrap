//
// Created by cpasjuste on 05/04/19.
//
#include <fstream>
#include <cstdarg>
#include <zlib.h>
#include "ss_api.h"

using namespace ss_api;

std::string Api::ss_devid;
std::string Api::ss_devpassword;
std::string Api::ss_softname;
bool ss_debug = false;

std::string Api::getXmlAttrStr(tinyxml2::XMLElement *element, const std::string &name, const std::string &defaultValue) {
    if (element == nullptr || element->Attribute(name.c_str()) == nullptr) {
        return defaultValue;
    }

    return element->Attribute(name.c_str());
}

int Api::getXmlAttrInt(tinyxml2::XMLElement *element, const std::string &name) {
    std::string attr = getXmlAttrStr(element, name);
    return parseInt(attr);
}

long Api::getXmlAttrLong(tinyxml2::XMLElement *element, const std::string &name) {
    std::string attr = getXmlAttrStr(element, name);
    return parseLong(attr);
}

unsigned long Api::getXmlAttrULong(tinyxml2::XMLElement *element, const std::string &name) {
    std::string attr = getXmlAttrStr(element, name);
    return parseULong(attr);
}

bool Api::getXmlAttrBool(tinyxml2::XMLElement *element, const std::string &name) {
    std::string attr = getXmlAttrStr(element, name);
    return parseBool(attr);
}

std::string Api::getXmlTextStr(tinyxml2::XMLElement *element) {
    if (element == nullptr || element->GetText() == nullptr) {
        return "";
    }

    return element->GetText();
}

int Api::getXmlTextInt(tinyxml2::XMLElement *element) {
    std::string text = getXmlTextStr(element);
    return parseInt(text);
}

float Api::getXmlTextFloat(tinyxml2::XMLElement *element) {
    std::string text = getXmlTextStr(element);
    return parseFloat(text);
}

bool Api::getXmlTextBool(tinyxml2::XMLElement *element) {
    std::string text = getXmlTextStr(element);
    return parseBool(text);
}

tinyxml2::XMLElement *Api::addXmlElement(tinyxml2::XMLDocument *doc, tinyxml2::XMLElement *parent,
                                         const std::string &name, const std::string &value) {
    if (parent == nullptr) {
        return nullptr;
    }

    if (!value.empty()) {
        tinyxml2::XMLElement *element = doc->NewElement(name.c_str());
        element->SetText(value.c_str());
        parent->InsertEndChild(element);
        return element;
    }

    return nullptr;
}

bool Api::sortByName(const std::string &g1, const std::string &g2) {
#ifdef _MSC_VER
    return _stricmp(g1.c_str(), g2.c_str()) < 0;
#else
    return strcasecmp(g1.c_str(), g2.c_str()) < 0;
#endif
}

bool Api::sortInteger(int i1, int i2) {
    return i1 < i2;
}

bool Api::sortGameByName(const Game &g1, const Game &g2) {
    const std::string lhs = g1.name;
    const std::string rhs = g2.name;
    const auto result = mismatch(
            lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend(), [](const auto &lhs, const auto &rhs) {
                return tolower(lhs) == tolower(rhs);
            });
    return result.second != rhs.cend() &&
           (result.first == lhs.cend() || tolower(*result.first) < tolower(*result.second));
}

bool Api::sortGameByPath(const Game &g1, const Game &g2) {
    const std::string lhs = g1.path;
    const std::string rhs = g2.path;
    const auto result = mismatch(
            lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend(), [](const auto &lhs, const auto &rhs) {
                return tolower(lhs) == tolower(rhs);
            });
    return result.second != rhs.cend() &&
           (result.first == lhs.cend() || tolower(*result.first) < tolower(*result.second));
}

bool Api::sortSystemByName(const System &s1, const System &s2) {
    const std::string lhs = s1.name;
    const std::string rhs = s2.name;
    const auto result = mismatch(
            lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend(), [](const auto &lhs, const auto &rhs) {
                return tolower(lhs) == tolower(rhs);
            });
    return result.second != rhs.cend() &&
           (result.first == lhs.cend() || tolower(*result.first) < tolower(*result.second));
}

bool Api::sortEditorByName(const Game::Editor &e1, const Game::Editor &e2) {
    const std::string lhs = e1.name;
    const std::string rhs = e2.name;
    const auto result = mismatch(
            lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend(), [](const auto &lhs, const auto &rhs) {
                return tolower(lhs) == tolower(rhs);
            });
    return result.second != rhs.cend() &&
           (result.first == lhs.cend() || tolower(*result.first) < tolower(*result.second));
}

bool Api::sortDeveloperByName(const Game::Developer &d1, const Game::Developer &d2) {
    const std::string lhs = d1.name;
    const std::string rhs = d2.name;
    const auto result = mismatch(
            lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend(), [](const auto &lhs, const auto &rhs) {
                return tolower(lhs) == tolower(rhs);
            });
    return result.second != rhs.cend() &&
           (result.first == lhs.cend() || tolower(*result.first) < tolower(*result.second));
}

bool Api::sortGenreByName(const Game::Genre &g1, const Game::Genre &g2) {
    const std::string lhs = g1.name;
    const std::string rhs = g2.name;
    const auto result = mismatch(
            lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend(), [](const auto &lhs, const auto &rhs) {
                return tolower(lhs) == tolower(rhs);
            });
    return result.second != rhs.cend() &&
           (result.first == lhs.cend() || tolower(*result.first) < tolower(*result.second));
}

int Api::parseInt(const std::string &str, int defValue) {
    char *end = nullptr;
    long i = strtol(str.c_str(), &end, 10);
    if (end != nullptr && end != str.c_str()) {
        return (int) i;
    }
    return defValue;
}

long Api::parseLong(const std::string &str, long defValue) {
    char *end = nullptr;
    long i = strtol(str.c_str(), &end, 10);
    if (end != nullptr && end != str.c_str()) {
        return i;
    }
    return defValue;
}

unsigned long Api::parseULong(const std::string &str, unsigned long defValue) {
    char *end = nullptr;
    unsigned long i = strtoll(str.c_str(), &end, 10);
    if (end != nullptr && end != str.c_str()) {
        return i;
    }
    return defValue;
}

float Api::parseFloat(const std::string &str, float defValue) {
    char *end = nullptr;
    float f = strtof(str.c_str(), &end);
    if (end != nullptr && end != str.c_str()) {
        return f;
    }
    return defValue;
}

bool Api::parseBool(const std::string &str, bool defValue) {
    if (str.empty() || str != "true" || str != "false" || str != "0" || str != "1") {
        return defValue;
    }

    return str == "true" || str == "1";
}

std::string Api::getFileCrc(const std::string &zipPath) {
    unsigned char buffer[BUFSIZ];
    char hex[16];
    size_t size;
    FILE *pFile;

    memset(hex, 0, 16);

#ifdef _MSC_VER
    fopen_s(&pFile, zipPath.c_str(), "rb");
#else
    pFile = fopen(zipPath.c_str(), "rb");
#endif
    if (!pFile) {
        return hex;
    }

    uLong crc = crc32(0L, Z_NULL, 0);
    while ((size = fread(buffer, 1, BUFSIZ, pFile)) != 0) {
        crc = crc32(crc, buffer, size);
    }
    snprintf(hex, 16, "%08lx", crc);

    fclose(pFile);

    return hex;
}

#ifdef __WINDOWS__

void Api::printc(int color, const char *format, ...) {
#else
void Api::printc(const char *color, const char *format, ...) {
#endif

    char buffer[1024];
    va_list arg;
    va_start(arg, format);
    vsnprintf(buffer, 1024, format, arg);
    va_end(arg);

#ifdef __WINDOWS__
    WORD consoleAttr = FOREGROUND_INTENSITY | FOREGROUND_RED
                       | FOREGROUND_GREEN | FOREGROUND_BLUE;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(h, &csbi)) {
        consoleAttr = csbi.wAttributes;
    }
    SetConsoleTextAttribute(h, color);
    printf("%s", buffer);
    SetConsoleTextAttribute(h, consoleAttr);
#else
    printf("%s%s" "\033[0m", color, buffer);
#endif
}

void Api::printe(int code, int delay) {
    if (code == 429) {
        printc(COLOR_O, "NOK: maximum requests per minute reached... retrying in %i seconds\n", delay);
    } else if (code == 28) {
        printc(COLOR_O, "NOK: timeout reached... retrying in %i seconds\n", delay);
    }
}


int Api::getCharacterBytes(unsigned char firstByte)
{
    if (firstByte <= 0x7F) {
        return 1;
    } else if ((firstByte & 0xE0) == 0xC0) {
        return 2;
    } else if ((firstByte & 0xF0) == 0xE0) {
        return 3;
    } else if ((firstByte & 0xF8) == 0xF0) {
        return 4;
    } else if ((firstByte & 0xFC) == 0xF8) {
        return 5;
    } else if ((firstByte & 0xFE) == 0xFC) {
        return 6;
    }
    return 0;
}

uint32_t Api::GetNextUnicode(const unsigned char *&str) {
    const unsigned char utf8_first_mask[] = { 0x7F, 0x1F, 0x0F, 0x07, 0x03, 0x01 };

    int len = Api::getCharacterBytes(*str);
    if (len <= 0) {
        str += 1;  // skip this byte
        return 0;
    }

    uint32_t code = 0;
    for (int i = 0; i < len; i++) {
        if (*str == 0) {
            str += (i + 1); // skip to next possible position
            return 0;
        }

        code <<= 6;
        if (i == 0) {
            code |= (*str & utf8_first_mask[len-1]);
        } else {
            code |= (*str & 0x3F);
        }        
        str++;
    }

    return code;
}

std::vector<uint16_t> Api::ss_vecCnSeqUnicode;

bool Api::loadPronCN(const std::string &filepath) {
    std::ifstream file;
    file.open(filepath.c_str(), std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return -1;
    }

    // get file size
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // read data to vector
    ss_vecCnSeqUnicode.resize(fileSize / sizeof(uint16_t));
    if (!file.read(reinterpret_cast<char*>(ss_vecCnSeqUnicode.data()), fileSize)) {
        return 1;
    }

    file.close();
    return 0;
}

// Unicode Chinese character start from 0x4E00
#define CN_UNICODE_START           0x4E00

// Unicode 0x3007 in Chinese is special
#define CN_UNICODE_0X_3007_CODE    0x3007
#define CN_UNICODE_0X_3007_SEQ       569

bool Api::sortGameByNameUnicode(const Game &g1, const Game &g2) {
    auto lhs = reinterpret_cast<const unsigned char*>(g1.name.c_str());
    auto rhs = reinterpret_cast<const unsigned char*>(g2.name.c_str());
    auto lhs_end = lhs + g1.name.size();
    auto rhs_end = rhs + g2.name.size();
    bool first_byte = true;

    do {
        if (*lhs != 0 && *rhs != 0) {
            // always sort to last if first byte is '['
            //if (0 == llen && 0 == rlen)
            if (first_byte)
            {
                if (*lhs == '[' && *rhs != '[') {
                    return false;
                } else if (*lhs != '[' && *rhs == '[') {
                    return true;
                }
                first_byte = false;
            }

            const auto lchar = Api::GetNextUnicode(lhs);
            const auto rchar = Api::GetNextUnicode(rhs);

            if (lchar != 0 && rchar != 0) {
                if (lchar <= 0x7F && rchar <= 0x7F) { // compare by latin
                    if (tolower(lchar) < tolower(rchar)) {
                        return true;
                    } else if (tolower(lchar) > tolower(rchar)) {
                        return false;
                    }
                } else if (lchar <= 0x7F && rchar > 0x7F) { // latin always in front of Chinese
                    return true;
                } else if (lchar > 0x7F && rchar <= 0x7F) {
                    return false;
                } else { // lchar > 0x7F && rchar > 0x7F   // try to compare by Chinese
                    uint16_t lseq = lchar;
                    uint16_t rseq = rchar;

                    if (lchar >= CN_UNICODE_START && lchar - CN_UNICODE_START < ss_vecCnSeqUnicode.size()) {
                        lseq = ss_vecCnSeqUnicode[lchar - CN_UNICODE_START];
                    } else if (CN_UNICODE_0X_3007_CODE == lchar) {
                        lseq = CN_UNICODE_0X_3007_SEQ;
                    }

                    if (rchar >= CN_UNICODE_START && rchar - CN_UNICODE_START < ss_vecCnSeqUnicode.size()) {
                        rseq = ss_vecCnSeqUnicode[rchar - CN_UNICODE_START];
                    } else if (CN_UNICODE_0X_3007_CODE == rchar) {
                        rseq = CN_UNICODE_0X_3007_SEQ;
                    }

                    return (lseq < rseq);
                }
            } else if (lchar != 0 && rchar == 0) { // legal always in front of illegal
                return true;
            } else if (lchar == 0 && rchar != 0) {
                return false;
            } else { // all illegal, dont swap
                return true;
            }
        } else if (*lhs == 0 && *rhs != 0) { // short always in front of long
            return true;
        } else if (*lhs != 0 && *rhs == 0) {
            return false;
        } else { // *lhs == 0 && *rhs == 0  // string is equal
            return true;
        }
    } while (lhs < lhs_end && rhs < rhs_end);

    return true;
}