#include <iostream>
#include <set>
#include <vector>
#include <map>

using namespace std;

//***Task 1***

template <typename T>
void unique_print(T begin, T end) {

    set <string> string;
    copy(begin, end, inserter(string, string.end()));
    copy(string.begin(), string.end(), ostream_iterator<std::string>(cout, ", "));

}

//***Task 2***

string filter(const string& str) {

    const char* ws(" \r\n\t");
    const auto first(str.find_first_not_of(ws));
    const auto last(str.find_last_not_of(ws));
    if (first == string::npos) {
        return {};
    }
    return str.substr(first, last - first + 1);

}

multimap<size_t, string> sentence_stats (const string& str) {

    multimap <size_t, string> ret;
    const auto end_it = str.end();
    auto it1 = str.begin();
    auto it2 = find(it1, end_it, '.');
    while (it1 != end_it && distance(it1, it2) > 0){
        string s = filter({it1, it2});
        if (str.length() > 0) {
            const auto words = count (begin(str), end(str), ' ' ) + 1;
            ret.emplace(make_pair(words, move(str)));
        }
        it1 = next(it2, 1);
        it2 = find(it1, end_it, '.');
        if (it2  == end_it) {
            break;
        }
        return ret;

    }
}




int main() {

    cout << "***Task 1***\n\n";

    vector<string> words{"animal", "car", "human", "car", "animal"};
    unique_print(words.begin(), words.end());
    cout << "\n\n";

    istream_iterator<string> begin {cin};
    istream_iterator<string> end;
    unique_print(begin, end);

    cout << "\n\n***Task 2 ***\n\n";

    cin.unsetf(ios::skipws);
    string str {istream_iterator<char>{cin}, {}};
    for (const auto& [word_count, sentence] : sentence_stats(str)) {
        cout << word_count << " words: " << sentence << "\n";
    }


    return 0;
}
