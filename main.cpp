/*
    Name :  Yassin  Gritli
    id   :  ###
    date :  2018/10/26
    ********************
    Table Driven Scanner
    ********************
*/
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <regex>
using namespace std;

    void tokenizer(string line);
    void PrintStatistics();
    bool id_Or_num(string token);
    bool comment(string str);
    bool isComment;
    int No_tokens = 0 , No_errors = 0 , No_lines = 0 , No_col;

    // Specification of Token Types:
    regex Alphabet("([(]|[)]|[:]|[{]|[}]|[<]|[>]|[=]|[*]|[A-Za-z]|[0-9]|[ ])");
    regex CompositeTokens("([:][=])|([>][=])|([<][=])|([<][>])");
    regex SimpleTokens("([(]|[)]|[:]|[{]|[}]|[<]|[>]|[=]|[*])");
    regex letters("[A-Za-z]") , digits("[0-9]|[.]");

    fstream file;
    string line , token="" , ch;

int main() {

    file.open("text.txt" , ios::in);

    if(!file) 
        cout << "File is not found";
    else {
        while(!file.eof()){
            getline(file , line);
            isComment = comment(line);// Check if the line is comment.
            if(!isComment){// If the line is not comment then start search for tokens
                tokenizer(line);
            }
        }
    }
    PrintStatistics();
    file.close();
    return 0;
}

bool id_Or_num(string token){
    regex id("^[a-zA-Z][a-zA-Z0-9]*");
    regex digits("[0-9]+[.][0-9]+");
    if(regex_match(token.begin(),token.end(),id))
        return true;
    if(regex_match(token.begin(),token.end(),digits))
        return true;
    return false;
}

bool comment(string str){
    // comment (*.....*)
    regex commentOpenB("^[(][*](.*)") , commentCloseB("(.)*[*][)]$");
    // comment {.......}
    regex commentOpenB_("^[{](.)*") , commentCloseB_("(.)*[}]$");
    //----------------------------------------------------------------------
     smatch match;

    // chick if comment type 1 (*.....*)
    if(regex_search(str,match,commentOpenB)&&regex_search(str,match,commentCloseB)){
            No_lines++;
            return true;
    }
    // chick if comment (*.....*) is not complete
    else if(regex_search(str,match,commentOpenB) && !regex_search(str,match,commentCloseB)){
            cout << "\n***Error comment is not complete ***\n"<< str <<" at line "<<No_lines<<"\n\n"<<string(80,'*');
            No_lines++;
            return true;
            }
    //----------------------------------------------------------------------


    // chick if comment type 2 {.......}
    if(regex_search(str,match,commentOpenB_)&&regex_search(str,match,commentCloseB_)){
            No_lines++;
            return true;
            }
    // chick if comment  {.......} is not complete
    else if(regex_search(str,match,commentOpenB_) && !regex_search(str,match,commentCloseB_)){
            cout << "\n***Error comment is not complete ***\n"<< str<<" at line "<<No_lines<<"\n\n"<<string(80,'*');
            No_lines++;
            return true;
    }
    //----------------------------------------------------------------------
    // if the previous conditions are false then it is not comment and the function will return false
    return false;
}

void tokenizer(string line){
    for(int i=0 ; i < line.size(); i++){
           ch = line[i];
           No_col = i;

             //is the character (letter or digit) ?
           if(regex_match(ch,letters) || regex_match(ch,digits)){
                while(regex_match(ch,letters) || regex_match(ch,digits)){
                    token += line[i];
                    i++;
                    ch = line[i];
                }
                if(id_Or_num(token)){// is token id or number? if not then it is Wrong identifier
                  No_col++;
                  No_tokens++;
                  cout <<"\tToken # "<<No_tokens<<"= '"<<token<<"'  at line "<<No_lines<<" col "<<No_col<<"\n\n";
                }
                else {
                  cout <<"Error # "<<No_errors<<" **Error at Line "<<No_lines<<", col "<<No_col <<"Wrong identifier** = "<<token<<"\n\n";
                  No_errors++;
                }
           }
            No_col = i ;
            token="";
            //-------------------------------------------
            // chick if the Character is unknown
            if(!regex_match(ch,Alphabet) && line[i] != '\0'){
                No_errors++;
                No_col++;
                token += line[i];
                cout <<string(80,'*')<<endl;
                cout <<"Error # "<<No_errors<<" **Error at Line "<<No_lines<<", col "<<No_col<<" Unknown Character** = "<<token<<"\n\n";
                cout <<string(80,'*')<<endl;
            }
            token="";
            //-------------------------------------------
            // chick if Character is simple tokens or composite tokens.
            /* if it's simple token then chick if it's composite token or not
               if not then it's composite token.*/
           if(regex_match(ch,SimpleTokens)){
               ch += line[i + 1];
               if(regex_match(ch,CompositeTokens)){
                    token += ch;
                    No_tokens++;
                    No_col++;
                    cout <<"\tToken # "<<No_tokens<<"= '"<<token<<"'  at line "<<No_lines<<" col "<<No_col<<"\n\n";
                    i++;
               }
               else {
                   token += line[i];
                   No_tokens++;
                   No_col++;
                   cout <<"\tToken # "<<No_tokens<<"= '"<<token<<"'  at line "<<No_lines<<" col "<<No_col<<"\n\n";
                   }
           }
           token="";
       //-------------------------------------------
    }// END for
    No_lines++;
}

void PrintStatistics(){
    cout<<"*** No of lines scanned = "  <<No_lines<<"\n\n";
    cout<<"*** No Of Errors found = "   <<No_errors<<" ***\n\n";
    cout<<"*** No Of Tokens found = "   <<No_tokens<<" ***\n\n";
}
