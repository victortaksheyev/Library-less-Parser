#include <iostream>
#include <fstream>

using namespace std;

class string_t {
    public:
        string_t(int size = 2);   // make everything change in size dynamically
        friend string_t operator + (const string_t &,  string_t &);
        friend istream & operator >> (istream &in, string_t &);
        void operator = (const string_t & otherString);
        friend string_t operator - (const string_t &,  string_t &);
        friend string_t operator * (const string_t &,  string_t &);
        friend string_t* operator /(const string_t &, const string_t &);
        friend string_t operator % (const string_t &,  string_t &);
        friend ostream & operator << (ostream & out, string_t &);
    
        int size;
        char* strArr;
        char oper;
};

string_t::string_t(int size) {
    this->size = size;
    strArr = new char[size];
    oper = ' ';
}

void string_t::operator = (const string_t & otherString) {
    for (int i = 0; i < otherString.size; i++) {                // performing deep copy
        this->strArr[i] = otherString.strArr[i];
    }                                   
}

ostream & operator << (ostream & out, string_t & s) {
    for (int i = 0; i < s.size; i++) {
        out << s.strArr[i];
    }
    out << '\n';
    return out;
}

string_t operator +(const string_t & s1,  string_t & s2) {
    int newsize = s1.size + s2.size;        // find size of s1 + s2
    // cout << "SIZE: " << newsize;
    
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

    delete [] s2.strArr;
    s2.strArr = new char[newsize];
    s2 = newString;
    s2.size = newsize;

    return s2;
}

istream & operator >> (istream &in, string_t & s) {
    int letters = 0;
    string_t temp(1);
    delete [] temp.strArr;
    // will read and store string info until it sees a space, newline, or eof
    // space case is between words ie banana - ana
    // newline case is at end of line ie - ana
    // eof case is at the very end of the file
    while (!(in.peek() == ' ' || in.peek() == '\n' ||in.peek() == EOF)) {
        in >> s.strArr[letters];
        ++letters;
        cout << letters << " ";
        
        // 2 > 1
        if (letters > s.size) {
            // string_t temp(s.size);          // creating a temporary object
            // delete[] temp.strArr;
            temp.strArr = new char[s.size];
            temp.size = s.size;
            temp = s;                       // deep copying s into temp
            delete [] s.strArr;             // deleting old (small) string
            s.strArr = new char[letters];   // creating new (larger) string
            s.size = temp.size+1;           // making it larger by 1

            s = temp;
            delete [] temp.strArr;
        }
    }
    if (in.peek() == ' ') {
        in.ignore();
    }   
    // cout <<"PEEK: "<< (char)in.peek() << endl;
    // storing operator associated with the first argument
    if (in.peek() == '+' || in.peek() == '-' || in.peek() == '*' || in.peek() == '/' || in.peek() == '%' ) {
        in >> s.oper;
        cin.ignore();               // ignoring the next character after the operator
    }
    return in;
}

string_t operator - (const string_t & s1,  string_t & s2) {
    // string_t result;
    // result.strArr = new char[s1.size];
    bool substring;
    // cout << "String 1 size: " << s1.size << endl;
    // cout <<" string 2 size: " << s2.size << endl;

// cout << "REAL DEAL" << endl;
    for (int i = 0; i < s1.size; i++) {
        bool substring = true;
    
        // cout << "SIZE:"<< s2.size << endl;
        for (int j = 0; j < s2.size; j++) {

            // cout << s2.strArr[j] <<  " " << s1.strArr[i+j];
            // cout << endl;
            if (s2.strArr[j] != s1.strArr[i+j]) {
                substring = false;
                continue;
            }
        }
        // cout << "index: " << i << " status: " << substring << endl; 
        if (substring == true) {  
            for (int j = i; j < s1.size-s2.size; j++) {
                s1.strArr[j] = s1.strArr[j+s2.size];
            }

            // creating the final array
            delete [] s2.strArr;
            s2.strArr = new char[s1.size - s2.size];
            s2 = s1;
            s2.size = s1.size - s2.size;
            return s2;
            break;
        }
    }
    //17 is diff
    // cout << "True or false: " << substring<< endl;
    if (substring == false) {
        cout << "Substring printed" << endl;
        delete [] s2.strArr;            // deleting old size
        s2.size = s1.size;                  // setting new size
        s2.strArr = new char[s2.size];  // creating array of that size
        
 
        // cout << "s2.size:" << s2.size << endl;
        for (int i = 0; i < s1.size; i++) {
            s2.strArr[i] = s1.strArr[i];
            // cout << s1.strArr[i] << endl;
        }
        // s2 = s1;                        // deep copy
        for (int i = 0; i < s1.size; i++) {
            cout << s2.strArr[i];
        }
        cout << endl;
    }

    // cout << endl;
    return s2;                  // if there is no match, result = s1
}

string_t operator * (const string_t & s1,  string_t & s2) {

    int oldsize = s1.size;
    // cout << "OLDSIZE: " <<s1.size << endl;
    s2.size = s1.size * ( s2.strArr[0] - '0');                     // converting char* to int
    // cout << "s2 size: " << s2.size << endl;
    // cout << "NEWSIZE: " <<s1.size << endl;
    // string_t result(s1.size); // 21
    // result.size = s1.size;
    delete [] s2.strArr;
    s2.strArr = new char[s2.size];
    for (int i = 0; i < s2.size; i+=oldsize){ // incrementing by previous size of array
        for (int j = 0; j < oldsize; j++) {         // creating deep copy
            s2.strArr[i + (j)] = s1.strArr[j];
            // cout << result.strArr[i + (j)];
        }
    }

    return s2;
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
            delete [] result[i].strArr;
            result[i].strArr = new char[result[i].size];
            result[i].strArr[j] = s.strArr[k++];
        }
    }
    return result;
}

string_t operator % (const string_t &s,  string_t & s2) {

    // p o t a t o e / 3 (should output e)
    // 0 1 2 3 4 5 6
    // s.size = 7

    int chars = s.size / (s2.strArr[0] - '0');
    int remain = s.size % (s2.strArr[0] - '0');                  
    int numArrs = (s2.strArr[0] - '0');
    int startingIndex = chars * numArrs;
    // string_t result(s.size - startingIndex);
    // result.size = s.size - startingIndex;
    s2.size = s.size - startingIndex;
    delete [] s2.strArr;
    s2.strArr = new char[s2.size];
    int k = 0;
    for (int i = startingIndex; i < s.size; i++) {
        s2.strArr[k++] = s.strArr[i];
    }
    return s2;
}

int size (const string_t & s) {
    return s.size;
}

// returns the number of operators per line

int main () {
    
    ofstream outFile("output.txt");
    outFile << ""; // clearing the outfile
    
    int k = 0;

    while (cin.peek()!= EOF){
        string_t s[15];
        int wordCounter = 0;
        bool division = true;
        int num;
        string_t * divideOper;
        string_t temp;

        while (cin.peek() != '\n' && cin.peek() != EOF) {
            cin >> s[wordCounter++];                              // inputs a new word
                                                                  // increments number of words or numbers (in the row)                                 
        }
        // cout << "Printing from the main portion" << endl;
            // for (int i = 0; i < s[1].size; i++) {
            // s2.strArr[i] = s1.strArr[i];
            // cout << s[1].strArr[i] << endl;
        // }
   
        for (int i = 0; i < wordCounter-1; i++) {
            division = false;
            // string_t better(10);
            // cout << "size of " << i << " element: " << s[i].size << endl;
            if (s[i].oper != ' ') { // if its operator is not ' ' (which is the default operator), we go into a case statement to look for that operation
                switch (s[i].oper) {
                    case '+':
                        s[i] + s[i+1];
                        break;
                    case '-':
                        s[i]-s[i+1];

                        break;
                    case '*':
                        s[i]*s[i+1];
                        
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
                        (s[i] % s[i+1]);
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
   
    return 0;
}