#include <iostream>
#include <fstream>

using namespace std;

class string_t {
    public:
        string_t(int size = 105);   // make everything change in size dynamically
        friend string_t operator + (const string_t &, const string_t &);
        friend istream & operator >> (istream &in, string_t &);
        void operator = (const string_t & otherString);
        friend string_t operator - (const string_t &, const string_t &);
        friend string_t operator * (const string_t &, const string_t &);
        friend string_t* operator /(const string_t &, const string_t &);
        friend string_t operator % (const string_t &, const string_t &);
        friend ostream & operator << (ostream & out, string_t &);
    // private:
        int size;
        char* strArr;
        char oper;
};

void string_t::operator = (const string_t & otherString) {
    int counter = 0;
    for (int i = 0; i < otherString.size; i++) {                // performing deep copy
        this->strArr[i] = otherString.strArr[i];
        ++counter;
    }
    this->size = counter;                                       // updating size of array
}

ostream & operator << (ostream & out, string_t & s) {
    for (int i = 0; i < s.size; i++) {
        out << s.strArr[i];
    }
    out << '\n';
    return out;
}

string_t operator +(const string_t & s1, const string_t & s2) {
    int newsize = s1.size + s2.size;        // find size of s1 + s2
    
    string_t newString(newsize);            // creates object with dynamic array of the following size
    // store combnation of s1 and s2 into that object
    for (int i = 0; i < s1.size; i++) {
        newString.strArr[i] = s1.strArr[i];
        // cout << newString.strArr[i] << endl;
    }
    for (int i = 0; i < s2.size; i++) {
        newString.strArr[i + s1.size] = s2.strArr[i];
        // cout << newString.strArr[i + s1.size] << endl;
    }
    
    return newString;
}

istream & operator >> (istream &in, string_t & s) {
    int counter = 0;
    // will read and store string info until it sees a space, newline, or eof
    // space case is between words ie banana - ana
    // newline case is at end of line ie - ana
    // eof case is at the very end of the file
    while (!(in.peek() == ' ' || in.peek() == '\n' ||in.peek() == EOF)) {
        in >> s.strArr[counter];
        ++counter;
        if (counter > s.size) {
            s.size = counter;
        }
    }
    if (in.peek() == ' ') {
        in.ignore();
    }   
    // storing operator associated with the first argument
    if (in.peek() == '+' || in.peek() == '-' || in.peek() == '*' || in.peek() == '/' || in.peek() == '%' ) {
        in >> s.oper;
        cin.ignore();               // ignoring the next character after the operator
    }
    s.size = counter;               // setting new size
    return in;
}

string_t operator - (string_t & s1, string_t & s2) {
    string_t result;
    for (int i = 0; i < s1.size; i++) {
        bool substring = true;
        for (int j = 0; j < s2.size; j++) {
            if (s2.strArr[j] != s1.strArr[i+j]) {
                substring = false;
            }
        }
        if (substring == true) {
            for (int j = i; j < s1.size-s2.size; j++) {
                s1.strArr[j] = s1.strArr[j+s2.size];
            }
            s1.size -= s2.size;
            break;
        }
    }
    result = s1;
    return result;
}

string_t operator * (string_t & s1, const string_t & s2) {
    string_t result;
    int oldsize = s1.size;
    s1.size *= s2.strArr[0] - '0';                     // converting char* to int
    result.size = s1.size;
    for (int i = 0; i < s1.size; i+=oldsize){ // incrementing by previous size of array
        int k = 0;
        for (int j = oldsize; j > 0; j--) {
            result.strArr[i + (k++)] = s1.strArr[oldsize -j];
        }
    }
    // result = s1;                                    // since = is overloaded, stores a deep copy of s1 into result
    return result;
}

string_t* operator / (const string_t & s, const string_t & s2) {
    int chars = s.size / (s2.strArr[0] - '0');              // converts to an integer
    int remain = s.size % (s2.strArr[0] - '0');             // make this be able to accept 2 and 3 digit values too
    int numArrs = (s2.strArr[0] - '0');                     
    string_t * result = new string_t[numArrs];
    // a0 = {b, a}
    // a1 = {n, a}
    // a2 = {n, a}

    // b a n a n a
    // 0 1 2 3 4 5

    // cout << "Chars in each array: " << chars << endl;
    // cout << "Remainder: " << remain << endl;                        
    // cout << "numArrs: " << numArrs << endl;                         

    for (int i = 0; i < numArrs; i++) {
        int k = i*chars;
        for (int j = 0; j < chars; j++) {
            result[i].size = chars;
            result[i].strArr[j] = s.strArr[k++];
        }
    }
    return result;
}

string_t operator % (const string_t &s, const string_t & s2) {

    // p o t a t o e / 3 (should output e)
    // 0 1 2 3 4 5 6
    // s.size = 7

    int chars = s.size / (s2.strArr[0] - '0');
    int remain = s.size % (s2.strArr[0] - '0');                  
    int numArrs = (s2.strArr[0] - '0');
    int startingIndex = chars * numArrs;
    string_t result(s.size - startingIndex);
    result.size = s.size - startingIndex;
    int k = 0;

    for (int i = startingIndex; i < s.size; i++) {
        result.strArr[k++] = s.strArr[i];
    }
    return result;
}

string_t::string_t(int size) {
    this->size = size;
    strArr = new char[size];
    oper = ' ';
}

int size (const string_t & s) {
    return s.size;
}

// returns the number of operators per line

int main () {
    string_t s[100];
    ofstream outFile("output.txt");
    outFile << ""; // clearing the outfile
    
    int k = 0;

    while (cin.peek()!= EOF){
        int wordCounter = 0;
        bool division = true;
        string_t * divideOper;
        while (cin.peek() != '\n' && cin.peek() != EOF) {
            cin >> s[wordCounter++];                              // inputs a new word
                                                                  // increments number of words or numbers (in the row)                                 
        }
        for (int i = 0; i < wordCounter-1; i++) {
            division = false;
            cout << "size of " << i << " element: " << s[i].size << endl;
            if (s[i].oper != ' ') { // if its operator is not ' ' (which is the default operator), we go into a case statement to look for that operation
                switch (s[i].oper) {
                    case '+':
                    // cout << "plus" << endl;
                        s[i+1] = (s[i] + s[i+1]);
                        break;
                    case '-':
                    // cout << "minus" << endl;
                        s[i+1] = (s[i] - s[i+1]);
                        break;
                    case '*':
                        // cout << "multiplication" << endl;
                        s[i+1] = (s[i] * s[i+1]);
                        break;
                    case '/':
                    cout << "division" << endl;
                    division = true;
                        // s[i] / s[i+1];
                        // since this will always be the last thing, we can cout << here
                        for (int j = 0; j < (s[i+1].strArr[0] - '0'); j++) {
                            for (int k = 0; k < s[i].size / (s[i+1].strArr[0] - '0'); k++) {
                                  outFile << ((s[i] / s[i+1])+j)->strArr[k];
                            }
                            outFile << endl;
                        }
                        break;
                    case '%':
                    cout << "modulus" << endl;
                        s[i+1] = (s[i] % s[i+1]);
                        break;
                }
            }
        }
        if (division) {
            cout << "CIN INSIDE DIV PEEK: " << cin.peek() << endl << endl;
            if (cin.peek() == '\n') cin.ignore();                       // moves beyond the newline, looking at next word
            continue;                                                   // skips last part
        }
    
        outFile << s[wordCounter-1];
        
        if (cin.peek() == '\n') cin.ignore();                       // moves beyond the newline, looking at next word

    }

// how to cout the / operator
        // for (int i = 0; i < 4; i++) {
    //     for (int j = 0; j < 1; j++) {
    //         cout << ((s1/4)+i)->strArr[j];
    //     }
    //     cout << endl;
    // }

//     for (int i = 0; i < s[0].size; i++) {
//         cout << s[0].strArr[i];
//     }
//     cout << endl;
//     for (int i = 0; i < s[1].size; i++) {
//         cout << s[1].strArr[i];
//     }

// cout << endl; 

//     s[1] = (s[0] * s[1]); // t + potatoe
//        for (int i = 0; i < s[1].size; i++) {
//         cout << s[1].strArr[i];
//     }
//     cout << endl;

    // for (int i = 0; i < 4; i++) {
    //     for (int j =0; j < s[i].size; j++) {
    //         cout << s[i].strArr[j];
    //     }
    //     cout << endl;
    // }



    // cout << "PEEK: "<< (char)cin.peek() << "kjashdf" << endl;

    // while (cin.peek() != '\n') {
    //     cout << "1" <<endl;
    //     cin >> s[i++];
    // }

    // for (int j = 0; j < i; j++) {
    //     for (int k = 0; k < s[j].size; j++) {
    //         cout << s[j].strArr[k];
    //     }
    //     cout << endl;
    // }


    


    // how to cout the % operator
    // for (int i = 0; i < (s1%4).size; i++) {
    //     cout << (s1%4).strArr[i];
    // }

    // cout << s1 << endl;
   
    return 0;
}
