#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

// Tokenizer

vector<string> tokenize(string str) {
    regex pattern("[A-Za-z]+|[0-9]+|\\S");
    sregex_iterator itr{str.begin(), str.end(), pattern};
    sregex_iterator last{};
    vector<string> tokens;
    for (; itr != last; ++itr) {
      tokens.push_back((*itr).str());
    }
    return tokens;
}

bool is_number(string* token) {
    if (!token) {
        return false;
    }
    return regex_match(*token, regex("[0-9]+"));
}

bool is_name(string* token) {
    if (!token) {
        return false;
    }
    return regex_match(*token, regex("[A-Za-z]+"));
}

// Parser

struct Node {
    string type;
    Node(const string& newType): type(newType) {}
    virtual ~Node() {};
};

struct BinaryExpression : Node {
    Node* left;
    Node* right;
    BinaryExpression(string type, Node* newLeft, Node* newRight): Node(type), left(newLeft), right(newRight) {}
    ~BinaryExpression() {
        delete left;
        delete right;
    }
};

struct Number : Node {
    string value;
    Number(const string& newValue): Node("number"), value(newValue) {}
};

struct Name : Node {
    string id;
    Name(const string& newId): Node("name"), id(newId) {}
};

struct State {
    int position;
    vector<string> tokens;
};

string peek(State* state) {
    return state->tokens[state->position];
}

void consume(State* state, string token = "") {
    if (token != "" && token != state->tokens[state->position]) {
        throw runtime_error("invalid token");
    }
    state->position++;
}

Node* parse_primary_expr(State* state);
Node* parse_mul_expr(State* state);
Node* parse_expr(State* state);

Node* parse_primary_expr(State* state) {
    string token = peek(state);
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
            throw runtime_error("expected )");
        }
        consume(state, ")");
        return expr;
    } else {
        throw runtime_error("expected a number, a variable, or parentheses");
    }
}

Node* parse_mul_expr(State* state) {
    Node* expr = parse_primary_expr(state);
    string token = peek(state);
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
    string token = peek(state);
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

Node* parse(string code) {
    State state;
    state.position = 0;
    state.tokens = tokenize(code);
    Node* result = parse_expr(&state);
    if (state.position != state.tokens.size()) {
        throw runtime_error("unexpected" + peek(&state) + "'");
    }
    return result;
}

// Main

string get_log(Node* node, int level = 0) {
    Number* number = dynamic_cast<Number*>(node);
    if (number) {
        return "{ type: number, value: " + number->value + " }";
    }
    Name* name = dynamic_cast<Name*>(node);
    if (name) {
        return "{ type: name, value: " + name->id + " }";
    }
    BinaryExpression* binary_expression = dynamic_cast<BinaryExpression*>(node);
    if (binary_expression) {
        string log = "";
        level++;
        string left_log = get_log(binary_expression->left, level);
        string right_log = get_log(binary_expression->right, level);
        string indent  = "";
        for(int i = 0; i < level; ++i) {
            indent += "  ";
        }
        log += "type: " + binary_expression->type;
        log += "\n" + indent + "left: " + left_log;
        log += "\n" + indent + "right: " + right_log;
        return log;
    }
    return "";
}

void display(Node* ast) {
    string log_text = get_log(ast);
    cout << log_text << endl;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "expected some expr" << endl;
        return 1;
    }
    string code = argv[1];
    Node* ast = parse(code);
    display(ast);
    delete ast;
}
