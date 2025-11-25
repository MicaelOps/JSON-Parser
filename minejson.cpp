//
// Created by Micael Cossa on 29/07/2025.
//

#include "minejson.h"
#include <stack>
#include <iostream>
#include <sstream>


constexpr std::string_view::size_type ERROR_VALUE = std::string_view::npos;


std::string_view::size_type getKeyName(const std::string_view stringView, std::string_view::size_type pos, std::string& out) {

    if(stringView[pos] != '"') {
        std::cout << "Unable to find fist quotation mark for JSON Document \n";
        std::cout << pos;
        std::cout << "\n";
        return ERROR_VALUE;
    }

    std::string_view::size_type quotationEndPos = pos + 1;
    std::ostringstream name;

    while(quotationEndPos < stringView.size()) {

        if(stringView[quotationEndPos] == '\\') {
            // Skip the backslash and the next character
            quotationEndPos += 2;
            continue;
        }
        if(stringView[quotationEndPos] == '"') {
            break;  // Found actual end
        }

        name << stringView[quotationEndPos];
        quotationEndPos++;
    }


    // The only reason this would be true is if the while loop stop due to quotationEndPos reaching the same length as the json string.
    if(quotationEndPos >= stringView.size()) {
        return ERROR_VALUE;
    }


    out = name.str();
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
        if(pos == ERROR_VALUE) {
            std::cout << "Unable to find end quotation mark for JSON Object key \n";
            goto error;
        }

        // checks if ':' exists and returns the position of the : char
        pos = hasValueSignate(stringView, pos + 1);
        if(pos == ERROR_VALUE) {
            std::cout << "Unable to find : signature in JSON\n";
            goto error;
        }

        json_node* value = nullptr;

        // skip the whitespace after, returns the position of the character after the value identified
        pos = valueIdentification(stringView, stringView.find_first_not_of(" \t\n\r", pos+1), value);

        if(pos == ERROR_VALUE || value == nullptr) {
            std::cout << "Unable to proccess JSON Value \n";
            goto error;
        }

        std::cout << "key  " << name << " and value " << value->toString() << " added. starting at " << stringView[pos] << " next \n";
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

    } else if(isdigit(stringView[pos])) {

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

    if(pos == ERROR_VALUE || root == nullptr) {
        std::cout << "Unable to read JSON \n";
        return nullptr;
    }


    return root;
}



