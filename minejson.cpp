//
// Created by Micael Cossa on 29/07/2025.
//

#include "minejson.h"
#include <stack>
#include <iostream>


constexpr short int ERROR_VALUE = -1;

int readJsonObject(const std::string_view& stringView, int pos) {

    return pos + 1;
}
int proccessKeyValue(json_object* jsonObject, std::string_view stringView, int pos) {
    std::string_view::size_type quotationBeginPos = stringView.find_first_of('"', pos);
    stringView+=quotationBeginPos;
    std::string_view::size_type quotationEndPos =.find_first_of('"', stringView.length());

    if(quotationBeginPos == std::string_view::npos ||  quotationEndPos == std::string_view::npos) {
        std::cout << "Unable to find quotation mark for JSON Document \n";
        std::cout << quotationBeginPos;
        std::cout << "\n";
        std::cout << quotationEndPos;
        return nullptr;
    }

    std::string_view elementName = stringView.substr(quotationBeginPos, quotationEndPos);
    std::cout << "Captured element name : " << elementName << "\n";
    return ERROR_VALUE;
}

int startObjectProccessing(json_object* root , std::string_view& stringView, int pos) {

    // while loop to dodge spaces and get to : ?

    while (stringView.length() > pos && stringView[pos] != '}') {

        if(stringView[pos] == ' ') {
            pos++;
            continue;
        }

        pos = proccessKeyValue(dynamic_cast<json_object*>(root), stringView, pos);

        // did an error happening while trying to process the key-value pair?
        if(pos == ERROR_VALUE)
            break;
    }

    // we have to cleanse memory if something went wrong
    if(pos==ERROR_VALUE) {
        delete root;
        return pos;
    }

    // if nothing went wrong then we just check if we have the correct ending.
    return stringView[pos] != ']' ? ERROR_VALUE: pos;
}

int startArrayProccessing(json_array* root , std::string_view& stringView, int pos) {


    while (stringView.length() > pos && stringView[pos] != ']') {

        if(stringView[pos] == ' ') {
            pos++;
            continue;
        }

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

    int pos = 0;

    json_node* root = nullptr;

    if(stringView[pos] == '{') {
        root = new json_object();
        pos = startObjectProccessing(dynamic_cast<json_object *>(root), stringView, pos + 1);

    } else if(stringView[pos] == '[') {
        root = new json_array();
        pos = startArrayProccessing(dynamic_cast<json_array *>(root), stringView, pos + 2);
    }

    if(pos == ERROR_VALUE)
        return nullptr;

    return root;
}



