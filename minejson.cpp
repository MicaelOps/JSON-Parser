//
// Created by Micael Cossa on 29/07/2025.
//

#include "minejson.h"
#include <stack>
#include <iostream>
#include <sstream>


constexpr std::string_view::size_type ERROR_VALUE = std::string_view::npos;


std::string_view::size_type getKeyName(const std::string_view& stringView, std::string_view::size_type pos, std::string& out) {

    std::string_view::size_type quotationBeginPos = stringView.find_first_of('"', pos);

    if(quotationBeginPos == std::string_view::npos) {
        std::cout << "Unable to find fist quotation mark for JSON Document \n";
        std::cout << quotationBeginPos;
        std::cout << "\n";
        return -1;
    }

    std::string_view::size_type quotationEndPos = quotationBeginPos + 1;
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
    if(quotationEndPos >= stringView.size())
        return ERROR_VALUE;


    out = name.str();
    std::cout << "Captured element name : " << out << "\n";
    return quotationEndPos;
}

// Pretty much just checking if there is a ':'
std::string_view::size_type hasValueSignate(std::string_view& stringView, std::string_view::size_type pos) {
    return stringView.find(':', pos);
}

std::string_view::size_type getValue(const json_object* root, json_node* out, std::string_view& stringView, std::string_view::size_type pos ) {
    pos = stringView.find_first_not_of(' ', pos);

    if(pos == ERROR_VALUE)
        return ERROR_VALUE;

    out
    return pos;
}
std::string_view::size_type startObjectProccessing(json_object* root , std::string_view& stringView, std::string_view::size_type pos) {

    // while loop to dodge spaces and get to : ?

    while (stringView.length() > pos && stringView[pos] != '}') {


        std::string name;
        pos = getKeyName(stringView, pos, name);

        // did an error happening while trying to process the key?
        if(pos == ERROR_VALUE) {
            std::cout << "Unable to find end quotation mark for JSON Document \n";
            break;
        }

        if(!hasValueSignate(stringView, pos)) {
            std::cout << "Unable to find : signature in JSON \n";
            break;
        }

        json_node* value = nullptr;
        pos = getValue(root, value, stringView, pos);

        if(pos == ERROR_VALUE || value == nullptr)
            break;

        std::string_view::size_type valueIndicator = stringView.find(':', pos);

        if(valueIndicator == std::string_view::npos){
            std::cout << "Unable to find : signature in JSON \n";
            std::cout << valueIndicator;
            break;
        }
        root->insertKeyValue(name, value);

        pos++;
    }

    // we have to cleanse memory if something went wrong
    if(pos==ERROR_VALUE) {
        delete root;
        return pos;
    }

    // if nothing went wrong then we just check if we have the correct ending.
    return stringView[pos] != ']' ? ERROR_VALUE: pos;
}


std::string_view::size_type startArrayProccessing(json_array* root , std::string_view& stringView, std::string_view::size_type pos) {


    while (stringView.length() > pos && stringView[pos] != ']') {

        pos = stringView.find_first_not_of(' ', pos);

        if(pos == ERROR_VALUE)
            break;

        if(stringView[pos] == '{') {
            auto* new_object_root = new json_object();
            pos = startObjectProccessing(dynamic_cast<json_object*>(new_object_root), stringView, pos+1);


            // did an error happening while trying to process the object?
            if(pos == ERROR_VALUE)
                break;

            root->addValue(new_object_root);

        } else if(stringView[pos] == '[') {
            auto* new_array_root = new json_array();
            pos = startArrayProccessing(dynamic_cast<json_array*>(new_array_root), stringView, pos+1);

            // did an error happening while trying to process the object?
            if(pos == ERROR_VALUE)
                break;

            root->addValue(new_array_root);

        } else if(stringView[pos] == '"') {

        } else if(isdigit(stringView[pos])) {
            
        } else if(stringView[pos] == ',') {
            if(root->size() == 0) { // why is there a comma if there are no values on the array?
                pos = ERROR_VALUE;
                break;
            }

        } else { // invalid character? TO BE REVISED
            pos = ERROR_VALUE;
            std::cout << "Unexpected character " << stringView[pos] << " \n";
            break;
        }

        pos++;
    }

    // we have to cleanse memory if something went wrong
    if(pos==ERROR_VALUE) {
        delete root;
        return pos;
    }

    // if nothing went wrong then we just check if we have the correct ending.
    return stringView[pos] != ']' ? ERROR_VALUE: pos;

}


json_node* readJsonString(std::string_view stringView) {


    if(stringView.empty() || stringView.length() < 2)
        return nullptr;

    std::string_view::size_type pos = 0;

    json_node* root = nullptr;

    if(stringView[pos] == '{') {
        root = new json_object();
        pos = startObjectProccessing(dynamic_cast<json_object *>(root), stringView, pos + 1);

    } else if(stringView[pos] == '[') {
        root = new json_array();
        pos = startArrayProccessing(dynamic_cast<json_array *>(root), stringView, pos + 1);
    }

    if(pos == ERROR_VALUE)
        return nullptr;

    return root;
}



