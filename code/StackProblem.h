#include<iostream>
#include<stack>
#include<string>
#include<vector>

//中缀表达式转成逆波兰表达式
std::vector<std::string> reverse_to_polish(std::vector<std::string> infix_tokens)
{
    std::vector<std::string> postfix;
    std::stack<std::string> temp_stack;
    for(const std::string& current_token : infix_tokens)
    {
        if(current_token == "(")
        {
            temp_stack.push(current_token);
        }
        else if(current_token ==  ")")
        {
            std::string temp_token = temp_stack.top();
            temp_stack.pop();
            while(temp_token != "(")
            {
                postfix.push_back(temp_token);
                temp_token = temp_stack.top();
                temp_stack.pop();
            }
        }
        else if(current_token == "*" || current_token == "/" )
        {
            if(!temp_stack.empty() && temp_stack.top() != "(")
            {
                std::string temp_token = temp_stack.top();
                if(temp_token == "*" || temp_token == "/")
                {
                    postfix.push_back(temp_token);
                    temp_stack.pop();
                }
            }
            temp_stack.push(current_token);
        }
        else if(current_token == "+" || current_token == "-")
        {
            if(!temp_stack.empty() && temp_stack.top() != "(")
            {
                while(!temp_stack.empty() && temp_stack.top() != "(")
                {
                    postfix.push_back(temp_stack.top());
                    temp_stack.pop();
                }
            }
            temp_stack.push(current_token);
        }
        else 
        {
            postfix.push_back(current_token);
        }
    }
    while(!temp_stack.empty())
    {
        postfix.push_back(temp_stack.top());
        temp_stack.pop();
    }
    return postfix;
}