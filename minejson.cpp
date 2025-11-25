//
// Created by Micael Cossa on 29/07/2025.
//

#include "minejson.h"
#include <stack>
#include <limits>

constexpr std::string_view::size_type ERROR_VALUE = std::string_view::npos;

std::string_view::size_type getKeyName(const std::string_view stringView, std::string_view::size_type pos, std::string& out) {

    if(stringView[pos] != '"')
        return ERROR_VALUE;


    std::string_view::size_type quotationEndPos = pos + 1;
    std::string name;

    while(quotationEndPos < stringView.size()) {

        if(stringView[quotationEndPos] == '\\') {
            quotationEndPos += 2;
            continue;
        }

        if(stringView[quotationEndPos] == '"')
            break;

        name += stringView[quotationEndPos];
        quotationEndPos++;
    }

    // The only reason this would be true is if the while loop stop due to quotationEndPos reaching the same length as the json string.
    if(quotationEndPos >= stringView.size())
        return ERROR_VALUE;


    out = std::move(name);

    return quotationEndPos;
}

// Pretty much just checking if there is a ':'
std::string_view::size_type hasValueSignate(std::string_view stringView, std::string_view::size_type pos) {

    pos = stringView.find_first_not_of(" \t\n\r", pos);

    if(pos == ERROR_VALUE)
        return pos;

    return stringView[pos] == ':' ? pos : ERROR_VALUE;
}


std::string_view::size_type validateCommaAndSkip(size_t listsize, std::string_view stringView, std::string_view::size_type pos) {
    if(pos != ERROR_VALUE && stringView[pos] == ',') {
        if (listsize == 0 || pos + 1 >= stringView.length())  // why is there a comma if there are no values on the array?
            return ERROR_VALUE;

        // skip the whitespace after ','
        pos = stringView.find_first_not_of(" \t\n\r", pos + 1);

    } else if(listsize > 0) // There must be a ',' after a value is entered on the array.
        return ERROR_VALUE;

    return pos;
}


std::string_view::size_type startObjectProccessing(json_object* root , std::string_view stringView, std::string_view::size_type pos) {


    while (stringView.length() > pos) {


        pos = stringView.find_first_not_of(" \t\n\r", pos);

        if(pos == ERROR_VALUE || stringView[pos] == '}')
            break;


        pos = validateCommaAndSkip(root->size(), stringView, pos);

        if(pos == ERROR_VALUE)
            goto error;


        std::string name;

        // gets text between two quotation marks, returns the '"' end char pos
        pos = getKeyName(stringView, pos, name);

        // did an error happening while trying to process the key?
        if(pos == ERROR_VALUE)
            goto error;


        // checks if ':' exists and returns the position of the : char
        pos = hasValueSignate(stringView, pos + 1);

        if(pos == ERROR_VALUE)
            goto error;


        json_node* value = nullptr;

        // skip the whitespace after, returns the position of the character after the value identified
        pos = valueIdentification(stringView, stringView.find_first_not_of(" \t\n\r", pos+1), value);

        if(pos == ERROR_VALUE || value == nullptr)
            goto error;


        root->insertKeyValue(name, value);
    }


    // did a while loop end because of our desired outcome?
    if(pos >= stringView.length() || stringView[pos] != '}')
        goto error;

    return pos;

    error:
        delete root;
        return ERROR_VALUE;

}


std::string_view::size_type startArrayProccessing(json_array* root , std::string_view stringView, std::string_view::size_type pos) {


    while (stringView.length() > pos) {

        pos = stringView.find_first_not_of(" \t\n\r", pos);

        if(pos == ERROR_VALUE || stringView[pos] == ']')
            break;

        pos = validateCommaAndSkip(root->size(), stringView, pos);

        if(pos == ERROR_VALUE)
            goto error;

        json_node* node = nullptr;


        pos = valueIdentification(stringView, pos, node);

        if(pos == ERROR_VALUE)
            goto error;

        root->addValue(node);
    }

    // did a while loop end because of our desired outcome?
    if(pos >= stringView.length() || stringView[pos] != ']')
        goto error;

    return pos;

    error:
        delete root;
        return ERROR_VALUE;

}

std::string_view::size_type valueIdentification(std::string_view stringView, std::string_view::size_type pos, json_node*& out) {

    if(pos >= stringView.length())
        return ERROR_VALUE;

    if(stringView[pos] == '{') {
        out = new json_object();
        pos = startObjectProccessing(dynamic_cast<json_object *>(out), stringView, pos + 1);

    } else if(stringView[pos] == '[') {
        out = new json_array();
        pos = startArrayProccessing(dynamic_cast<json_array *>(out), stringView, pos + 1);
    } else if(stringView[pos] == '"') {

        std::string stringvalue;
        pos = getKeyName(stringView, pos, stringvalue);

        if(pos != ERROR_VALUE) {
            out = new json_string(stringvalue);
        }

    } else if(isdigit(stringView[pos]) || stringView[pos] == '-') {

        std::string originalValue;
        long long number = 0;
        bool decimal = false , negative = false;

        if(stringView[pos] == '-') {
            negative = true;
            originalValue+=stringView[pos];
            pos+=1;
        }

        for(; pos < stringView.length() ; pos++) {

            if(stringView[pos] == '.') {
                if(decimal)
                    return ERROR_VALUE; // why is there two '.' ?

                decimal = true; // i cba to process decimal values, maybe some other time when im feeling challenged.
                originalValue+=stringView[pos];

            } else if(isdigit(stringView[pos]) ) {
                originalValue+=stringView[pos];
                if(!decimal) {

                    long long max_val = std::numeric_limits<long long>::max();

                    // Check if number*10 would overflow
                    if(number > max_val / 10) {
                        // Overflow will occur
                        return ERROR_VALUE;
                    }

                    number *= 10;

                    long long digit = stringView[pos] - '0';

                    // Check if adding digit would overflow
                    if(number > max_val - digit) {
                        // Overflow will occur
                        return ERROR_VALUE;
                    }

                    number += digit;
                }
            } else {
                // idk what this character can be, I will let the startObjectProccessing or startArrayProccessing discover
                pos-=1;
                break;
            }
        }

        if(originalValue == "-")
            return ERROR_VALUE;

        if(negative)
            number*=-1;


        out = new json_number(number, originalValue);
    } else
        return ERROR_VALUE;


    return pos + (pos == ERROR_VALUE ? 0 : 1);
}


json_node* readJsonString(std::string_view stringView) {


    if(stringView.empty() || stringView.length() < 2)
        return nullptr;

    std::string_view::size_type pos = 0;

    json_node* root = nullptr;

    pos = valueIdentification(stringView, pos, root);

    if(pos == ERROR_VALUE || root == nullptr)
        return nullptr;


    return root;
}



