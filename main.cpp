#include<iostream>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <set>
#include <queue>
using namespace std;
//从正规表达式构造NFA
struct Edge{
    int to;
    string c;//需要用到ε这个字符，所以用string
};
struct Node{
    int index;
    vector<Edge>edge;
};
class NFA{
public:
    int begin, end;
    map<int,vector<Edge> >Graph;//构建图
    NFA(){
        begin = 0;
        end = 1;
        Edge edge = {end,"\u03B5"};//"\u03B5"代表ε这个字符
        Graph[begin].push_back(edge);
        Graph[end] = vector<Edge>();
    }
    NFA(string c){
        begin = 0;
        end = 1;
        Edge edge = {1,c};
        Graph[begin].push_back(edge);
        Graph[end] = vector<Edge>();
    }
    //重载三个操作方式
    NFA operator *(){
        NFA newNFA;
        int offset = 1;
        for(auto &node : Graph){
            if(node.second.size() != 0){
                for(auto &edge:node.second){
                    edge.to += offset;
                }
            }
            newNFA.Graph[node.first +offset] = node.second;
        }
        newNFA.begin = 0;
        newNFA.end = Graph.size() + 1;
        newNFA.Graph[newNFA.begin].clear();
        newNFA.Graph[newNFA.end] = vector<Edge>();
        Edge p = {newNFA.begin + 1,"\u03B5"};
        newNFA.Graph[newNFA.end -1].push_back(p);
        Edge e1 = {newNFA.begin + 1,"\u03B5"};
        newNFA.Graph[newNFA.begin].push_back(e1);
        Edge e2 = {newNFA.end,"\u03B5"};
        newNFA.Graph[newNFA.end - 1].push_back(e2);
        Edge e3 = {newNFA.end,"\u03B5"};
        newNFA.Graph[newNFA.begin].push_back(e3);
        return newNFA;
    }
    NFA operator |(NFA &t){
        NFA newNFA;
        int offset = 1;
        for(auto &node : Graph){
            if(node.second.size() != 0){
                for(auto &edge : node.second){
                    edge.to += offset;//更新
                }
            }
            newNFA.Graph[node.first + offset] = node.second;
        }
        offset = Graph.size() + 1;
        for(auto &node :t.Graph){
            if(node.second.size() != 0){
                for(auto &edge : node.second){
                    edge.to += offset;//更新
                }
            }
            newNFA.Graph[node.first + offset] = node.second;
        }
        newNFA.begin = 0;
        newNFA.end = Graph.size() + t.Graph.size() + 1;
        newNFA.Graph[newNFA.begin].clear();
        newNFA.Graph[newNFA.end] = vector<Edge>();
        Edge e1 = {1,"\u03B5"};
        Edge e2 = {Graph.size() + 1 ,"\u03B5"};
        newNFA.Graph[begin].push_back(e1);
        newNFA.Graph[begin].push_back(e2);
        Edge e3 = {newNFA.end,"\u03B5"};
        newNFA.Graph[Graph.size()].push_back(e3);
        newNFA.Graph[Graph.size() + t.Graph.size()].push_back(e3);
        return newNFA;
    }
    NFA operator +(NFA &t){
        int offset = Graph.size();
        //加入边
        for(auto &edge : t.Graph[t.begin]){
            Edge p = {t.begin + offset ,edge.c};
            Graph[end].push_back(p);
        }
        // 重合了一个节点
        offset --;
        for(auto &node : t.Graph){
            if(node.first != t.begin){
                if(node.second.size() != 0){//第一个节点不加入
                    for(auto &edge : node.second){
                        edge.to += offset;//更新
                    }
                }
                Graph[node.first + offset] = node.second;
            }

        }
        begin = 0;
        end = Graph.size() - 1; //重合了一个节点
        return *this;
    }
    set<int> e_closure(set<int> states){
        queue<int> e_queue;
        set<int> closure;
        for(auto &state : states){
            e_queue.push(state);
            closure.insert(state);
        }
        while (!e_queue.empty()){
            int node = e_queue.front();
            e_queue.pop();
            for(auto &edge :Graph[node]){
                if(edge.c == "\u03B5"){
                    e_queue.push(edge.to);
                    closure.insert(edge.to);
                }
            }
        }
        return closure;
    }

    set<int> move(set<int> v, string s){
        set<int> ans;
        for(auto &i : v){
            for(auto &edge : Graph[i]){
                if(edge.c == s){
                    ans.insert(edge.to);
                }
            }
        }
        return ans;
    }
    void printNFA(){
        for(auto &node :Graph){
            cout << "node:"<<node.first << ",edge: ";
            if(!node.second.empty()){
                for (auto &edge: node.second)
                    cout <<node.first<<"->"<< edge.c<<"->"<<edge.to <<" ";
            }
            cout << endl;
        }
        for(auto &node :Graph){
            if(!node.second.empty()){
                for (auto &edge :node.second) {
                    cout << node.first << " "<< edge.to << " " << edge.c << endl;
                }
            }

        }
        cout << "NFA起始状态: " <<begin << ",NFA终止状态为: " << end <<endl;
    }

};
class DFA{
public:
    map<int,vector<Edge> > Graph;
    map<set<int>,char > states;
    set<char> acceptStates;
    void printDFA(){
        for(auto &state :states){
            cout <<"state " <<state.second << ": ";
            for(auto &s : state.first){
                cout << s <<' ';
            }
            cout << endl;
        }
        for(auto &node :Graph){
            cout << "node:"<<char(node.first) << ",edge: ";
            if(!node.second.empty()){
                for (auto &edge: node.second)
                    cout <<char(node.first)<<"->"<< edge.c<<"->"<<char(edge.to) <<" ";
            }
            cout << endl;
        }
        for(auto &node :Graph){
            if(!node.second.empty()){
                for (auto &edge :node.second) {
                    cout << char(node.first) << " "<< char(edge.to) << " " << edge.c << endl;
                }
            }
        }
        cout << "DFA接受状态： ";
        for(auto &state : acceptStates){
            cout <<state << " ";
        }
    }
};
class Regex{
public:
    string regex;
    string formatted;
    string suffix;
    NFA nfa;
    DFA dfa;
    set<char> chars;
    void Build_format();
    void Build_suffix();
    void Build_NFA();
    void Build_DFA();

    Regex(string str){
        regex = str;
        Build_format();//转化为带有连接符号的
        Build_suffix();//转化为后缀表达式
    }
    void print_regex(){
        cout << "正规表达式为： " << this->regex << endl;
    }
    void print_formatted(){
        cout << "格式表达式为： " << this->formatted << endl;
    }
    void print_suffix(){
        cout << "后缀表达式为： " <<this->suffix << endl;
    }
    void printGraph(map<int,vector<Edge> > Graph){
        for(auto &node :Graph){
            cout << "node:"<<node.first << ",edge: ";
            if(!node.second.empty()){
                for (auto &edge: node.second)
                    cout <<node.first<<"->"<< edge.c<<"->"<<edge.to <<" ";
            }
            cout << endl;
        }
        for(auto &node :Graph){
            if(!node.second.empty()){
                for (auto &edge :node.second) {
                    cout << node.first << " "<< edge.to << " " << edge.c << endl;
                }
            }

        }
    }
};
//判断是否要加连接符
bool judge_add_point(char front,char back){
    if(front != '(' && front != '|' && back != ')' && back != '|' && back != '*' )
        return true;
    else
        return false;
}
//构造格式化表达式
void Regex::Build_format() {
    for(int i = 0; i < regex.size(); i++){
        //找到一些添加连接符号的地方，最后不需要加
        if(judge_add_point(regex[i],regex[i+1]) && i+1 < regex.size()){
            //主要要插入的位置
            formatted += regex[i];
            formatted += '+';
        }
        else
            formatted += regex[i];
    }
}
//计算运算符的优先级
int Priority(char c){
    map<char,int> m;
    m['*'] = 3;
    m['|'] = 1;
    m['+'] = 2;
    return m[c];
}
//用来表示显示的连接运算符
bool isOperator(char c){
    if(c == '*' || c == '(' || c == ')' || c == '+' || c == '|'){
        return true;
    }
    else
        return false;
}
//构造后缀表达式
void Regex::Build_suffix() {
    stack<char> s;
    for(char &c : formatted){
        //判断是不是字符
        if(isOperator(c)){
            switch (c) {
                case '(':
                    s.push(c);
                    break;
                case ')':
                    while (!s.empty() && s.top() != '('){
                        suffix += s.top();
                        s.pop();
                    }
                    s.pop();
                    break;
                // c = '*' && c == '|‘ && c == '+'的情况
                default:
                    while (!s.empty() && Priority(c) <= Priority(s.top())){
                        suffix += s.top();
                        s.pop();
                    }
                    s.push(c);
            }
        }
        //处理字母，直接加入
        else{
            chars.insert(c);
            suffix += c ;
        }
    }
    while (!s.empty()){
        suffix += s.top();
        s.pop();
    }
}
//构造NFA
void Regex::Build_NFA() {
    stack<NFA> NFAStack;
    for(char &c : suffix){
        switch (c) {
            case '*':
                NFAStack.top() = *NFAStack.top();
                break;
            case '|': {
                NFA a = NFAStack.top();
                NFAStack.pop();
                NFA b = NFAStack.top();
                NFAStack.pop();
                NFAStack.push(b|a);
                break;
            }
            case '+':{
                NFA m, n;
                m = NFAStack.top();
                NFAStack.pop();
                n = NFAStack.top();
                NFAStack.pop();
                NFAStack.push(n+m);
                break;
            }
            default:
                string s(1,c);//转化为string
                NFAStack.push(NFA(s));
                break;
        }
    }
    nfa = NFAStack.top();
    NFAStack.pop();
}


vector<int> judge_empty_state(map<vector<int>,int> states){
    for(auto &state : states){
        if(!state.second){
            return state.first;
        }
    }
    return vector<int>();
}

void Regex::Build_DFA() {
    // 初始化DFA
    char index = 'A';
    queue<set<int> > workQueue;
    // 获取NFA起始状态的ε闭包，并将其设置为DFA的起始状态
    set<int> begin_state;
    begin_state.insert(0);
    set<int> startState = nfa.e_closure(begin_state);
    dfa.states[startState] = index++;
    if (startState.find(nfa.end) != startState.end()) {
        dfa.acceptStates.insert(dfa.states[startState]); // 将新的接受状态添加到集合中
    }
    workQueue.push(startState);
    // 开始循环，处理所有的状态集合
    while (!workQueue.empty()) {
        set<int> currentState = workQueue.front();
        workQueue.pop();
        // 对于当前状态集合，按照字母表中的每个字符计算转移
        for (char c : chars) {
            string s(1, c); // 将字符转换为字符串
            set<int> nextState = nfa.e_closure(nfa.move(currentState, s)); // 计算转移后的ε闭包
            if (nextState.empty()) {
                continue; // 跳过空的ε闭包
            }
            // 如果新状态集合还没有在DFA中出现过，加入到DFA中
            if (dfa.states.find(nextState) == dfa.states.end()) {
                dfa.states[nextState] = index++;
                workQueue.push(nextState);
            }
            if (nextState.find(nfa.end) != nextState.end()) {
                dfa.acceptStates.insert(dfa.states[nextState]); // 将新的接受状态添加到集合中
            }
            // 在DFA中为当前状态集合到新状态集合添加边
            Edge newEdge = {dfa.states[nextState], s};
            dfa.Graph[dfa.states[currentState]].push_back(newEdge);
        }
    }
}




int main(){
    string  str = "(a|b)*abb";
    //string  str = "a|b|c|d";
    //string str = "abc";
    //string str = "(aa)*";
    Regex s(str);
    s.print_regex();
    s.print_formatted();
    s.print_suffix();
    s.Build_NFA();
    s.nfa.printNFA();
    s.Build_DFA();
    s.dfa.printDFA();
    return 0;
}

