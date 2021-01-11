#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

struct Node {
    std::string type;
    Node(const std::string& newType);
    virtual ~Node();
};
struct BinaryExpression : Node {
    Node* left;
    Node* right;
    BinaryExpression(std::string type, Node* newLeft, Node* newRight);
    ~BinaryExpression();
};
struct Number : Node {
    std::string value;
    Number(const std::string& newValue);
};
struct Name : Node {
    std::string id;
    Name(const std::string& newId);
};

struct State {
    int position;
    std::vector<std::string> tokens;
    State(std::vector<std::string> newTokens);
};

void peek(State* state);
void consume(State* state, std::string token);

Node* parse_primary_expr(State* state);
Node* parse_mul_expr(State* state);
Node* parse_expr(State* state);

Node* parse(std::string code);

#endif
