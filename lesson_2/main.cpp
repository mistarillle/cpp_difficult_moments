#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <algorithm>

//***Task 1***

template <typename T>
void Swap(T* ptr1, T* ptr2) {
    T temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
}

//***Task 2***

template <typename T>
void SortPointers(std::vector<T*>& ptr_vector) {
    std::sort(ptr_vector.begin(), ptr_vector.end(),
                                 [](T* ptr1, T* ptr2) {
                                    return *ptr1 < *ptr2;
                                });
}

template <typename T>
void PrintVector (std::vector <T*> ptr_vector) {
    for (auto ptr : ptr_vector) {
        std::cout << *ptr << " ";
    }
}

//***Task 3***

class Timer {

        using clock_t = std::chrono::high_resolution_clock;
        using second_t = std::chrono::duration<double, std::ratio<1>>;

        std::string m_name;
        std::chrono::time_point<clock_t> m_beg;
        double elapsed() const {
            return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
        }
public:

        Timer() : m_beg(clock_t::now()) {}
        Timer(std::string name) : m_name(name), m_beg(clock_t::now()) {}

        void start(std::string name) {
            m_name = name;
            m_beg = clock_t::now();
        }

        void print() const {
            std::cout << m_name << ":\t" << elapsed() * 1000 << " ms" << "\n";
        }

    };

void CountVowels (const std::string& s) {
    std::string vowels = "АаЕеЁёИиОоУуЫыЭэЮюЯя";
    std::string_view sw = s;

    Timer timer1 ("Count_if и find");
    int32_t result1 = std::count_if(sw.begin(), sw.end(),
                  [&](char sw_letter)
                  {
                    return (vowels.find(sw_letter) != std::string::npos);
                  }
                  );
    timer1.print();
    std::cout << "Result: " << result1 << std::endl;

    Timer timer2("Count_if и for");
    int32_t result2 = std::count_if(sw.begin(), sw.end(),
                                [&](char sw_letter)
                                {
                                     for(char letter : vowels)
                                    {
                                        if (letter == sw_letter)
                                        {
                                            return true;
                                        }
                                    }
                                }
                                );
    timer2.print();
    std::cout << "Result: " << result2 << std::endl;

    Timer timer3("For и find");
    int32_t result3 = 0;
    for (char sw_letter : sw) {
        if (vowels.find(sw_letter) != std::string::npos)
            ++result3;
    }
    timer3.print();
    std::cout << "Result: " << result3 << std::endl;

    Timer timer4("For и for");
    int32_t result4 = 0;
    for (char sw_letter : sw){
        for (char letter : vowels) {
            if (letter == sw_letter)
                ++result4;
        }
    }
    timer4.print();

    std::cout << "Result: " << result4 << std::endl;
}


int main() {

    std::cout << "\n***Task 1***\n\n";
    int number1 = 25;
    int number2 = 34;
    int *ptr1 = &number1;
    int *ptr2 = &number2;
    std::cout << "Values before change:\n" << "ptr1: " << *ptr1 << "\nptr2: " << *ptr2;
    Swap(ptr1, ptr2);
    std::cout << "\nValues after change:\n" << "ptr1: " << *ptr1 << "\nptr2: " << *ptr2;

    std::cout << "\n\n***Task 2***\n\n";
    int number3 = 2;
    int number4 = 44;
    int *ptr3 = &number3;
    int *ptr4 = &number4;
    std::vector<int*> ptr_vector;
    ptr_vector.push_back(ptr1);
    ptr_vector.push_back(ptr2);
    ptr_vector.push_back(ptr3);
    ptr_vector.push_back(ptr4);
    std::cout << "Original vector:\n";
    PrintVector(ptr_vector);
    std::cout << "\nSorted vector:\n";
    SortPointers(ptr_vector);
    PrintVector(ptr_vector);

    std::cout << "\n\n***Task 3***\n\n";
    std::ifstream file ("war_and_peace.txt");
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0);
    std::string s(size, ' ');
    file.read(&s[0], size);
    CountVowels(s);

    return 0;
}
