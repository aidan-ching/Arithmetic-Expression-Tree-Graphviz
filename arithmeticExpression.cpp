#include "arithmeticExpression.h"
#include <stack>
#include <fstream>
#include <sstream>

arithmeticExpression::arithmeticExpression(const string &user)
{
    infixExpression = user;
}

void arithmeticExpression::buildTree()
{
    string postfixExpression = infix_to_postfix();
    stack<TreeNode*> treeCreator;
    char keyAssign = 'a';
    TreeNode *temp;
    TreeNode *temp1;
    TreeNode *temp2;

    for (unsigned i = 0; i < postfixExpression.size(); ++i)
    {
        temp = new TreeNode(postfixExpression.at(i), keyAssign + i);
        
        if (!isOperator(postfixExpression.at(i)))
        {
            treeCreator.push(temp);
        }

        else
        {
            temp1 = treeCreator.top();
            treeCreator.pop();
            temp2 = treeCreator.top();
            treeCreator.pop();

            temp->right = temp1;
            temp->left = temp2;
            treeCreator.push(temp);
        }
    }

    root = treeCreator.top();
    treeCreator.pop();
    
}

void arithmeticExpression::infix()
{
    infix(root);
}

void arithmeticExpression::prefix()
{
    prefix(root);
}

void arithmeticExpression::postfix()
{
    postfix(root);
}

void arithmeticExpression::infix(TreeNode *curr)
{
    
    
    if (curr)
    {
        if(curr->left == 0 && curr->right == 0)
        {
            cout << curr->data;
        }
        else
        {
            cout << "(";
            infix(curr->left);
            cout << curr->data;
            infix(curr->right);
            cout << ")";
        }
    }
    
}

void arithmeticExpression::prefix(TreeNode *curr)
{
    if (curr)
    {
        cout << curr->data;
        prefix(curr->left);
        prefix(curr->right);
    }
}

void arithmeticExpression::postfix(TreeNode *curr)
{
    if (curr)
    {
        postfix(curr->left);
        postfix(curr->right);
        cout << curr->data;
    }
}

int arithmeticExpression::priority(char op){
    int priority = 0;
    if(op == '('){
        priority =  3;
    }
    else if(op == '*' || op == '/'){
        priority = 2;
    }
    else if(op == '+' || op == '-'){
        priority = 1;
    }
    return priority;
}

string arithmeticExpression::infix_to_postfix(){
    stack<char> s;
    ostringstream oss;
    char c;
    for(unsigned i = 0; i< infixExpression.size();++i){
        c = infixExpression.at(i);
        if(c == ' '){
            continue;
        }
        if(c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')'){ //c is an operator
            if( c == '('){
                s.push(c);
            }
            else if(c == ')'){
                while(s.top() != '('){
                    oss << s.top();
                    s.pop();
                }
                s.pop();
            }
            else{
                while(!s.empty() && priority(c) <= priority(s.top())){
                    if(s.top() == '('){
                        break;
                    }
                    oss << s.top();
                    s.pop();
                }
                s.push(c);
            }
        }
        else{ //c is an operand
            oss << c;
        }
    }
    while(!s.empty()){
        oss << s.top();
        s.pop();
    }
    return oss.str();
}

void arithmeticExpression::visualizeTree(const string &outputFilename){
    ofstream outFS(outputFilename.c_str());
    if(!outFS.is_open()){
        cout<<"Error opening "<< outputFilename<<endl;
        return;
    }
    outFS<<"digraph G {"<<endl;
    visualizeTree(outFS,root);
    outFS<<"}";
    outFS.close();
    string jpgFilename = outputFilename.substr(0,outputFilename.size()-4)+".jpg";
    string command = "dot -Tjpg " + outputFilename + " -o " + jpgFilename;
    system(command.c_str());
}

bool arithmeticExpression::isOperator(char a)
{
    if (a == '+' || a == '-' || a == '*' || a == '/')
    {
        return true;
    }

    else
    {
        return false;
    }
}

void arithmeticExpression::visualizeTree(ofstream & outFS, TreeNode *n)
{
    if(n){
        outFS<<n->key << "[ label = \""<<n->data<<"\"];"<<endl;
        if(n->left){
            visualizeTree(outFS,n->left);
            outFS<<n->key <<" -> " <<n->left->key<<";"<<endl;    
        }
        
        if(n->right){
            visualizeTree(outFS,n->right);
            outFS<<n->key <<" -> " <<n->right->key<<";"<<endl;    
        }
    }
}
