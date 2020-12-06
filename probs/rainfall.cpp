#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>

using namespace std;

using rev = vector<int>::reverse_iterator;

int calc_stagnated_water(vector<int> v) {
    // Find max element in v
    auto max_elem = max_element(begin(v), end(v));

    // Find vector of max-height-so-far for scan from 0th element to highest element.
    // Do the same while scanning from last element towards the highest element
    auto mymax = [](int a, int b){ return (a>b) ? a : b; };
    vector<int> max_so_far(v.size());
    partial_sum(begin(v), max_elem, begin(max_so_far), mymax);
    partial_sum(rbegin(v), rev(max_elem), rbegin(max_so_far), mymax);

    // difference of ith element in max-element-so-far and ith element in v gives the 
    // water retained. Calculate it and add all those values and return
    return inner_product(begin(max_so_far), end(max_so_far), begin(v), 0, plus<int>{}, minus<int>{});
}

int main() {
    vector<int> v {1, 2, 1, 4, 3, 7, 0, 7, 2, 6, 2, 1, 4, 5, 5, 5, 5, 5};
    cout<<"Water stagnated: " << calc_stagnated_water(v)<< endl;
}