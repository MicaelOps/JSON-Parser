//
// Created by Micael Cossa on 29/07/2025.
//

#ifndef MINECRAFTSERVER_JSON_READER_H
#define MINECRAFTSERVER_JSON_READER_H

#include <unordered_map>
#include <string>
#include <vector>


enum class JSON_NODE_TYPE : unsigned short int {
    JSON_OBJECT = 0,
    JSON_NUMBER = 1,
    JSON_STRING = 3,
    JSON_ARRAY = 4,
    JSON_INVALID = 100
};


// at some point in the future i will do this
class json_node {

public:

    virtual ~json_node() = default;

    virtual std::string toString() const = 0;
    virtual JSON_NODE_TYPE getType() const {
        return JSON_NODE_TYPE::JSON_INVALID;
    }
};



class json_string : public json_node {

public:
    ~json_string() override = default;


    JSON_NODE_TYPE getType() const override  {
        return JSON_NODE_TYPE::JSON_STRING;
    }
    std::string toString()  const override {
        return "[]";
    }
};

class json_int : public json_node {
public:

    ~json_int() override = default;

    JSON_NODE_TYPE getType() const override  {
        return JSON_NODE_TYPE::JSON_NUMBER;
    }

    std::string toString() const override {
        return "[]";
    }
};

class json_array : public json_node {
private:
    std::vector<json_node*> data;
public:
    json_array() = default;
    json_array(const json_array&) = delete;
    json_array& operator=(const json_array&) = delete;

    ~json_array() override {
        for (auto item: data) {
            delete item;
        }
    }
    std::vector<json_node*>::size_type size() const {
        return data.size();
    }
    void addValue(json_node* value) {
        data.emplace_back(value);
    }
    JSON_NODE_TYPE getType() const override  {
        return JSON_NODE_TYPE::JSON_ARRAY;
    }

    std::string toString() const  override {
        return "[]";
    }
};

class json_object : public json_node {

private:
    std::unordered_map<std::string, json_node*> data;

public:
    json_object() = default;
    json_object(const json_object&) = delete;
    json_object& operator=(const json_object&) = delete;

    ~json_object() override {
        for (const auto &item: data) {
            delete item.second;
        }
    }

    std::unordered_map<std::string, json_node*>::size_type size() const {
        return data.size();
    }
    void insertKeyValue(const std::string& key, json_node* value) {
        data[key] = value;
    }

    bool existsValue(const std::string& key) {
        return data.find(key) != data.end();
    }

    json_node* retrieveValue(const std::string& key) {

        if(!existsValue(key))
            return nullptr;

        return data[key];
    }

    JSON_NODE_TYPE getType() const override  {
        return JSON_NODE_TYPE::JSON_OBJECT;
    }


    std::string toString() const  override {
        return "[]";
    }
};

json_node* readJsonString(std::string_view jsonString);
std::string_view toJsonString(json_node* jsonNode);


#endif //MINECRAFTSERVER_JSON_READER_H
