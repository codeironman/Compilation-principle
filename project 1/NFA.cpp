#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>

// Function to return precedence of operators
int precedence(char c) {
    if(c == '*') return 3;
    else if(c == '.') return 2; // Assuming '.' as concatenation operator
    else if(c == '|') return 1;
    else return -1;
}

// The main function to convert infix expression to postfix
std::string infixToPostfix(std::string regex) {
    std::stack<char> st;
    std::string postfix;

    // Insert concatenation operator '.' between adjacent characters
    std::string formatted;
    for (int i = 0; i < regex.size(); i++) {
        if (i + 1 < regex.size() && regex[i] != '(' && regex[i] != '|' &&
            regex[i+1] != ')' && regex[i+1] != '*' && regex[i+1] != '|') {
            formatted += regex[i];
            formatted += '.'; // Insert concatenation operator
        } else {
            formatted += regex[i];
        }
    }

    for(char c : formatted) {
        // If the scanned character is an operand, add it to output
        if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            postfix += c;
        }
            // If the scanned character is an '(', push it to the stack
        else if(c == '(') {
            st.push('(');
        }
            // If the scanned character is an ')', pop and output from the stack until '(' is encountered
        else if(c == ')') {
            while(st.top() != '(') {
                postfix += st.top();
                st.pop();
            }
            st.pop(); // remove '(' from stack
        }
            // If an operator is encountered
        else {
            while(!st.empty() && precedence(c) <= precedence(st.top())) {
                postfix += st.top();
                st.pop();
            }
            st.push(c);
        }
    }

    // Pop all the remaining elements from the stack
    while(!st.empty()) {
        postfix += st.top();
        st.pop();
    }

    return postfix;
}

int main() {
    std::string regex = "(a|b)*abb";
    std::string postfix = infixToPostfix(regex);
    std::cout << "Postfix Notation: " << postfix << std::endl;
    return 0;
}
