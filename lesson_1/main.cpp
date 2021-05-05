#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

//***Task 1***

struct Person {

    std::string m_last_name = "None";
    std::string m_first_name = "None";
    std::optional<std::string> m_patronymic;

    Person (std::string last_name, std::string first_name, std::optional<std::string> patronymic = std::nullopt) :
    m_last_name (std::move(last_name)), m_first_name(std::move(first_name)), m_patronymic(std::move(patronymic)) {};

};

std::ostream& operator<<(std::ostream& out, const Person& person) {

    out << person.m_last_name << "\t" << person.m_first_name;
    if (person.m_patronymic && person.m_patronymic.value() != "-") {
        out << "\t" << person.m_patronymic.value();
    }
    else out << "\t";
    return out;
}

bool operator<(const Person& person1, const Person& person2) {

    if (person1.m_patronymic.has_value() && person2.m_patronymic.has_value()) {
        if (person1.m_first_name == person2.m_first_name) {
            return person1.m_patronymic.value() < person2.m_patronymic.value();
        }
        if (person1.m_last_name == person2.m_last_name) {
            return person1.m_first_name < person2.m_last_name;
        }
        return person1.m_last_name < person2.m_last_name;

}}

bool operator==(const Person& person1, const Person& person2) {

    if (person1.m_patronymic && person2.m_patronymic)
    {
        return tie(person1.m_last_name, person1.m_first_name, person1.m_patronymic.value()) ==
        tie(person2.m_last_name, person2.m_first_name, person2.m_patronymic.value());
    }
    else
    {
        return tie(person1.m_last_name, person1.m_first_name) ==
        tie(person2.m_last_name, person2.m_first_name);
    }

}

//***Task 2***

struct PhoneNumber {

    std::string m_number = "none";
    std::optional<int> m_additional_number = std::nullopt;
    int16_t m_country_code = 0;
    int16_t m_city_code = 0;

    PhoneNumber (int16_t country_code, int16_t city_code, std::string number, std::optional<int> additional_number = std::nullopt ) :
    m_country_code(country_code), m_city_code(city_code), m_number(std::move(number)), m_additional_number(additional_number) {};

};

std::ostream& operator<<(std::ostream& out, PhoneNumber& phone_number) {

    out << "+" << phone_number.m_country_code << "(" << phone_number.m_city_code << ")" <<
    phone_number.m_number;
    if (phone_number.m_additional_number && phone_number.m_additional_number.value() != 0) {
        out << " " << phone_number.m_additional_number.value();
    }
    return out;
}

//***Task 3***

class PhoneBook {

//protected:
    std::vector<std::pair<Person, PhoneNumber>> m_phone_book;

public:

    PhoneBook(std::ifstream& input) {

        std::string tmp;

          while(std::getline(input, tmp)) {
            std::istringstream str(tmp);

            std::string tmp_last_name;
            std::string tmp_first_name;
            std::string tmp_patronymic;
            int16_t tmp_country_code = 0;
            int16_t tmp_city_code = 0;
            std::string tmp_number;
            int16_t tmp_additional_number = 0;

            str >> tmp_last_name >> tmp_first_name >> tmp_patronymic >> tmp_country_code >>
                  tmp_city_code >> tmp_number >> tmp_additional_number;

            Person person (tmp_last_name, tmp_first_name, tmp_patronymic);
            PhoneNumber phone_number (tmp_country_code, tmp_city_code, tmp_number, tmp_additional_number);

            m_phone_book.emplace_back(std::make_pair(person,phone_number));

          }
        }

    void SortByName() {

        std::sort(m_phone_book.begin(), m_phone_book.end(),
                  [](std::pair<Person,PhoneNumber>& pair1, std::pair<Person,PhoneNumber>& pair2)
                  {
                      return pair1.first < pair2.first;
                        }
                  );
    }

    void SortByPhone() {

        std::sort(m_phone_book.begin(), m_phone_book.end(),
                  [](std::pair<Person,PhoneNumber>& pair1, std::pair<Person,PhoneNumber>& pair2)
                    {
                        if (pair1.second.m_additional_number.has_value() && pair2.second.m_additional_number.has_value())
                        {
                            if (pair1.second.m_number == pair2.second.m_number) {
                                return pair1.second.m_additional_number.value() < pair2.second.m_additional_number.value();
                            }
                        }
                        if (pair1.second.m_city_code == pair2.second.m_city_code) {
                            return pair1.second.m_number < pair2.second.m_number;
                        }
                        if (pair1.second.m_country_code == pair2.second.m_country_code) {
                            return pair1.second.m_city_code < pair2.second.m_city_code;
                        }
                        return pair1.second.m_country_code < pair2.second.m_country_code;


                    });
    }

    std::tuple<std::string, PhoneNumber> GetPhoneNumber(const std::string& last_name) {
        //Making empty string for returning it empty when all its ok
        std::string result;
        //Making vector for store temporary results from std::for_each
        std::vector<PhoneNumber> tmp_vector;

        std::for_each(m_phone_book.begin(), m_phone_book.end(),
                      [&](std::pair<Person,PhoneNumber>& pair) {
                            if (pair.first.m_last_name == last_name){
                                tmp_vector.emplace_back(pair.second);
                            }
        });

        if (tmp_vector.empty()) {
            result = "not found";
            PhoneNumber dull_pn(0,0,"0");
            return std::tie(result, dull_pn);
        }

        if (tmp_vector.size() == 1) {return std::tie(result, tmp_vector[0]);}

        if (tmp_vector.size() > 1) {
            result = "found more than 1";
            return std::tie(result, tmp_vector[0]);
        }

        }

    void ChangePhoneNumber (const Person& person, const PhoneNumber& new_phone_number) {

        auto it = std::find_if(m_phone_book.begin(), m_phone_book.end(),
                     [&](std::pair<Person, PhoneNumber>& pair) {return pair.first == person;}
                     );
        if (it != m_phone_book.end()) {
            it->second = new_phone_number;
        };
    };

    friend std::ostream& operator<<(std::ostream& out, const PhoneBook& book);

    };

std::ostream& operator<<(std::ostream& out, const PhoneBook& book) {

    for (auto [person,number] : book.m_phone_book) {
        out << person << "\t" << number << "\n";
    }

    return  out;
}


int main() {

    std::cout << "\n***Task 1***\n\n";
    Person person1("Paulson", "Robert");
    std::cout << person1 << "\n";

    Person person2("Ivanov", "Ivan", "Ivanovich");
    std::cout << person2 << "\n\n";

    std::cout << std::boolalpha << "It is " << (person1 < person2) << " that person 1 name length's is smaller than person 2\n\n";

    std::cout << "It is " << (person1 == person2) << " that person 1 and person 2 are the same persons\n\n";

    std::cout << "***Task 2***\n\n";

    PhoneNumber phone_number(7,911,"1234567",12);
    std::cout << phone_number;

    std::cout << "\n\n***Task 3***\n\n";

    std::ifstream file("../PhoneBook.txt");
    PhoneBook book(file);
    std::cout << book;

    std::cout << "------SortByPhone-------" << std::endl;
    book.SortByPhone();
    std::cout << book;

    std::cout << "------SortByName--------" << std::endl;
    book.SortByName();
    std::cout << book;

    std::cout << "-----GetPhoneNumber-----" << std::endl;

    auto print_phone_number = [&book](const std::string& surname) {
        std::cout << surname << "\t";
        auto answer = book.GetPhoneNumber(surname);
        if (get<0>(answer).empty())
            std::cout << get<1>(answer);
        else
            std::cout << get<0>(answer);
        std::cout << std::endl;
    };

    print_phone_number("Ivanov");
    print_phone_number("Petrov");

    std::cout << "----ChangePhoneNumber----" << std::endl;
    book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" },
                           PhoneNumber{7, 123, "15344458", std::nullopt});
    book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" },
                           PhoneNumber{ 16, 465, "9155448", 13 });
    std::cout << book;


    return 0;
}
