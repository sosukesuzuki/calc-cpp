#include <string>
#include <iostream>
#include "parser.h"

std::string get_log(Node* node, int level = 0) {
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
        std::string log = "";
        level++;
        std::string left_log = get_log(binary_expression->left, level);
        std::string right_log = get_log(binary_expression->right, level);
        std::string indent  = "";
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
    std::string log_text = get_log(ast);
    std::cout << log_text << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "expected some expr" << std::endl;
        return 1;
    }
    std::string code = argv[1];
    Node* ast = parse(code);
    display(ast);
    delete ast;
}
