#include <iostream>
#include <thread>
#include <map>
#include <vector>
#include <cmath>
#include <chrono>

using namespace std;

mutex m;

//***Task 1***

void pcout (const int thread_number) {
    lock_guard lg(m);
    cout << "I'm the thread " << thread_number << std::endl;
}

//***Task 2***

void get_prime_number (int64_t number, int64_t& prime_number) {

    lock_guard lg(m);

    vector<int64_t> prime_numbers;
    auto is_prime {
            [] (int64_t number) {
                for (int64_t i = 2; i <= sqrt(number); ++i)
                    if (number % i == 0) return false;
                return true;
            }
    };

    for (int64_t n = 0; n < (number * 30); ++n) {
        if (is_prime(n)) {prime_numbers.push_back(n);}
        else continue;
    }
    prime_number = prime_numbers.at(number+1);
}

void progress_notification() {

    this_thread::sleep_for(chrono::seconds(2));
    int seconds_ago = 2;
    while (!m.try_lock()) {
            cout << "Work started " << seconds_ago << " seconds ago. Calculation in progress... \n";
            this_thread::sleep_for(chrono::seconds(5));
            seconds_ago += 5;
    }
    m.unlock();
}

//***Task 3***

void masters_home (map<int, string>& masters_stuff, map<int, string>& shop) {

    int patience = 5;
    while (patience > 0) {
        m.lock();
        cout << "Master: What a lovely day! It's time to byu some nice stuff!\n";
        pair<int, string> new_stuff = *shop.rbegin();
        cout << "Master: Look at beautiful " << new_stuff.second << " that i bought today\n";
        masters_stuff.insert(new_stuff);
        shop.erase(new_stuff.first);
        cout << "Master: It's time to sleep\n";
        m.unlock();
        this_thread::sleep_for(chrono::seconds(6));
        cout << "Master: Oh no! I got robbed!\n--------------------\n";
        --patience;
    }
    cout << "Master: I'v had enough! Im moving out from this terrible neighbourhood!\n";
    masters_stuff.clear();
}

void thieves_den (map<int, string>& masters_stuff, map<int,string>& stolen_stuff) {
    this_thread::sleep_for(chrono::seconds(5));
    while (!masters_stuff.empty()) {
        cout << "Thief: Getting darker. Its time for a job!\n";
        m.lock();
        pair<int, string> stolen_thing = *masters_stuff.rbegin();
        cout << "Thief: The " << stolen_thing.second << " has been stolen!\n";
        stolen_stuff.insert(stolen_thing);
        masters_stuff.erase(stolen_thing.first);
        cout << "Thief: Now waiting for the next night...\n";
        m.unlock();
        this_thread::sleep_for(chrono::seconds(5));
}

    
}

int main() {

    cout << "\n\n***Task 1***\n\n";
    thread tr1(pcout, 1);
    tr1.join();
    thread tr2(pcout, 2);
    tr2.join();

    cout << "\n\n***Task 2***\n\n";

    int64_t number = 1;
    cout << "Input a number to get a prime number: ";
    cin >> number;
    int64_t prime_number = 0;
    thread tr3(get_prime_number, number, ref(prime_number));
    thread tr4(progress_notification);
    tr3.join();
    tr4.join();

    cout << "Thank you for waiting! Prime number " << number << " is " << prime_number;

    cout << "\n\n***Task 3***\n\n";

    map<int, string> shop = {{200,"table"},
                             {600, "audio-system"},
                             {3000, "GTX 3080"},
                             {100, "blender"},
                             {70,"microwave"},
                             {400,"painting"}};

    map<int, string> masters_stuff = {{800, "TV"},
                                          {50, "chair"}};
    map<int,string> stolen_stuff;
    thread master(masters_home, ref(masters_stuff), ref(shop));
    thread thief(thieves_den, ref(masters_stuff), ref(stolen_stuff));
    master.join();
    thief.join();

    return 0;
}
