#include <vector>
#include <string>
#include "lexer.h"

struct Node {
    std::string type;
    Node(const std::string& newType): type(newType) {}
    virtual ~Node() {};
};

struct BinaryExpression : Node {
    Node* left;
    Node* right;
    BinaryExpression(std::string type, Node* newLeft, Node* newRight): Node(type), left(newLeft), right(newRight) {}
    ~BinaryExpression() {
        delete left;
        delete right;
    }
};

struct Number : Node {
    std::string value;
    Number(const std::string& newValue): Node("number"), value(newValue) {}
};

struct Name : Node {
    std::string id;
    Name(const std::string& newId): Node("name"), id(newId) {}
};

struct State {
    int position;
    std::vector<std::string> tokens;
    State(std::vector<std::string> newTokens): position(0), tokens(newTokens) {}
};

std::string peek(State* state) {
    return state->tokens[state->position];
}

void consume(State* state, std::string token = "") {
    if (token != "" && token != state->tokens[state->position]) {
        throw std::runtime_error("invalid token");
    }
    state->position++;
}

Node* parse_expr(State* state);

Node* parse_primary_expr(State* state) {
    std::string token = peek(state);
    if (is_number(&token)) {
        consume(state);
        Number* number = new Number(token);
        return number;
    } else if (is_name(&token)) {
        consume(state);
        Name* name = new Name(token);
        return name;
    } else if (token == "(") {
        consume(state);
        Node* expr = parse_expr(state);
        if (peek(state) != ")") {
            throw std::runtime_error("expected )");
        }
        consume(state, ")");
        return expr;
    } else {
        throw std::runtime_error("expected a number, a variable, or parentheses");
    }
}

Node* parse_mul_expr(State* state) {
    Node* expr = parse_primary_expr(state);
    std::string token = peek(state);
    while (token == "*" || token == "/") {
        consume(state, token);
        Node* rhs = parse_primary_expr(state);
        Node* lhs = expr;
        BinaryExpression* binExpr = new BinaryExpression(token, lhs, rhs);
        expr = binExpr;
        token = peek(state);
    }
    return expr;
}

Node* parse_expr(State* state) {
    Node* expr = parse_mul_expr(state);
    std::string token = peek(state);
    while(token == "+" || token == "-") {
        consume(state, token);
        Node* rhs = parse_mul_expr(state);
        Node* lhs = expr;
        BinaryExpression* binExpr = new BinaryExpression(token, lhs, rhs);
        expr = binExpr;
        token = peek(state);
    }
    return expr;
}

Node* parse(std::string code) {
    State* state = new State(tokenize(code));
    Node* result = parse_expr(state);
    if (state->position != state->tokens.size()) {
        throw std::runtime_error("unexpected" + peek(state) + "'");
    }
    delete state;
    return result;
}
