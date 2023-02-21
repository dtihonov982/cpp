#include <iostream>
#include <ctime>
#include <time.h>
#include <iomanip>

using namespace std;

class entry {
private:
    time_t date;
    string dt;
    string ct;
    double amount;
    string description;

public:
    entry() {

    }
    entry(time_t d, string debit, string credit, double s, string descr = "") {
        date = d;
        dt = debit;
        ct = credit;
        amount = s;
        description = descr;
    }

    time_t get_date() {
        return date;
    }

    string get_dt() {
        return dt;
    }

    string get_ct() {
        return ct;
    }

    double get_amount() {
        return amount;
    }

    string get_description() {
        return description;
    }

    void set_date(time_t d) {
        date = d;
    }

    void set_dt(string a) {
        dt = a;
    }

    void set_ct(string a) {
        ct = a;
    }

    void set_amount(double x) {
        amount = x;
    }

    void set_description(string d) {
        description = d;
    }

    void setDateByString(string dateString, string format = "%d.%m.%Y") {
        tm tm;
        istringstream ss(dateString);
        ss >> get_time(&tm, format.c_str()); // or just %T in this case
        date = mktime(&tm);
    }

    /*
    void input() {
        cout << "date: ";
        cin >> date;
        cout << "debit: ";
        cin >> dt;
        cout << "credit: ";
        cin >> ct;
        cout << "amount: ";
        cin >> amount;
        cout << "description: ";
        //!Error
        cin >> description;
    }
    */

    void print() {
        cout << date << "\t" << dt << "\t" << ct << "\t" << amount << "\t" << description << endl;
    }
};

class journal {
private:
    static const int MAX_LENGTH = 20;
    entry entries[MAX_LENGTH];
    int length = 0;

public:
    void append(entry e) {
         if (length < MAX_LENGTH) {
            entries[length] = e;
            length++;
        } else{
            throw "The journal doesn't have empty space!";
        }
    }

    //stream param
    /*
    void print() {
        for (int i = 0; i < length; i++) {
            entry e = entries[i];
            cout << e.get_date() << "\t" << e.get_dt() << "\t" << e.get_ct() << "\t" << e.get_amount() << "\t" << e.get_description() << endl;
        }
    }
    */

};

//https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
std::vector<std::string> getNextLineAndSplitIntoTokens(std::istream& str)
{
    std::vector<std::string>   result;
    std::string                line;
    std::getline(str,line);

    std::stringstream          lineStream(line);
    std::string                cell;

    while(std::getline(lineStream,cell, ','))
    {
        result.push_back(cell);
    }
    // This checks for a trailing comma with no data after it.
    if (!lineStream && cell.empty())
    {
        // If there was a trailing comma then add an empty element.
        result.push_back("");
    }
    return result;
}



int main() {
    /*
    journal Journal;



    Journal.append(e);

    entry e2("23/01/2023", "60", "51", 50.00);

    Journal.append(e2);

    Journal.append( entry("24/01/2023", "41", "60", 50.00) );

    Journal.print();
    */

    entry e;
    e.setDateByString("21.01.2023");
    e.set_dt("51");
    e.set_ct("80");
    e.set_amount(100.00);
    e.set_description("Creating of the company.");

    return 0;
}

