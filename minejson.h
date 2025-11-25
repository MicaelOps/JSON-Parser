//
// Created by Micael Cossa on 29/07/2025.
//

#ifndef MINECRAFTSERVER_JSON_READER_H
#define MINECRAFTSERVER_JSON_READER_H

#include <unordered_map>
#include <string>
#include <utility>
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

    [[nodiscard]] virtual std::string toString() const = 0;
    [[nodiscard]] virtual JSON_NODE_TYPE getType() const {
        return JSON_NODE_TYPE::JSON_INVALID;
    }
};



class json_string : public json_node {

private:
    std::string value;

public:
    explicit json_string(std::string basicString) : value(std::move(basicString)) {}

    ~json_string() override = default;


    [[nodiscard]] JSON_NODE_TYPE getType() const override  {
        return JSON_NODE_TYPE::JSON_STRING;
    }
    [[nodiscard]] std::string toString()  const override {
        return "\"" + value + "\"";
    }
};

class json_number : public json_node {

private:
    std::string originalValue;
    long long number;

public:

    explicit json_number(long long number, std::string originalValue) : originalValue(std::move(originalValue)) , number(number) {}
    ~json_number() override = default;

    [[nodiscard]] JSON_NODE_TYPE getType() const override  {
        return JSON_NODE_TYPE::JSON_NUMBER;
    }

    [[nodiscard]] unsigned long long getNumber() const{
        return number;
    }

    [[nodiscard]] double convertToDecimal() const {
        // blalablala convert the originalValue to decimal
        // i cba to do this
        return 0.0;
    }

    [[nodiscard]] std::string toString() const override {
        return originalValue;
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
    [[nodiscard]] std::vector<json_node*>::size_type size() const {
        return data.size();
    }
    void addValue(json_node* value) {
        data.emplace_back(value);
    }
    [[nodiscard]] JSON_NODE_TYPE getType() const override  {
        return JSON_NODE_TYPE::JSON_ARRAY;
    }

    [[nodiscard]] std::string toString() const  override {
        std::string text("[");
        for (auto &item: data) {
            text+=item->toString();
        }
        text+=']';
        return text;
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

    JSON_NODE_TYPE getType() const override  {
        return JSON_NODE_TYPE::JSON_OBJECT;
    }


    std::string toString() const  override {
        std::string text("{");
        for (auto &item: data) {
            text+="\"" + item.first + "\" : ";
            text+=item.second->toString();
        }
        text+='}';
        return text;
    }
};

json_node* readJsonString(std::string_view jsonString);

std::string_view::size_type valueIdentification(std::string_view text, std::string_view::size_type pos, json_node*& out);

#endif //MINECRAFTSERVER_JSON_READER_H
