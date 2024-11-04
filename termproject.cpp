#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdexcept>
using namespace std;

/* �ռ� UI ���� */

class Account;
class Bank;
class Single_ATM;
class Multi_ATM;
class Trans_History;


class Trans_History {
private:
    static long long id_counter;
    long long trans_id;
    long long cardnumber;
    string accountnumber;
    string trans_type;
    long long amount;
    string other_account;
public:
    long long get_trans_id() { return trans_id; }
    long long get_cardnumber() { return cardnumber; }
    string get_accountnumber() { return accountnumber; }
    string get_trans_type() { return trans_type; }
    long long get_amount() { return amount; }
    string get_other_acc() { return other_account; }
    Trans_History(long long cnum, string anum, string type, long long money, string othacc) {
        cardnumber = cnum;
        accountnumber = anum;
        trans_type = type;
        amount = money;
        other_account = othacc;
        trans_id = ++id_counter;
    }
};

class Account {
private:
    Bank* bank; // ���¿� ������ ����
    string acc_num; // ���¹�ȣ 12�ڸ�
    long long password; // ��й�ȣ
    long long Acash; // ���º�����
    long long card_num; // ������ ī���ȣ
    string user_name; // ���� ����� �̸�
public:
    Account(Bank* bname, string anum, long long pw, long long ac, long long cn, string un);
    void update_cash(long long inoutcash) { Acash += inoutcash; }; // ���º����� ����
    long long get_password() { return password; }; // ��й�ȣ ��ȯ
    Bank* get_bank() { return bank; }; // ���� ��ȯ
    string get_accountnum() { return acc_num; }; //���¹�ȣ ��ȯ
    long long get_cash() { return Acash; }; //���� ���� ��ȯ
    long long get_card_num() { return card_num; } // ī�� ��ȣ ��ȯ
    string get_name() { return user_name; }
};

class Bank {
private:
    string bank_name; // ���� �̸�
    vector<Account*> account_lst; // ���� ���� ���� ���
    long long numOfuser = 0; // user account ����
    vector<Bank*> bank_list;
public:
    Bank(string bname) {
        bank_name = bname;
        bank_list.push_back(this);
    };

    void addBankToList(Bank* bank) {
        bank_list.push_back(bank);
    }
    string get_bank_name() { return bank_name; }; // ��ũ �̸� ��ȯ
    void add_accountlst(Account* new_account) { account_lst.push_back(new_account); }; // ���� ������ �߰�
    Account* find_account(long long card_num) {
        vector<Bank*>::iterator iter_bank;
        vector <Account*>::iterator iter_account;
        for (iter_bank = bank_list.begin(); iter_bank != bank_list.end(); ++iter_bank) {
            for (iter_account = (*iter_bank)->account_lst.begin(); iter_account != (*iter_bank)->account_lst.end(); ++iter_account) {
                if ((*iter_account)->get_card_num() == card_num) {
                    return (*iter_account);
                }
            }
        }
        return nullptr; // Move this line outside of the loop
    }
    Account* find_trans_account(string acc_num) {
        vector<Bank*>::iterator iter_bank;
        vector <Account*>::iterator iter_account;
        for (iter_bank = bank_list.begin(); iter_bank != bank_list.end(); ++iter_bank) {
            for (iter_account = (*iter_bank)->account_lst.begin(); iter_account != (*iter_bank)->account_lst.end(); ++iter_account) {
                if ((*iter_account)->get_accountnum() == acc_num) {
                    return (*iter_account);
                }
            }
        }
        return nullptr; // Move this line outside of the loop
    }
    Bank* find_bank(string b_name) {
        vector<Bank*>::iterator iter_bank;
        for (iter_bank = bank_list.begin(); iter_bank != bank_list.end(); ++iter_bank) {
            if ((*iter_bank)->get_bank_name() == b_name) {
                return (*iter_bank);
            }
        }
        return nullptr;
    }
    void display_account() {
        vector<Bank*>::iterator iter_bank;
        vector<Account*>::iterator iter_account;
        for (iter_bank = bank_list.begin() + 1; iter_bank != bank_list.end(); ++iter_bank) {
            for (iter_account = (*iter_bank)->account_lst.begin(); iter_account != (*iter_bank)->account_lst.end(); ++iter_account) {
                cout << "Account [Bank: " << ((*iter_account)->get_bank())->get_bank_name() << ", No: " << (*iter_account)->get_accountnum() << ", Owner: " << (*iter_account)->get_name() << "] balance: " << (*iter_account)->get_cash() << endl;
            }
        }
    }

    void set_banklist(vector<Bank*> list) {
        bank_list = list;
    }

    vector<Bank*> get_banklist() {
        return bank_list;
    }
};

Account::Account(Bank* bname, string anum, long long pw, long long ac, long long cn, string un) {
    bank = bname;
    acc_num = anum;
    password = pw;
    Acash = ac;
    card_num = cn;
    user_name = un;
    cout << "���»���) ���� �̸�: " << bname->get_bank_name() << ", ���¹�ȣ: " << anum << ", ��й�ȣ: " << pw << ", ���º�����: " << ac << ", ī�� ��ȣ: " << cn << ", ����� �̸�:" << un << endl;
}; // ���� �ʱ� ����

class Multi_ATM {
protected:
    Bank* main_bank; // �� ���� ����
    string ATM_serial; // ���� �ѹ� 6�ڸ�
    string lang_type{ 1 }; // ���� ���, ���� ���(�ѿ�)
    string lang_state;
    long long cash_1000; // ATM 1000��¥�� ����
    long long cash_5000; // ATM 5000��¥�� ����
    long long cash_10000; // ATM 10000��¥�� ����
    long long cash_50000; // ATM 50000��¥�� ����
    long long p_deposit_fee;
    long long n_deposit_fee;
    long long p_withdrawal_fee;
    long long n_withdrawal_fee;
    long long pp_transfer_fee;
    long long pn_transfer_fee;
    long long nn_transfer_fee;
    long long cash_transfer_fee;
    static long long withdrawal_num;
    vector<Multi_ATM*> ATM_list;
    vector<Trans_History> history_lst;
    vector<Trans_History> session_lst;

public:
    Multi_ATM(Bank* mbank, string sernum, string lang, long long num_1000, long long num_5000, long long num_10000, long long num_50000, long long p_dep, long long n_dep, long long p_with, long long n_with, long long pp_trans, long long pn_trans, long long nn_trans, long long cash_trans) {
        main_bank = mbank;
        ATM_serial = sernum;
        lang_state = lang;
        cash_1000 = num_1000;
        cash_5000 = num_5000;
        cash_10000 = num_10000;
        cash_50000 = num_50000;
        p_deposit_fee = p_dep;
        n_deposit_fee = n_dep;
        p_withdrawal_fee = p_with;
        n_withdrawal_fee = n_with;
        pp_transfer_fee = pp_trans;
        pn_transfer_fee = pn_trans;
        nn_transfer_fee = nn_trans;
        cash_transfer_fee = cash_trans;
        static long long withdrawal_num;
        ATM_list.push_back(this);
    }; // ATM �ʱ� ����

    void addATMToList(Multi_ATM* atm) {
        ATM_list.push_back(atm);
    }

    void set_atmlist(vector<Multi_ATM*> atm) {
        ATM_list = atm;
    }

    vector<Multi_ATM*> get_atmlist() {
        return ATM_list;
    }


    Bank* get_ATMBank() { return main_bank; } // ATM�� primiary bank ��ȯ
    string get_serial() { return ATM_serial; } // ATM�� �Ϸù�ȣ ��ȯ

    Multi_ATM* find_atm(string num) {
        vector<Multi_ATM*>::iterator iter_atm;
        for (iter_atm = ATM_list.begin(); iter_atm != ATM_list.end(); ++iter_atm) {
            if ((*iter_atm)->get_serial() == num) {
                return (*iter_atm);
            }
        }
        return nullptr;
    }
    long long get_remaining_cash() { return 50000 * cash_50000 + 10000 * cash_10000 + 5000 * cash_5000 + 1000 * cash_1000; }
    void display_ATM() {
        vector<Multi_ATM*>::iterator iter_ATM;
        vector <Account*>::iterator iter_account;
        for (iter_ATM = ATM_list.begin() + 1; iter_ATM != ATM_list.end(); ++iter_ATM) {
            cout << "ATM [Bank: " << (*iter_ATM)->get_ATMBank()->get_bank_name() << ", SN: " << (*iter_ATM)->get_serial() << "] remaining cash : " << (*iter_ATM)->get_remaining_cash() << " (50000: " << (*iter_ATM)->cash_50000 << ", 10000 : " << (*iter_ATM)->cash_10000 << ", 5000 : " << (*iter_ATM)->cash_5000 << ", 1000 : " << (*iter_ATM)->cash_1000 << ")" << endl;
        }
    }

    void start_ATM() {
        if (lang_state == "Uni") {
            lang_type = "1";
        }
        else if (lang_state == "Bi") {
            choicelang();
            atmui_e();
            cin >> lang_type;
            while (lang_type == "99") {           //99
                display_ATM();
                main_bank->display_account();
                choicelang();
                atmui_e();
                cin >> lang_type;
            }
        }
        insert_card();
    }
    virtual void insert_card() {
        string card_num;
        hello(lang_type);
        if (lang_type !="1"){
            if (lang_type != "2") {
                return;
            }
            }
        cin >> card_num;
        while (card_num == "99") {           //99
            display_ATM();
            main_bank->display_account();
            hello(lang_type);
            cin >> card_num;
        }
        if (card_num == ATM_serial) {        //admin
            admin(lang_type);
            long long ans;
            cin >> ans;
            while (ans == 99) {           //99
                display_ATM();
                main_bank->display_account();
                admin(lang_type);
                cin >> ans;
            }
            if (ans == 1) {
                print_all_session();
                return;
            }
            else { 
                return; }
        }

        Account* acc = main_bank->find_account(stoll(card_num));
        if (acc == nullptr) {
            notcard(lang_type);
            return;
        }
        enter_password(acc);
    }
    void enter_password(Account* acc) { // ��й�ȣ �Է� ȭ��
        long long password;
        long long wrong_time = 0;

        while (true) {
            pw(lang_type);
            cin >> password;
            while (password == 99) {           //99
                display_ATM();
                main_bank->display_account();
                pw(lang_type);
                cin >> password;
            }
            if (password != acc->get_password()) {
                wrong_time++;
                if (wrong_time == 3) {
                    outnumber(lang_type);
                    return;
                }
                wrongpw(lang_type);
            }
            else {
                mainstage(acc);
                break;
            }
        }
    }
    void mainstage(Account* acc) { // Transaction Main
        totalmenu(lang_type);
        long long input;
        cin >> input;
        while (input == 99) {           //99
            display_ATM();
            main_bank->display_account();
            totalmenu(lang_type);
            cin >> input;
        }
        if (input == 1) {
            depositmenu(lang_type);
            deposit(acc);
            return;
        }
        else if (input == 2) {
            if (withdrawal_num >= 3) {
                w3(lang_type);
                mainstage(acc);
                return;
            }
            withdrawal(lang_type);
            withdrawal(acc);
            return;
        }
        else if (input == 3) {
            transfermenu(lang_type);
            transfer(acc);
            return;
        }
        else if (input == 4) {
            withdrawal_num = 0;
            finish(lang_type);
            print_all_history();
            return;
        }
        else {
            invalidnum(lang_type);
            mainstage(acc);
        }
    }
    void deposit(Account* acc) { // ���� ��� ����
        long long deposit_method;
        cin >> deposit_method;
        while (deposit_method == 99) {           //99
            display_ATM();
            main_bank->display_account();
            depositmenu(lang_type);
            cin >> deposit_method;
        }
        if (deposit_method == 1) {
            cash_deposit(acc);
            return;
        }
        else if (deposit_method == 2) {
            check_deposit(acc);
            return;
        }
        else if (deposit_method == 3) {
            mainstage(acc);
            return;
        }
        else if (deposit_method == 4) {
            withdrawal_num = 0;
            return;
        }
        else {
            invalidnum(lang_type);
            depositmenu(lang_type);
            deposit(acc);
            return;
        }
    };
    void cash_deposit(Account* acc) {
        long long num_50000, num_10000, num_5000, num_1000;
        if (main_bank == acc->get_bank()) {
            depwithfee(lang_type);
            cout << "50000: ";
            cin >> num_50000;
            cout << "10000: ";
            cin >> num_10000;
            cout << "5000: ";
            cin >> num_5000;
            cout << "1000: ";
            cin >> num_1000;
            if (num_50000 + num_10000 + num_5000 + num_1000 > 50) {
                banknotelimit(lang_type);
                mainstage(acc);
                return;
            }
            if(num_50000 + num_10000 + num_5000 + num_1000 == 0) {
                if (lang_type == "1") {
                    cout << "0 input is invalid" << endl;
                    mainstage(acc);
                    return;
                }
                cout << "0�� �Է��� �Ұ��մϴ�." << endl;
                mainstage(acc);
                return;
            }
            if (p_deposit_fee != 0) {
                long long fee_num;
                checkinputfee(lang_type, p_deposit_fee); //��
                cout << "1000: ";
                cin >> fee_num;
                if (fee_num * 1000 == p_deposit_fee) {
                    //                    acc->update_cash(check_cash * check_num);
                    cash_50000 += num_50000;
                    cash_10000 += num_10000;
                    cash_5000 += num_5000;
                    cash_1000 += (num_1000+fee_num);
                    acc->update_cash(50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000);
                    history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "cash_deposit", 50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000, "0"));
                    mainstage(acc);
                    return;
                }
                else {
                    invalidfee(lang_type);
                    mainstage(acc);
                    return;
                }
            }
            else {
                cash_50000 += num_50000;
                cash_10000 += num_10000;
                cash_5000 += num_5000;
                cash_1000 += num_1000;
                acc->update_cash(50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000);
                history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "cash_deposit", 50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000, "0"));
                mainstage(acc);
                return;
            }
        }
            
        else {
            depwithfee(lang_type);
            cout << "50000: ";
            cin >> num_50000;
            cout << "10000: ";
            cin >> num_10000;
            cout << "5000: ";
            cin >> num_5000;
            cout << "1000: ";
            cin >> num_1000;
            if (num_50000 + num_10000 + num_5000 + num_1000 > 50) {
                banknotelimit(lang_type);
                mainstage(acc);
                return;
            }
            if(num_50000 + num_10000 + num_5000 + num_1000 == 0) {
                if (lang_type == "1") {
                    cout << "0 input is invalid" << endl;
                    mainstage(acc);
                    return;
                }
                cout << "0�� �Է��� �Ұ��մϴ�." << endl;
                mainstage(acc);
                return;
            }
            if (n_deposit_fee != 0) {
                long long fee_num;
                checkinputfee(lang_type, n_deposit_fee); //��
                cout << "1000: ";
                cin >> fee_num;
                if (fee_num * 1000 == n_deposit_fee) {
                    cash_50000 += num_50000;
                    cash_10000 += num_10000;
                    cash_5000 += num_5000;
                    cash_1000 += (num_1000+fee_num);
                    acc->update_cash(50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000);
                    history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "cash_deposit", 50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000, "0"));
                    mainstage(acc);
                    return;
                }
                else {
                    invalidfee(lang_type);
                    mainstage(acc);
                    return;
                }
            }
            else {
                cash_50000 += num_50000;
                cash_10000 += num_10000;
                cash_5000 += num_5000;
                cash_1000 += num_1000;
                acc->update_cash(50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000);
                history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "cash_deposit", 50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000, "0"));
                mainstage(acc);
                return;
            }
        }
    }
    void check_deposit(Account* acc) {
        long long check_cash, check_num;
        if (main_bank == acc->get_bank()) {
            paper(lang_type);

            if (lang_type == "1") { cout << "The amount of a check: "; }
            else { cout << "��ǥ�� ������ �Է����ּ���: "; }
            cin >> check_cash;
            if (check_cash < 100000) {
                if (lang_type == "1") {
                    cout << "The amount of a check is above 100,000won " << endl;
                    mainstage(acc);
                    return;
                }
                else {
                    cout << "��ǥ�ݾ��� 100,000�� �̻��Դϴ�" << endl;
                    mainstage(acc);
                    return;
                }
            }
            if (lang_type == "1") { cout << "Check number: "; }
            else { cout << "��ǥ�� ���� �Է��� �ּ���: "; }
            cin >> check_num;
            if (check_num > 30) {
                checklimit(lang_type);
                mainstage(acc);
                return;
            }
            if (p_deposit_fee != 0) {
                long long fee_num;
                checkinputfee(lang_type, p_deposit_fee);
                cout << "1000: ";
                cin >> fee_num;
                if (fee_num * 1000 == p_deposit_fee) {
                    acc->update_cash(check_cash * check_num);
                    cash_1000 += fee_num;
                    history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "check_deposit", check_cash * check_num, "0"));
                    mainstage(acc);
                    return;
                }
                else {
                    invalidfee(lang_type);
                    mainstage(acc);
                    return;
                }
            }
            else {
                acc->update_cash(check_cash * check_num);
                history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "check_deposit", check_cash * check_num, "0"));
                mainstage(acc);
                return;
            }
        }
        else {
            if (lang_type == "1") { cout << "The amount of a check: "; }
            else { cout << "��ǥ�� ������ �Է����ּ���: "; }
            cin >> check_cash;
            if (check_cash < 100000) {
                if (lang_type == "1") {
                    cout << "The amount of a check is above 100,000won " << endl;
                    mainstage(acc);
                    return;
                }
                else {
                    cout << "��ǥ�ݾ��� 100,000�� �̻��Դϴ�" << endl;
                    mainstage(acc);
                    return;
                }
            }
            if (lang_type == "1") { cout << "Check number: "; }
            else { cout << "��ǥ�� ���� �Է��� �ּ���: "; }
            cin >> check_num;
            if (check_num > 30) {
                checklimit(lang_type);
                mainstage(acc);
                return;
            }
            if (n_deposit_fee != 0) {
                long long fee_num;
                checkinputfee(lang_type, n_deposit_fee);
                cout << "1000: ";
                cin >> fee_num;
                if (fee_num * 1000 == n_deposit_fee) {
                    cash_1000 += fee_num;
                    acc->update_cash(check_cash * check_num);
                    history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "check_deposit", check_cash * check_num, "0"));
                    mainstage(acc);
                    return;
                }
                else {
                    invalidfee(lang_type);
                    mainstage(acc);
                    return;
                }
            }
            else {
                acc->update_cash(check_cash * check_num);
                history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "check_deposit", check_cash * check_num, "0"));
                mainstage(acc);
                return;
            }
        }
    }
    void withdrawal(Account* acc) { // ���
        long long num_50000, num_10000, num_5000, num_1000;
        cout << "50000: ";
        cin >> num_50000;
        if (num_50000 > cash_50000) {
            atmnomoney(lang_type);
            mainstage(acc);
            return;
        }
        cout << "10000: ";
        cin >> num_10000;
        if (num_10000 > cash_10000) {
            atmnomoney(lang_type);
            mainstage(acc);
            return;
        }
        cout << "5000: ";
        cin >> num_5000;
        if (num_5000 > cash_5000) {
            atmnomoney(lang_type);
            mainstage(acc);
            return;
        }
        cout << "1000: ";
        cin >> num_1000;
        if (num_1000 > cash_1000) {
            atmnomoney(lang_type);
            mainstage(acc);
            return;
        }
        if (num_50000 + num_10000 + num_5000 + num_1000 == 0) {
            if (lang_type == "1") {
                cout << "0 input is invalid" << endl;
                mainstage(acc);
                return;
            }
            cout << "0�� �Է��� �Ұ��մϴ�" << endl;
            mainstage(acc);
            return;
        }
        if (main_bank == acc->get_bank()) {
            if (50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 > 500000) {
                withdrawlimit(lang_type);
                mainstage(acc);
                return;
            }
            if (50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 + p_withdrawal_fee > acc->get_cash()) {
                acc_nomoney(lang_type);
                mainstage(acc);
                return;
            }
            cash_50000 -= num_50000;
            cash_10000 -= num_10000;
            cash_5000 -= num_5000;
            cash_1000 -= num_1000;
            acc->update_cash(-(50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 + p_withdrawal_fee));
            history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "withdrawal", 50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000, "0"));
            withdrawal_num++;
            mainstage(acc);
            return;
        }
        else {
            if (50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 > 500000) {
                withdrawlimit(lang_type);
                mainstage(acc);
                return;
            }
            if (50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 + n_withdrawal_fee > acc->get_cash()) {
                acc_nomoney(lang_type);
                mainstage(acc);
                return;
            }
            cash_50000 -= num_50000;
            cash_10000 -= num_10000;
            cash_5000 -= num_5000;
            cash_1000 -= num_1000;
            acc->update_cash(-(50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 + n_withdrawal_fee));
            history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "withdrawal", 50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000, "0"));
            withdrawal_num++;
            mainstage(acc);
            return;
        }
    };
    void transfer(Account* acc) { // �۱�
        long long transfer_type;
        cin >> transfer_type;
        while (transfer_type == 99) {           //99
            display_ATM();
            main_bank->display_account();
            transfermenu(lang_type);
            cin >> transfer_type;
        }
        if (transfer_type == 1) {
            cash_transfer(acc);
            return;
        }
        else if (transfer_type == 2) {
            acc_transfer(acc);
            return;
        }
        else if (transfer_type == 3) {
            mainstage(acc);
            return;
        }
        else if (transfer_type == 4) {
            withdrawal_num = 0;
            return;
        }
        else {
            invalidnum(lang_type);
            transfermenu(lang_type);
            transfer(acc);
            return;
        }
    };
    void cash_transfer(Account* acc) {
        long long trans_acc_num;
        transferaccount(lang_type);
        cin >> trans_acc_num;
        while (trans_acc_num == 99) {           //99
            display_ATM();
            main_bank->display_account();
            transferaccount(lang_type);
            cin >> trans_acc_num;
        }
        string Trans_acc_num = to_string(trans_acc_num);
        Account* trans_acc = main_bank->find_trans_account(Trans_acc_num);
        if (trans_acc == nullptr) {
            inv_accnum(lang_type);
            mainstage(acc);
            return;
        }
        long long num_50000, num_10000, num_5000, num_1000;
        depwithfee(lang_type);
        cout << "50000: ";
        cin >> num_50000;
        cout << "10000: ";
        cin >> num_10000;
        cout << "5000: ";
        cin >> num_5000;
        cout << "1000: ";
        cin >> num_1000;
        if (num_50000 + num_10000 + num_5000 + num_1000 > 50) {
            banknotelimit(lang_type);
            mainstage(acc);
            return;
        }
        //
        if (num_50000 + num_10000 + num_5000 + num_1000 == 0) {
            if (lang_type == "1") {
                cout << "0 input is invalid" << endl;
                mainstage(acc);
                return;
            }
            cout << "0�� �Է��� �Ұ��մϴ�." << endl;
            mainstage(acc);
            return;
        }
        if (cash_transfer_fee != 0) {
            long long fee_num;
            checkinputfee(lang_type, cash_transfer_fee); //��
            cout << "1000: ";
            cin >> fee_num;
            if (fee_num * 1000 == cash_transfer_fee) {
                cash_50000 += num_50000;
                cash_10000 += num_10000;
                cash_5000 += num_5000;
                cash_1000 += (num_1000+fee_num);
                trans_acc->update_cash(50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000);
                history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "cash_transfer", 50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000, Trans_acc_num));
                mainstage(acc);
                return;
            }
            else {
                invalidfee(lang_type);
                mainstage(acc);
                return;
            }
        }
        else {
                cash_50000 += num_50000;
                cash_10000 += num_10000;
                cash_5000 += num_5000;
                cash_1000 += num_1000;
                trans_acc->update_cash(50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000);
                history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "cash_transfer", 50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000, Trans_acc_num));
                mainstage(acc);
                return;
        }
        //
    }
    void acc_transfer(Account* acc) {
        long long trans_acc_num;
        transferaccount(lang_type);
        cin >> trans_acc_num;
        while (trans_acc_num == 99) {           //99
            display_ATM();
            main_bank->display_account();
            transferaccount(lang_type);
            cin >> trans_acc_num;
        }
        string Trans_acc_num = to_string(trans_acc_num);
        Account* trans_acc = main_bank->find_trans_account(Trans_acc_num);
        if (trans_acc == nullptr) {
            inv_accnum(lang_type);
            mainstage(acc);
            return;
        }
        long long fund;
        transfermoney(lang_type);
        cin >> fund;
        if (fund == 0) {
            if (lang_type == "1") {
                cout << "0 input is invalid" << endl;
                mainstage(acc);
                return;
            }
                cout << "0�� �Է��� �Ұ��մϴ�." << endl;
                mainstage(acc);
                return;
          }
        if (acc->get_bank() == main_bank && trans_acc->get_bank() == main_bank) {
            if (fund + pp_transfer_fee > acc->get_cash()) {
                acc_nomoney(lang_type);
                mainstage(acc);
                return;
            }
            acc->update_cash(-(fund + pp_transfer_fee));
            history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "acc_transfer", fund, Trans_acc_num));
            trans_acc->update_cash(fund);
            mainstage(acc);
        }
        else if (acc->get_bank() != main_bank && trans_acc->get_bank() != main_bank) {
            if (fund + nn_transfer_fee > acc->get_cash()) {
                acc_nomoney(lang_type);
                mainstage(acc);
                return;
            }
            acc->update_cash(-(fund + nn_transfer_fee));
            trans_acc->update_cash(fund);
            history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "acc_transfer", fund, Trans_acc_num));
            mainstage(acc);
            return;
        }
        else {
            if (fund + pn_transfer_fee > acc->get_cash()) {
                acc_nomoney(lang_type);
                mainstage(acc);
                return;
            }
            acc->update_cash(-(fund + pn_transfer_fee));
            trans_acc->update_cash(fund);
            history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "acc_transfer", fund, Trans_acc_num));
            mainstage(acc);
            return;
        }
    }
    void print_history(Trans_History th) {
        cout << "transaction id (�ŷ� ��ȣ) : " << th.get_trans_id() << endl;
        cout << "card number (ī�� ��ȣ) : " << th.get_cardnumber() << endl;
        cout << "account number (���� ��ȣ) : " << th.get_accountnumber() << endl;
        cout << "transaction type (�ŷ� ����) : " << th.get_trans_type() << endl;
        cout << "transaction amount (�ŷ� �ݾ�) : " << th.get_amount() << endl;
        if (th.get_other_acc() != "0") {
            cout << "transfer account (��ü ��� ����) : " << th.get_other_acc() << endl;
        }
        cout << "\n";
    }
    void print_all_history() {
        for (long i = 0; i < history_lst.size(); i++) {
            print_history(history_lst[i]);
            session_lst.push_back(history_lst[i]);
        }
        history_lst.clear();
    }
    void print_all_session() {
        std::ofstream outputFile("output" + this->get_serial() + ".txt");
        for (long i = 0; i < session_lst.size(); i++) {
            cout << "transaction id (�ŷ� ��ȣ) : " << session_lst[i].get_trans_id() << endl;
            cout << "card number (ī�� ��ȣ) : " << session_lst[i].get_cardnumber() << endl;
            cout << "account number (���� ��ȣ) : " << session_lst[i].get_accountnumber() << endl;
            cout << "transaction type (�ŷ� ����) : " << session_lst[i].get_trans_type() << endl;
            cout << "transaction amount (�ŷ� �ݾ�) : " << session_lst[i].get_amount() << endl;
            if (session_lst[i].get_other_acc() != "0") {
               cout << "transfer account (��ü ��� ����) : " << session_lst[i].get_other_acc() << endl;
            }
            cout << "\n" << endl;
            outputFile << "transaction id (�ŷ� ��ȣ) : " << session_lst[i].get_trans_id() << endl;
            outputFile << "card number (ī�� ��ȣ) : " << session_lst[i].get_cardnumber() << endl;
            outputFile << "account number (���� ��ȣ) : " << session_lst[i].get_accountnumber() << endl;
            outputFile << "transaction type (�ŷ� ����) : " << session_lst[i].get_trans_type() << endl;
            outputFile << "transaction amount (�ŷ� �ݾ�) : " << session_lst[i].get_amount() << endl;
            if (session_lst[i].get_other_acc() != "0") {
                outputFile << "transfer account (��ü ��� ����) : " << session_lst[i].get_other_acc() << endl;
            }
            outputFile << "\n";

        }
        outputFile.close();
    }
    void choicelang() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                      Choose your language                                             I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                1. English              2. �ѱ���                                      I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void atmui_e() {
        cout << "I                                                                                                                 I" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I      ----------------------------              ------------------------------------------------------------     I" << endl;
        cout << "I      I                          I              I                                                          I     I" << endl;
        cout << "I      I  -----   -----   -----   I              I              Cash/Check Insert and Dispenser             I     I" << endl;
        cout << "I      I  I 1 I   I 2 I   I 3 I   I              I                                                          I     I" << endl;
        cout << "I      I  -----   -----   -----   I              ------------------------------------------------------------     I" << endl;
        cout << "I      I                          I                                                                               I" << endl;
        cout << "I      I  -----   -----   -----   I                                                                               I" << endl;
        cout << "I      I  I 4 I   I 5 I   I 6 I   I              ------------------------------------------------------------     I" << endl;
        cout << "I      I  -----   -----   -----   I              I                                                          I     I" << endl;
        cout << "I      I                          I              I                       Card Insert                        I     I" << endl;
        cout << "I      I  -----   -----   -----   I              I                                                          I     I" << endl;
        cout << "I      I  I 7 I   I 8 I   I 9 I   I              ------------------------------------------------------------     I" << endl;
        cout << "I      I  -----   -----   -----   I                                                                               I" << endl;
        cout << "I      I                          I                                                                               I" << endl;
        cout << "I      I  -----                   I              ------------------------------------------------------------     I" << endl;
        cout << "I      I  I 0 I     OK    Cancel  I              I                                                          I     I" << endl;
        cout << "I      I  -----                   I              I                      Receipt Print                       I     I" << endl;
        cout << "I      I                          I              I                                                          I     I" << endl;
        cout << "I      ----------------------------              ------------------------------------------------------------     I" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
    }
    void atmui_k() {
        cout << "I                                                                                                                 I" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I      ----------------------------              ------------------------------------------------------------     I" << endl;
        cout << "I      I                          I              I                                                          I     I" << endl;
        cout << "I      I  -----   -----   -----   I              I                    ����/��ǥ ����ݱ�                    I     I" << endl;
        cout << "I      I  I 1 I   I 2 I   I 3 I   I              I                                                          I     I" << endl;
        cout << "I      I  -----   -----   -----   I              ------------------------------------------------------------     I" << endl;
        cout << "I      I                          I                                                                               I" << endl;
        cout << "I      I  -----   -----   -----   I                                                                               I" << endl;
        cout << "I      I  I 4 I   I 5 I   I 6 I   I              ------------------------------------------------------------     I" << endl;
        cout << "I      I  -----   -----   -----   I              I                                                          I     I" << endl;
        cout << "I      I                          I              I                       ī�� ���Ա�                        I     I" << endl;
        cout << "I      I  -----   -----   -----   I              I                                                          I     I" << endl;
        cout << "I      I  I 7 I   I 8 I   I 9 I   I              ------------------------------------------------------------     I" << endl;
        cout << "I      I  -----   -----   -----   I                                                                               I" << endl;
        cout << "I      I                          I                                                                               I" << endl;
        cout << "I      I  -----                   I              ------------------------------------------------------------     I" << endl;
        cout << "I      I  I 0 I    Ȯ��   ���    I              I                                                          I     I" << endl;
        cout << "I      I  -----                   I              I                      ������ ���౸                       I     I" << endl;
        cout << "I      I                          I              I                                                          I     I" << endl;
        cout << "I      ----------------------------              ------------------------------------------------------------     I" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
    }
    void hello(string lang) {
        if (lang == "1") {
            hello_e();
            atmui_e();
        }
        else if (lang == "2") {
            hello_k();
            atmui_k();
        }
        else { 
               invalidnum("1");
               return;
            
        }
    }

    void hello_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                            �ݰ����ϴ�.                                                I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                  �ŷ��� �����ϱ� ���� ī�带 �־��ּ���.                              I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void hello_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                Welocme                                                I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                  To start, please insert your debit card                              I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void notcard(string lang) {
        if (lang == "1") {
            notcard_e();
            atmui_e();
        }
        else if (lang == "2") {
            notcard_k();
            atmui_k();
        }
    }
    void pw_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                   Write down your password.                                           I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void pw_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                      ��й�ȣ�� �Է��ϼ���.                                           I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void notcard_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                        Invalid cardinput                                              I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void notcard_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                       ȣȯ���� �ʴ� ī���Դϴ�                                        I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void pw(string lang) {
        if (lang == "1") {
            pw_e();
            atmui_e();
        }
        else if (lang == "2") {
            pw_k();
            atmui_k();
        }
    }
    void wrongpw(string lang) {
        if (lang == "1") {
            wrongpw_e();
            atmui_e();
        }
        else if (lang == "2") {
            wrongpw_k();
            atmui_k();
        }
    }
    void outnumber_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                      Exceed number of attempts.                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }

    void outnumber_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                ��й�ȣ �Է� Ƚ���� �ʰ��Ǿ����ϴ�.                                   I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }

    void wrongpw_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                        Invalid Password                                               I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                          Retry please.                                                I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }

    void wrongpw_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                     �߸��� ��й�ȣ �Դϴ�.                                           I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                        �ٽ� �Է��� �ּ���.                                            I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void outnumber(string lang) {
        if (lang == "1") {
            outnumber_e();
            atmui_e();
        }
        else if (lang == "2") {
            outnumber_k();
            atmui_k();
        }
    }

    void depositmenu_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                         Choose what you want                                          I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                  1) Cash  2) Check  3) Menu  4) Quit                                  I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }


    void depositmenu_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                      ���Ͻô� ������ �������ּ���                                     I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                  1) ����  2) ��ǥ  3) �޴�  4) ������                                 I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void depositmenu(string lang) {
        if (lang == "1") {
            depositmenu_e();
            atmui_e();
        }
        else if (lang == "2") {
            depositmenu_k();
            atmui_k();
        }
    }
    void totalmenu_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                    Please select transaction                                          I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                              1) Deposit  2) Withdrawal  3) Transfer  4) Quit                          I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }

    void totalmenu_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                      ���Ͻô� �ŷ��� �������ּ���                                     I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                  1) �Ա�  2) ���  3) �۱�  4) ������                                 I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void totalmenu(string lang) {
        if (lang == "1") {
            totalmenu_e();
            atmui_e();
        }
        else if (lang == "2") {
            totalmenu_k();
            atmui_k();
        }
    }
    void withdrawal_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                   Please input withdrawal money                                       I    I" << endl;
        cout << "I    I                                  Fees will be paid automatically                                      I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void withdrawal_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                      ����� �ݾ��� �Է����ּ���                                       I    I" << endl;
        cout << "I    I                                   ������� �ڵ����� ������ ���Դϴ�                                   I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void withdrawal(string lang) {
        if (lang == "1") {
            withdrawal_e();
            atmui_e();
        }
        else if (lang == "2") {
            withdrawal_k();
            atmui_k();
        }
    }
    void w3_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                            ��� Ƚ���� ������ �Ѿ���ϴ�.(�ִ� 3ȸ)                                 I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void w3_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                       Out number withdrawal. (Max 3)                                  I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void w3(string lang) {
        if (lang == "1") {
            w3_e();
            atmui_e();
        }
        else if (lang == "2") {
            w3_k();
            atmui_k();
        }
    }
    void transfermenu_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                        Choose what you want                                           I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                      1) Cash transfer  2) Account transfer  3) Menu  4) Quit                          I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void transfermenu_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                            ���� �� ����ּ���                                         I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                               1) ���� �۱�  2) ���� �۱�  3) �޴�  4) ������                          I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void transfermenu(string lang) {
        if (lang == "1") {
            transfermenu_e();
            atmui_e();
        }
        else if (lang == "2") {
            transfermenu_k();
            atmui_k();
        }
    }
    void finish_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                            Thank you                                                  I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void finish_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                            �����մϴ�.                                                I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void finish(string lang) {
        if (lang == "1") {
            finish_e();
            atmui_e();
        }
        else if (lang == "2") {
            finish_k();
            atmui_k();
        }
    }
    void admin_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                     Admin Card�� ���Խ��ϴ�. ��ü �ŷ� ������ Ȯ���ϰڽ��ϱ�?                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                     1. ��           2. �ƴϿ�                                         I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void admin_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                       Admin Card is in. Do you want check All transaction                             I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                     1. Yes                2. No                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void admin(string lang) {
        if (lang == "1") {
            admin_e();
            atmui_e();
        }
        else if (lang == "2") {
            admin_k();
            atmui_k();
        }
    }
    void invalidnum_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                       Invalid input number                                            I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void invalidnum_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                       �߸��� �Է��Դϴ�.                                              I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void invalidnum(string lang) {
        if (lang == "1") {
            invalidnum_e();
            atmui_e();
        }
        else if (lang == "2") {
            invalidnum_k();
            atmui_k();
        }
    }
    void atmnomoney_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                      There is not enough cash in the ATM                              I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void atmnomoney_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                        ATM ���� ������ �����մϴ�                                     I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void atmnomoney(string lang) {
        if (lang == "1") {
            atmnomoney_e();
            atmui_e();
        }
        else if (lang == "2") {
            atmnomoney_k();
            atmui_k();
        }
    }
    void acc_nomoney_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                       Insufficient funds in account                                   I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void acc_nomoney_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                       ���¿� �ܾ��� �����մϴ�                                        I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void acc_nomoney(string lang) {
        if (lang == "1") {
            acc_nomoney_e();
            atmui_e();
        }
        else if (lang == "2") {
            acc_nomoney_k();
            atmui_k();
        }
    }
    void banknotelimit_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                       Banknote limit exeeded                                          I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void banknotelimit_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                       ���� �ѵ� �ʰ�                                                  I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void banknotelimit(string lang) {
        if (lang == "1") {
            banknotelimit_e();
            atmui_e();
        }
        else if (lang == "2") {
            banknotelimit_k();
            atmui_k();
        }
    }
    void depwithfee_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                            Insert cash                                                I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void depwithfee_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                         ������ �Ա����ֽʽÿ�                                         I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void depwithfee(string lang) {
        if (lang == "1") {
            depwithfee_e();
            atmui_e();
        }
        else if (lang == "2") {
            depwithfee_k();
            atmui_k();
        }
    }
    void paper_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                              Insert Check                                             I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void paper_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                             ��ǥ�� �������ּ���                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void paper(string lang) {
        if (lang == "1") {
            paper_e();
            atmui_e();
        }
        else if (lang == "2") {
            paper_k();
            atmui_k();
        }
    }
    void invalidfee_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                       Invalid fee inserted                                            I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void invalidfee_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                       �߸��� �����ᰡ �ԷµǾ����ϴ�                                  I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void invalidfee(string lang) {
        if (lang == "1") {
            invalidfee_e();
            atmui_e();
        }
        else if (lang == "2") {
            invalidfee_k();
            atmui_k();
        }
    }
    void checklimit_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                          Check limit exeeded                                          I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void checklimit_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                          ��ǥ ���� �ʰ�                                               I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void checklimit(string lang) {
        if (lang == "1") {
            checklimit_e();
            atmui_e();
        }
        else if (lang == "2") {
            checklimit_k();
            atmui_k();
        }
    }
    void checkinputfee_e(long long fee) {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                  Insert deposit fee " << fee << " Won                                I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void checkinputfee_k(long long fee) {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                      ������ " << fee << " ���� �Ա����ּ���                                     I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void checkinputfee(string lang, long long fee) {
        if (lang == "1") {
            checkinputfee_e(fee);
            atmui_e();
        }
        else if (lang == "2") {
            checkinputfee_k(fee);
            atmui_k();
        }
    }
    void withdrawlimit_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                          Withdrawal limit exeeded                                     I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void withdrawlimit_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                            ���� �ѵ� �ʰ�                                             I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void withdrawlimit(string lang) {
        if (lang == "1") {
            withdrawlimit_e();
            atmui_e();
        }
        else if (lang == "2") {
            withdrawlimit_k();
            atmui_k();
        }
    }
    void transferaccount_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                            Input Account Number                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void transferaccount_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                   ������ ���� ��ȣ�� �Է����ּ���                                     I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void transferaccount(string lang) {
        if (lang == "1") {
            transferaccount_e();
            atmui_e();
        }
        else if (lang == "2") {
            transferaccount_k();
            atmui_k();
        }
    }
    void transfermoney_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                    Please input transfermoney                                         I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void transfermoney_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                   �۱��� �ݾ��� �Է����ּ���                                          I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void transfermoney(string lang) {
        if (lang == "1") {
            transfermoney_e();
            atmui_e();
        }
        else if (lang == "2") {
            transfermoney_k();
            atmui_k();
        }
    }
    void inv_accnum_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                        Invalid account number                                         I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void inv_accnum_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                        �߸��� ���¹�ȣ�Դϴ�                                          I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void inv_accnum(string lang) {
        if (lang == "1") {
            inv_accnum_e();
            atmui_e();
        }
        else if (lang == "2") {
            inv_accnum_k();
            atmui_k();
        }
    }
	void lessthanfee_e() {
		cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
		cout << "I                                                                                                                 I" << endl;
		cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
		cout << "I    I                                                                                                       I    I" << endl;
		cout << "I    I                                                                                                       I    I" << endl;
		cout << "I    I                                    Please input more than fee                                         I    I" << endl;
		cout << "I    I                                                                                                       I    I" << endl;
		cout << "I    I                                                                                                       I    I" << endl;
		cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
	}
	void lessthanfee_k() {
		cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
		cout << "I                                                                                                                 I" << endl;
		cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
		cout << "I    I                                                                                                       I    I" << endl;
		cout << "I    I                                                                                                       I    I" << endl;
		cout << "I    I                                 �����Ẹ�� ���� �ݾ��� �Ա��Ͻÿ�                                     I    I" << endl;
		cout << "I    I                                                                                                       I    I" << endl;
		cout << "I    I                                                                                                       I    I" << endl;
		cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
	}
	void lessthanfee(string lang) {
		if (lang == "1") {
			lessthanfee_e();
			atmui_e();
		}
		else if (lang == "2") {
			lessthanfee_k();
			atmui_k();
		}
	}
};

class Single_ATM : public Multi_ATM {
public:
    Single_ATM(Bank* mbank, string sernum, string lang, long long num_1000, long long num_5000, long long num_10000, long long num_50000, long long p_dep, long long n_dep, long long p_with, long long n_with, long long pp_trans, long long pn_trans, long long nn_trans, long long cash_trans) : Multi_ATM(mbank, sernum, lang, num_1000, num_5000, num_10000, num_50000, p_dep, n_dep, p_with, n_with, pp_trans, pn_trans, nn_trans, cash_trans) {

    }
    void insert_card() {
        string card_num;
        hello(lang_type);
        if (lang_type != "1") {
            if (lang_type != "2") {
                return;
            }
        }
        cin >> card_num;
        while (card_num == "99") {           //99
            display_ATM();
            main_bank->display_account();
            hello(lang_type);
            cin >> card_num;
        }
        if (card_num == ATM_serial) {
            admin(lang_type);
            long long ans;
            cin >> ans;
            while (ans == 99) {           //99
                display_ATM();
                main_bank->display_account();
                admin(lang_type);
                cin >> ans;
            }
            if (ans == 1) {
                print_all_session();
                return;
            }
            else { return; }
        }

        Account* acc = main_bank->find_account(stoll(card_num));
        if (acc == nullptr) {
            notcard(lang_type);
            return;
        }
        if (acc->get_bank() != main_bank) {
            notcard(lang_type);
            return;
        }
        enter_password(acc);
    }
};

long long Multi_ATM::withdrawal_num = 0;

long long Trans_History::id_counter = 0;

int main() {
    Bank* b0 = new Bank("None");
    Multi_ATM* a0 = new Multi_ATM(nullptr, "", "", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    //Multi_ATM(Bank* mbank, long long sernum, string lang, long long num_1000, long long num_5000, long long num_10000, long long num_50000, long long p_dep, long long n_dep, long long p_with, long long n_with, long long pp_trans, long long pn_trans, long long nn_trans, long long cash_trans)

    while (1) {
        long long ia;
        cout << "0. Bank ���� 1. Account ����  2. ATM ����  3. �ŷ� ����" << endl;
        cin >> ia;
        if (ia == 0) {
            string bank_name;
            // Bank(string bname)
            cout << "���� �̸� ����" << endl;
            cin >> bank_name;
            Bank* b1 = new Bank(bank_name);
            b0->addBankToList(b1);
        }
        else if (ia == 1) {
            string abank_name;
            cout << "�ŷ� ������ �������ּ���" << endl;
            cin >> abank_name;
            Bank* bank = b0->find_bank(abank_name);
            while (bank == nullptr) {
                cout << "���������ʴ� �����Դϴ�. �ùٸ� �ŷ� ������ �������ּ���" << endl;
                cin >> abank_name;
                bank = b0->find_bank(abank_name);
            }
            string b;
            long long c;
            long long d;
            long long e;
            string f;
            cout << "���¹�ȣ�� �������ּ���(12�ڸ�)" << endl;
            cin >> b;
            while (b.size() != 12) {
                cout << "�ùٸ��� ���� �Է��Դϴ�. �ùٸ� ���¹�ȣ�� �������ּ���(12�ڸ�)" << endl;
                cin >> b;
            }
            cout << "��й�ȣ�� �������ּ���" << endl;
            cin >> c;
            cout << "���� �ܾ��� �������ּ���" << endl;
            cin >> d;
            cout << "���� ���� ī�� ��ȣ�� �������ּ���" << endl;
            cin >> e;
            cout << "����� �̸��� �Է����ּ���" << endl;
            cin >> f;
            Account* aa1 = new Account(bank, b, c, d, e, f);
            bank->add_accountlst(aa1); // ���� ������ �߰�
            //for (Bank* bank : Bank::bank_list)
        }
        else if (ia == 2) {
            long long aa;
            cout << "1. ��Ƽ ���� ATM   2. ���� ���� ATM" << endl;
            cin >> aa;
            //ATM(Bank* mbank, long long sernum, string type, string lang, long long num_1000, long long num_5000, long long num_10000, long long num_50000, long long p_dep, long long n_dep, long long p_with, long long n_with, long long pp_trans, long long pn_trans, long long nn_trans, long long cash_trans)
            if (aa == 1) {
                //Multi_ATM(Bank* mbank, long long sernum, string lang, long long num_1000, long long num_5000, long long num_10000, long long num_50000, long long p_dep, long long n_dep, long long p_with, long long n_with, long long pp_trans, long long pn_trans, long long nn_trans, long long cash_trans)
                string Abank_name;
                string sn;
                string lg; //Uni, Bi ����
                long long n1; // 1000
                long long n5; // 5000
                long long n10; // 10000
                long long n50; // 50000
                long long pdf;
                long long ndf;
                long long pwf;
                long long nwf;
                long long pptf;
                long long pntf;
                long long nntf;
                long long ctf;
                cout << "�� ������ �������ּ���" << endl;
                cin >> Abank_name;
                Bank* bank = b0->find_bank(Abank_name);
                while (bank == nullptr) {
                    cout << "���������ʴ� �����Դϴ�. �ùٸ� �� ������ �������ּ���" << endl;
                    cin >> Abank_name;
                    bank = b0->find_bank(Abank_name);
                }
                cout << "ATM ���� �ѹ��� �������ּ���(6�ڸ�)" << endl;
                cin >> sn;
                while (sn.size() != 6) {
                    cout << "�ùٸ��� ���� �Է��Դϴ�. ATM ���� �ѹ��� �������ּ���(6�ڸ�)" << endl;
                    cin >> sn;
                }
                cout << "ATM ������� ����(Uni-���� or Bi-����)(����� �Է�)" << endl;
                cin >> lg;
                cout << "ATM ���� 1000�� ����" << endl;
                cin >> n1;
                cout << "ATM ���� 5000�� ����" << endl;
                cin >> n5;
                cout << "ATM ���� 10000�� ����" << endl;
                cin >> n10;
                cout << "ATM ���� 50000�� ����" << endl;
                cin >> n50;
                cout << "�� ���� �Ա� ������" << endl;
                cin >> pdf;
                cout << "���� ���� �Ա� ������" << endl;
                cin >> ndf;
                cout << "�� ���� ��� ������" << endl;
                cin >> pwf;
                cout << "���� ���� ��� ������" << endl;
                cin >> nwf;
                cout << "�� ���࿡�� �� �������� ������ü ������" << endl;
                cin >> pptf;
                cout << "�� ���࿡�� ���� ����/���� ���࿡�� �� �������� ������ü ������" << endl;
                cin >> pntf;
                cout << "���� ���࿡�� ���� �������� ������ü ������" << endl;
                cin >> nntf;
                cout << "���� �۱� ������" << endl;
                cin >> ctf;
                Multi_ATM* a1 = new Multi_ATM(bank, sn, lg, n1, n5, n10, n50, pdf, ndf, pwf, nwf, pptf, pntf, nntf, ctf);
                a0->addATMToList(a1);
                //��Ƽ ATM Constructor
            }

            else if (aa == 2) {
                //Multi_ATM(Bank* mbank, long long sernum, string lang, long long num_1000, long long num_5000, long long num_10000, long long num_50000, long long p_dep, long long n_dep, long long p_with, long long n_with, long long pp_trans, long long pn_trans, long long nn_trans, long long cash_trans)
                string Abank_name;
                string sn;
                string lg; //Uni, Bi ����
                long long n1; // 1000
                long long n5; // 5000
                long long n10; // 10000
                long long n50; // 50000
                long long pdf;
                long long ndf;
                long long pwf;
                long long nwf;
                long long pptf;
                long long pntf;
                long long nntf;
                long long ctf;
                cout << "�� ���� �̸��� �������ּ���" << endl;
                cin >> Abank_name;
                Bank* bank = b0->find_bank(Abank_name);
                while (bank == nullptr) {
                    cout << "���������ʴ� �����Դϴ�. �ùٸ� �� ������ �������ּ���" << endl;
                    cin >> Abank_name;
                    bank = b0->find_bank(Abank_name);
                }
                cout << "ATM ���� �ѹ��� �������ּ���" << endl;
                cin >> sn;
                while (sn.size() != 6) {
                    cout << "�ùٸ��� ���� �Է��Դϴ�. ATM ���� �ѹ��� �������ּ���(6�ڸ�)" << endl;
                    cin >> sn;
                }
                cout << "ATM ������� ����(Uni-���� or Bi-����)(����� �Է�)" << endl;
                cin >> lg;
                cout << "ATM ���� 1000�� ����" << endl;
                cin >> n1;
                cout << "ATM ���� 5000�� ����" << endl;
                cin >> n5;
                cout << "ATM ���� 10000�� ����" << endl;
                cin >> n10;
                cout << "ATM ���� 50000�� ����" << endl;
                cin >> n50;
                cout << "�� ���� �Ա� ������" << endl;
                cin >> pdf;
                cout << "���� ���� �Ա� ������" << endl;
                cin >> ndf;
                cout << "�� ���� ��� ������" << endl;
                cin >> pwf;
                cout << "���� ���� ��� ������" << endl;
                cin >> nwf;
                cout << "�� ���࿡�� �� �������� ������ü ������" << endl;
                cin >> pptf;
                cout << "�� ���࿡�� ���� ����/���� ���࿡�� �� �������� ������ü ������" << endl;
                cin >> pntf;
                cout << "���� ���࿡�� ���� �������� ������ü ������" << endl;
                cin >> nntf;
                cout << "���� �۱� ������" << endl;
                cin >> ctf;
                //���� ATM Constructor
                Single_ATM* a1 = new Single_ATM(bank, sn, lg, n1, n5, n10, n50, pdf, ndf, pwf, nwf, pptf, pntf, nntf, ctf);
                a0->addATMToList(a1);
            }
        }
        else if (ia == 3) {
            break;
        }
        else {
        }
    }


    for (Bank* bank : b0->get_banklist()) {
        bank->set_banklist(b0->get_banklist());
    }

    for (Multi_ATM* atm : a0->get_atmlist()) {
        atm->set_atmlist(a0->get_atmlist());
    }

    while (1) {
        long long a;
        cout << "1. ���ϴ� ATM ����   2. ���α׷� ���� ����   3. ��� ���� Ȯ��" << endl;
        cin >> a;
        if (a == 1) {
            string num;
            cout << "ATM ���� ��ȣ�� �Է��ϼ���(6�ڸ�)" << endl;
            cin >> num;
            Multi_ATM* atm = a0->find_atm(num);
            try {
                if (atm == nullptr) {
                    throw std::invalid_argument("�������� �ʴ� ATM�Դϴ�");
                }
                atm->start_ATM();
            }
            catch (const std::exception& e) {
                std::cerr << "Caught exception: " << e.what() << std::endl;
            }
        }
        else if (a == 2) {
            break;
        }
        else if (a == 3) {
            a0->display_ATM();
            b0->display_account();
        }
        else {
        }
    }
    return 0;

}