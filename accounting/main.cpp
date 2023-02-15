#include <iostream>

using namespace std;

class entry {
private:
    string date;
    string dt;
    string ct;
    double amount;
    string description;

public:
    entry() {

    }
    //!Can I use same name as attributes?
    entry(string d, string debit, string credit, double s, string descr = "") {
        date = d;
        dt = debit;
        ct = credit;
        amount = s;
        description = descr;
    }

    string get_date() {
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

    void set_date(string d) {
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
    void print() {
        for (int i = 0; i < length; i++) {
            entry e = entries[i];
            cout << e.get_date() << "\t" << e.get_dt() << "\t" << e.get_ct() << "\t" << e.get_amount() << "\t" << e.get_description() << endl;
        }
    }

};

class TableOfAccounts {

};

class AccountingSystem {
private:
    journal Journal;
    TableOfAccounts accounts;

};



int main() {
    journal Journal;

    entry e;
    e.set_date("21/01/2023");
    e.set_dt("51");
    e.set_ct("80");
    e.set_amount(100.00);
    e.set_description("Creating of the company.");

    Journal.append(e);

    entry e2("23/01/2023", "60", "51", 50.00);

    Journal.append(e2);

    Journal.append( entry("24/01/2023", "41", "60", 50.00) );

    Journal.print();


    return 0;
}

