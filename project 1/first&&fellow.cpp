#include <iostream>
#include <map>
#include <set>
#include <vector>

#include <sstream>
using namespace std;

class Productions{
private:
    map<string ,set<string> > first, follow;
    map<string ,vector<string> > productions;
public:
    void read_productions(){
        string line;
        cout << "输入产生式：" << endl;
        while (getline(cin,line) && line != "end"){
            istringstream iss(line);//分割字符串
            string non_terminal;//非终结符号
            string arrow;//箭头
            string production;
            iss >> non_terminal >> arrow;
            while (iss >> production) {
                // 如果读取到了'|'，则跳过它
                if (production == "|") {
                    continue;
                }
                productions[non_terminal].push_back(production);
            }
        }
    }
    //处理单个字符
    set<string> getFirst(char symbol) {
        //小写字母终结符号加入
        string s(1,symbol) ;//转化为string
        if (!isupper(symbol)) {
            set<string> c;
            c.insert(s);
            return c;
        }

        return first[s];
    }
    //处理字符串
    set<string> getFirstString(const string& str) {
        set<string> result;
        for (char symbol : str) {
            set<string> symbolFirst = getFirst(symbol);
            result.insert(symbolFirst.begin(), symbolFirst.end());
            //如果找到了小写字母就退出
            if(symbolFirst.size() == 1)break;
            //特殊处理e,不包含e就退出
            if (symbolFirst.find("ε") == symbolFirst.end()) {
                break;
            }
        }
        return result;
    }
    void calculateFirst() {
        bool changed = true;
        while (changed) {
            changed = false;
            for (const auto& prod : productions) {
                for (const auto& str : prod.second) {
                    size_t beforeSize = first[prod.first].size();
                    if(str == "ε"){
                        first[prod.first].insert(str);
                    }
                    else {
                        set<string> strFirst = getFirstString(str);
                        first[prod.first].insert(strFirst.begin(), strFirst.end());
                    }
                    if (first[prod.first].size() != beforeSize) {
                        changed = true;
                    }
                }
            }
        }
    }
    void calculateFollow() {
        follow["S"].insert("$");  //假设S是第一个推导的
        bool changed = true;
        while (changed){
            changed = false;
            for (const auto& prod : productions) {
                for (const auto& str : prod.second) {
                    for (size_t i = 0; i < str.size(); ++i) {//遍历字符串
                        if (isupper(str[i])) {//非终结符号
                            //如果后面还有其他符号，First集（去除e）加入
                            //如果是最后一个，就将左侧的Follow加入到右侧
                            string s(1,str[i]);//转化为string
                            size_t beforeSize = follow[s].size();
                            set<string> nextFirst = (i + 1 < str.size()) ? getFirstString(str.substr(i + 1)) : follow[prod.first];
                            if (nextFirst.count("ε") == str.size() - 1 - i) {
                                nextFirst.erase("ε");
                                nextFirst.insert(follow[prod.first].begin(), follow[prod.first].end());
                            }
                            if(nextFirst.find("ε") !=nextFirst.end())nextFirst.erase("ε");
                            follow[s].insert(nextFirst.begin(), nextFirst.end());
                            //检查Follow是否更新
                            if (follow[s].size() != beforeSize) {
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
    }
    void PrintFirst(){
        for (const auto& f : first) {
            cout << "FIRST(" << f.first << ") = { ";
            for (string c : f.second) {
                cout << c << " ";
            }
            cout << "}" << endl;
        }
    }
    void PrintFollow(){
        for (const auto& f : follow) {
            cout << "FOLLOW(" << f.first << ") = { ";
            for (string c : f.second) {
                cout << c << " ";
            }
            cout << "}" << endl;
        }
    }

    void CreateLL1Table() {
        map<pair<string , string>, string> ll1Table;
        for (const auto& prod : productions) {
            string nonTerminal = prod.first;//左侧非终结符号
            for (const auto& alpha : prod.second) {
                set<string> firstAlpha = first[nonTerminal];
                //set<string> firstAlpha = getFirstString(alpha);//求first集
                for (string terminal : firstAlpha) {//终结符号
                    if (terminal != "ε") {
                        auto table = make_pair(nonTerminal, terminal);
                        ll1Table[table] = alpha;
                    } else {//如果是e，用Follow集
                        set<string> followNonTerminal = follow[nonTerminal];
                        for (string followTerminal : followNonTerminal) {
                            auto table = make_pair(nonTerminal, followTerminal);
                            ll1Table[table] = alpha;
                        }
                    }
                }
            }
        }
        // 打印分析表
        cout << "LL(1) 分析表:" << endl;
        for (const auto& entry : ll1Table) {
            cout << "M[" << entry.first.first << ", " << entry.first.second << "] = " << entry.second << endl;
        }
    }

//    void creatll1table(){
//        map<pair<char, string>, string> ll1Table;
//        for(auto &prod : productions){
//            for(auto &terminal : first[prod.first]){
//                auto table = make_pair(prod.first, terminal);
//                ll1Table[table] = prod.second
//            }
//        }
//    }

};




int main() {
    Productions s;
    s.read_productions();
    s.calculateFirst();
    s.calculateFollow();
    s.PrintFirst();
    s.PrintFollow();
    s.CreateLL1Table();
    return 0;
}
/*

S -> a
B -> t | S

S → A | a
A → Bc | ε
B → d | AS
S → MH | a
H → LSo | ε
K → dML | ε
L → eHf
M → K | bLM


S → A | a
A → Bc | ε
B → d | AS

 */