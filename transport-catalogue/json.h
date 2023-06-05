#pragma once


#include <map>
#include <vector>
#include <string>
#include <variant>
#include <iostream>

using namespace std::string_view_literals;

namespace json {

class Node;

using Dict = std::map<std::string, Node>;
using Array = std::vector<Node>;

class ParsingError : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

using NodeData = std::variant<std::nullptr_t, Array, Dict, bool, int, double, std::string>;

class Node : private NodeData {
public:
    using NodeData::variant;

    bool IsInt() const;
    bool IsDouble() const;
    bool IsPureDouble() const;
    bool IsBool() const;
    bool IsString() const;
    bool IsNull() const;
    bool IsArray() const;
    bool IsMap() const;

    int AsInt() const;
    bool AsBool() const;
    double AsDouble() const;
    const std::string& AsString() const;
    const Array& AsArray() const;
    const Dict& AsMap() const;

    const NodeData& GetValue() const;

    friend bool operator==(const Node& lhs, const Node& rhs) {
        return static_cast<NodeData>(lhs) == static_cast<NodeData>(rhs);
    }
    friend bool operator!=(const Node& lhs, const Node& rhs) {
        return !(lhs == rhs);
    }


};

class Document {
public:
    explicit Document(Node root);

    const Node& GetRoot() const;

    bool operator==(const Document& rhs) const;
    bool operator!=(const Document& rhs) const;

private:
    Node root_;
};

Document Load(std::istream& input);

struct PrintContext {
    std::ostream& out;
    int indent_step = 4;
    int indent = 0;

    void PrintIndent() const {
        for (int i = 0; i < indent; ++i) {
            out.put(' ');
        }
    }

    PrintContext Indented() const {
        return { out, indent_step, indent_step + indent };
    }
};

void PrintValue(std::nullptr_t, const PrintContext& ctx);
void PrintValue(std::string value, const PrintContext& ctx);
void PrintValue(bool value, const PrintContext& ctx);
void PrintValue(Array array, const PrintContext& ctx);
void PrintValue(Dict dict, const PrintContext& ctx);

template <typename Value>
void PrintValue(const Value& value, const PrintContext& ctx) {
    ctx.out << value;
}
void PrintNode(const Node& node, const PrintContext& ctx);
void Print(const Document& doc, std::ostream& output);

}