#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

//***Task 1***

template <class T, class T2>
void insert_sorted(T& container, T2 value) {
    auto bound = lower_bound(container.begin(), container.end(), value);
    container.insert(bound, value);
}

template <class T>
void print_vector(const vector<T>& vector) {
    copy(vector.begin(), vector.end(), ostream_iterator<T>{cout, " "});
}


int main() {

    cout << "\n***Task 1***\n\n";
    vector<int> sorted_vector = {0,2,3,4,5,6,8,9};
    cout << "Original vector: ";
    print_vector(sorted_vector);
    int value = 0;
    cout << "\n\nEnter a value to insert in vector: ";
    cin >> value;
    insert_sorted(sorted_vector, value);
    cout << "\nNew modified vector: ";
    print_vector(sorted_vector);

    cout << "\n\n***Task 2***\n\n";

    vector<double> analog_signal(100);
    vector<int> digital_signal(100);

    generate(analog_signal.begin(), analog_signal.end(),
             [i = 0]() mutable {
        return (5.0 * sin(++i * 2.0 * 3.14 / 100));
    });

    copy (analog_signal.begin(), analog_signal.end(), digital_signal.begin());

    cout << "Analog signal vector: ";
    print_vector(analog_signal);
    cout << "\nDigital signal vector: ";
    print_vector(digital_signal);
    cout << "\n";

    auto error = inner_product(analog_signal.begin(), analog_signal.end(), digital_signal.begin(), 0.0, plus<double>{},
                               [](double a, double b)
                               {
                                    return pow(a - b, 2);
                               });

    cout << "Error: " << error;

    return 0;
}
