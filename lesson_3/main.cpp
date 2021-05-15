#include <iostream>
#include <list>

//***Task 1***

void addAverage (std::list<double>& numbers) {
double result = 0.0;
    for (auto num : numbers) {
        result += num;
    }
numbers.push_back(result/numbers.size());
}

template <class T>
void printContainer(const T& numbers){
    for (auto & num : numbers) {
        std::cout << num << " ";
    }
}

//***Task 2***

class Matrix {

    int m_matrix[3][3] = {{1, 2, 5},
                          {3, 4, 7},
                          {6, 8, 9}};

public:

//    Matrix (int * matrix) : m_matrix(matrix) {};

    int findDeterminant() {

        if ((sizeof(m_matrix) / sizeof(int)) == 4) {

            return (m_matrix[0][0] * m_matrix[1][1] - m_matrix[0][1] * m_matrix[1][0]);
        }
        if ((sizeof(m_matrix) / sizeof(int)) == 9) {

            return (m_matrix[0][0] * (m_matrix[1][1] * m_matrix[2][2] - m_matrix[1][2] * m_matrix[2][1]) -
                    m_matrix[1][0] * (m_matrix[0][1] * m_matrix[2][2] - m_matrix[0][2] * m_matrix[2][1]) +
                    m_matrix[2][0] * (m_matrix[0][1] * m_matrix[1][2] - m_matrix[0][2] * m_matrix[1][1]));
        }

    }
};

//Тут пытался возвращать матрицу меньшего порядка после разложения по строке/столбцу

//    int *get_matrix(int row) {
//
//        int new_array[2][2] = {0};
//        int new_i = 0;
//        int new_k = 0;
//
//        for (int i = 0; i <= 2; ++i) {
//            if (i == row) {continue;}
//            new_i = 0;
//            new_k = 0;
//            for (int k = 0; k <= 2; ++k)
//            {
//                if (k == 0) {continue;}
//                new_array[new_i][new_k] = m_matrix[i][k];
//                new_i += 1;
//                new_k += 1;
//            }
//        }
//        std::cout <<"new arr " << new_array[1][0] << "\n";
//        return *new_array;
//
//    }
//};


//***Task 3***

class num_iterator {

    int i = 0;

public:

    explicit num_iterator(int position = 0) : i{position} {}
    int operator*() const {return i;}
    num_iterator& operator++() {
        ++i;
        return *this;
    }
    bool operator!=(const num_iterator& other) const {
        return i != other.i;
    }

};

class num_range {

    int a = 0;
    int b = 0;

public:

    num_range(int from, int to)
    : a{from}, b{to}

    {}

    num_iterator begin() const {return num_iterator{a};}
    num_iterator end() const {return num_iterator{b};}
};


int main() {

    std::cout << "\n***Task 1***\n\n";
    std::list<double> numbers{1.1, 2.3, 4.1, 32.5, 15.6};
    std::cout << "Original list: ";
    printContainer(numbers);
    std::cout << "\nResult list: ";
    addAverage(numbers);
    printContainer(numbers);

    std::cout << "\n\n***Task 2***\n\n";

    Matrix matrix;
    std::cout << "Determinant of 3x3 matrix: " << matrix.findDeterminant();

    std::cout << "\n\n***Task 3***\n\n";

    for (int i : num_range(100, 110)) {
        std::cout << i << " ";
    }

    return 0;
}
