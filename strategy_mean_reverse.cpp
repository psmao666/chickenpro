#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include <math.h>
#include <list>
#include <memory>
#include <unistd.h>

enum Action{
    Long,
    Short,
    None
};
enum CandleStickType{
    Bearish,
    Bullish
};

class timeType{
public:
    int year, month, day;
    int hour, minute, second;
    timeType() {}
    timeType(std::string dateTime) {
        // in the format of year-month-day time : "xxxx-xx-xx xx:xx:xx"
        this->year = convert(dateTime[0]) * 1000 + convert(dateTime[1]) * 100 + convert(dateTime[2]) * 10 + convert(dateTime[3]);
        this->month = convert(dateTime[5]) * 10 + convert(dateTime[6]);
        this->day = convert(dateTime[8]) * 10 + convert(dateTime[9]);
        this->hour = convert(dateTime[11]) * 10 + convert(dateTime[12]);
        this->minute = convert(dateTime[14]) * 10 + convert(dateTime[15]);
        this->second = convert(dateTime[17]) * 10 + convert(dateTime[18]);
    }
    inline int convert(char c) {return c-'0';}
    inline std::string fillZero(std::string x) {
        // say the day is 2, we need to fix it as 02
        if (x.size() == 1) x = "0" + x;
        return x;
    }
    inline std::string show() {
        return std::to_string(this->year) + "-" + fillZero(std::to_string(this->month)) + 
                "-" + fillZero(std::to_string(this->day)) + " " + 
                fillZero(std::to_string(this->hour)) + ":" + 
                fillZero(std::to_string(this->minute)) + ":" + 
                fillZero(std::to_string(this->second));
    }
    inline bool isLeapYear(const int year) {
        if (year % 4 == 0) {
            if (year % 100 == 0) {
                if (year % 400 == 0) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return true;
            }
        } else {
            return false;
        }
    }
    inline bool monthChecker(const int month) {
        return (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12);
    }
    timeType& operator++() {
        ++ this->second;
        if (this->second == 60) {
            this->second = 0;
            ++ this->minute;
        }
        if (this->minute == 60) {
            this->minute = 0;
            ++ this->hour;
        }
        if (this->hour == 24) {
            this->hour = 0;
            ++ this->day;
        }
        if (this->month == 2 && isLeapYear(this->year) && this->day == 30) {
            this->day = 1;
            this->month = 3;
        }
        else if (this->month == 2 && !isLeapYear(this->year) && this->day == 29) {
            this->day = 1;
            this->month = 3;
        }
        else if (monthChecker(this->month) && this->day == 32) {
            this->day = 1;
            this->month ++;
        }
        else if (!monthChecker(this->month) && this->day == 31) {
            this->day = 1;
            this->month ++;
        }
        if (month == 13) {
            this->month = 1;
            this->year ++;
        }
        return *this;
    }
    bool operator<= (timeType& obj) {
        if (this->year < obj.year) {
            return true;
        }
        if (this->year > obj.year) {
            return false;
        }
        if (this->month < obj.month) {
            return true;
        }
        if (this->month > obj.month) {
            return false;
        } 
        if (this->day < obj.day) {
            return true;
        }
        if (this->day > obj.day) {
            return false;
        } 
        if (this->hour < obj.hour) {
            return true;
        }
        if (this->hour > obj.hour) {
            return false;
        } 
        if (this->minute < obj.minute) {
            return true;
        }
        if (this->minute > obj.minute) {
            return false;
        } 
        if (this->second < obj.second) {
            return true;
        }
        if (this->second > obj.second) {
            return false;
        } 
        return true;
    }
};

class CandleStick{
public:
    timeType date;
    double open, high, low, close, vol, volCcy, volCcyQuote;
    bool confirm;
    CandleStick() {}
    CandleStick(timeType _date, double _open, double _high, double _close, double _low, double _vol, double _volCcy, double _volCcyQuote, std::string _confirm) {
        this->date = _date;
        this->open = _open;
        this->high = _high;
        this->low = _low;
        this->close = _close;
        this->vol = _vol;
        this->volCcy = _volCcy;
        this->volCcyQuote = _volCcyQuote;
        this->confirm = (_confirm == "1");
    }
};

class Order{
public:
    int id;
    Action direction;
    double lot;
    double entryPrice;
    Order() {}
    Order(int _id, Action _dir, double _lot, double _entryPrice): id(_id), direction(_dir), lot(_lot), entryPrice(_entryPrice) {}

    double calcPnL(const double curPrice) {
        if (this->direction == Action::Long) {
            return (curPrice > this->entryPrice ? 1 : -1) * fabs(curPrice - this->entryPrice) * this->lot;
        }
        else {
            return (curPrice > this->entryPrice ? -1 : 1) * fabs(curPrice - this->entryPrice) * this->lot;
        }
    }
};

class OrderBook{
public:
    class LinkList{
    public:
        class node{
        public:
            node* prev;
            node* next;
            Order order;
            node() {
                this->prev = nullptr;
                this->next = nullptr;
            }
            node(Order _order) {
                this->order = _order;
                this->prev = nullptr;
                this->next = nullptr;
            }
        };
        node* head;
        LinkList() {
            this->head = nullptr;
        }
        void insert(Order _order) {
            if (this->head == nullptr) {
                this->head = new node(_order);
                return;
            }
            node* cur = this->head;
            while (cur->next != nullptr) {
                cur = cur->next;
            }
            cur->next = new node(_order);
        }
        double getEquity(const double balance, const double curPrice) {
            double equity = balance;
            node* cur = this->head;
            while (cur != nullptr) {
                equity += cur->order.calcPnL(curPrice);
                equity -= cur->order.lot;
                cur = cur->next;
            }
            return equity;
        }
        void cleanOrders(double& balance, const double& curPrice, const double& stoploss, const double& takeprofit) {
            node* cur = this->head;
            while (cur != nullptr) {
                double pnl = cur->order.calcPnL(curPrice);
                if (pnl <= -1*stoploss || pnl >= takeprofit) {
                    // close order here
                    balance += pnl;
                    if (cur->prev != nullptr) cur->prev->next = cur->next;
                    if (cur->next != nullptr) cur->next->prev = cur->prev;
                    node* tmp = cur->next;
                    if (cur == this->head) {
                        this->head = tmp;
                    }
                    delete cur;
                    cur = tmp;
                    continue;
                }
                cur = cur->next;
            }
        }
        void display(const double& curPrice) {
            node* cur = this->head;
            while (cur != nullptr) {
                std::cout << "orderID: " << cur->order.id << ", ";
                if (cur->order.direction == Action::Long) {
                    std::cout << "Direction: LONG, ";
                }
                else std::cout << "Direction: SHORT, ";
                std::cout << "Entry price at " << cur->order.entryPrice << ", we holding " << cur->order.lot << " worth of ETH, ";
                printf("and this order has an PnL of $%.5f\n", cur->order.calcPnL(curPrice));
                cur = cur->next;
            }
        }
        ~LinkList() {
            node* cur = this->head;
            while(cur != nullptr) {
                node* tmp = cur->next;
                delete cur;
                cur = tmp;
            }
            this->head = nullptr;
        }
    };
    LinkList orders;
    double pnl;

    OrderBook() {this->pnl = 0;}

    void updateEquity(double& equity, const double marketPrice, const double balance) {
        equity = orders.getEquity(balance, marketPrice);
    }
    void addOrder(const Order& newOrder){
        orders.insert(newOrder);
    }
    void displayOrders(const double marketPrice) {
        orders.display(marketPrice);
    } 
};

class Strategy{
public:
    int period;
    int order_id;
    std::deque<double> dataStream;
    double ave;
    double tradesize;
    double sl_level;
    double tp_level;
    OrderBook orderbook;

    Strategy() {}
    Strategy(int _period): period(_period) {
        this->ave = 0;
        this->order_id = 1;
        this->tradesize = 0;
        this->sl_level = 0.0002;
        this->tp_level = 0.0003;
    }
    
    void checkForOpen(double balance, double& equity, const CandleStick& newData, const double& slippage) {
        try {
            if (newData.confirm == false) {
                throw std::logic_error("the data feed in is not complete");
            }
        }catch(std::exception&e) {
            std::cout << "Exception raised: " << e.what() << std::endl;
        }

        // update moving average
        this->ave *= dataStream.size();
        if (dataStream.size() == this->period) {
            this->ave -= dataStream.front();
            dataStream.pop_front();
        }
        this->ave += newData.close;
        dataStream.push_back(newData.close);
        this->ave /= dataStream.size();
        
        // check if hit moving average
        std::cout << "(current price, 50day average): ";
        std::cout << newData.close << ' ' << this->ave << std::endl;
        if (fabs(newData.close - this->ave) < 3) {
            int trend = (newData.open < newData.close);
            if (trend == CandleStickType::Bearish) {
                // go long reverse
                orderbook.addOrder(Order(this->order_id++, Action::Long, this->tradesize / newData.close, newData.close - slippage));
                orderbook.updateEquity(equity, newData.close, balance);
            }
            else {
                // go short reverse
                orderbook.addOrder(Order(this->order_id++, Action::Short, this->tradesize / newData.close, newData.close + slippage));
                orderbook.updateEquity(equity, newData.close, balance);
            }
        }
    }
    void checkForClose(double& equity, const CandleStick& newData, double& balance) {
        //orderbook.orders.cleanOrders(balance, newData.close, balance * this->sl_level, balance * this->tp_level);
        orderbook.updateEquity(equity, newData.close, balance);
    }
    void displayOrders(const double& marketPrice) {
        orderbook.displayOrders(marketPrice);
    }
    inline void normalize_tradesize(double equity) {
        this->tradesize = equity * 0.02; // always risk with 2% of our equity only
    }
    virtual ~Strategy() {
        while (!dataStream.empty()) {
            dataStream.pop_back();
        }
    }
};

class Backtester{
public:
    std::vector<CandleStick> histData;
    double balance;
    double equity;

    Backtester() {}
    Backtester(std::string coinDataFile) {
        std::ifstream file(coinDataFile);
        this->balance = 10000; // initial deposit is 10000 usdt
        this->equity = this->balance; // equity is balance as we don't have any orders yet

        // Read each line of the file and split it into tokens
        std::string line, token;
        int ind = 0;
        while (getline(file, line)) {
            ++ ind;
            std::vector<std::string> row;
            std::stringstream ss(line);
            while (getline(ss, token, ',')) {
                row.push_back(token);
            }
            if (ind == 1) continue; // ignore first row
            histData.emplace_back(timeType(row[0]), std::stod(row[1]), std::stod(row[2]), std::stod(row[3]), std::stod(row[4]), 
                                    std::stod(row[5]), std::stod(row[6]), std::stod(row[7]), row[8]);
        }
    }
    // backtest the time interval between [start_time, end_time]
    void run(std::string start_time, std::string end_time) {
        Strategy strategy(50);
        timeType start(start_time);
        timeType end(end_time);
        int increment = 0;
        reverse(histData.begin(), histData.end());
        for (auto p : histData) {
            strategy.checkForClose(this->equity, p, this->balance);
            strategy.normalize_tradesize(this->equity);
            strategy.checkForOpen(this->balance, this->equity, p, 0.01);
            printf("#############################################\n");
            std::cout << p.date.show() << ":\n";
            std::cout << "balance: " << this->balance << "\n";
            std::cout << "equity: " << this->equity << "\n";
            std::cout << "current ETH price at: " << p.close << "\n";
            strategy.displayOrders(p.close);
            printf("#############################################\n");
            // sleep(1);
        }
        
    } 
};

int main() {
    freopen("hisotry.txt", "w", stdout);
    Backtester tester("ETH-USDT.csv");
    tester.run("2018-10-12 16:00:00", "2023-04-18 20:00:00");
    return 0;
}