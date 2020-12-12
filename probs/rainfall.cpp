#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>

using namespace std;

using rev = vector<int>::reverse_iterator;
auto mymax = [](int a, int b){ return (a>b) ? a : b; };
auto mymin = [](int a, int b){ return (a<b) ? a : b; };

void pv(const vector<int> v, const string name) {
    cout<< endl << name << ":";
    for(auto e: v) {
        cout<<e<<" ";
    }
}

/**
 * Given n non-negative integers representing an elevation map where the width of each bar is 1, 
 * compute how much water it is able to trap after raining.
 * 
 * Imagine 2 parts of the range. left part, from first element to highest.
 * And right part, from last element to highest (in right to left direction).
 * For left part, the maximum level of water (not amount of it) trapped on top of ith element
 * can be calculated by finding the minimum of 
 * 1. highest element to the left of i
 * 2. highest element to right of i
 * Therefore, max_level_of_trapped_water[i] = MIN( MAX(left_of_i), MAX(right_of_i) )
 * Also, max_level_of_trapped_water[i] = elevation[i] + trapped_water[i]
 * => trapped_water[i] = max_level_of_trapped_water[i] - elevation[i]
 * Substituting max_level_of_trapped_water[i],
 * trapped_water[i] = MIN( MAX(elevations_left_of_i), MAX(elevations_right_of_i) ) - elevation[i]
 * 
 * We had divided the elevation map into left and right parts. So if we consider above algo
 * just for left part, we can reduce the equation to following:
 * trapped_water[i] = MAX(elevations_left_of_i) - elevation[i], where i belongs to left of highest element.
 * And we can apply the same algo in reverse order on the right part
 * trapped_water[i] = MAX(elevations_right_of_i) - elevation[i], where i belongs to left of highest element.
 * */

int calc_stagnated_water(vector<int> v) {
    // Find max element in v
    auto max_elem = max_element(begin(v), end(v));

    // Find vector of max-height-so-far for scan from 0th element to highest element.
    // Do the same while scanning from last element towards the highest element
    vector<int> max_so_far(v.size());
    partial_sum(begin(v), max_elem, begin(max_so_far), mymax);
    partial_sum(rbegin(v), rev(max_elem), rbegin(max_so_far), mymax);

    // difference of ith element in max-element-so-far and ith element in v gives the 
    // water retained. Calculate it and add all those values and return
    return inner_product(begin(max_so_far), end(max_so_far), begin(v), 0, plus<int>{}, minus<int>{});
}


/**
 * Another solution to the problem is based on the similar fact but instead of dividing 
 * map into left and right parts, we keep track of max elements on left and right of each element.
 * Then,
 * trapped_water[i] = MIN( MAX(elevations_left_of_i), MAX(elevations_right_of_i) ) - elevation[i]
 * 
 * To implement, create 2 lists from the given elevation map:
 * 1. max_so_far_ltr, keeps the max element from 0 to i-1 in ith place
 * 2. max_so_far_rtl, keeps the max element from i+1 to n-1 in ith place
 * 
 * */
int calc_stagnated_water2(vector<int> v) {
    vector<int> ltr_max(v.size()), rtl_max(v.size());
    partial_sum(begin(v), end(v), begin(ltr_max), mymax);
    partial_sum(rbegin(v), rend(v), rbegin(rtl_max), mymax);
    pv(v, "v");
    pv(ltr_max, "ltr_max");
    pv(rtl_max, "rtl_max");

    vector<int> water_level(v.size());
    transform(begin(ltr_max), end(ltr_max), begin(rtl_max), begin(water_level), mymin);
    pv(water_level, "water_level");
    return inner_product(begin(water_level), end(water_level), begin(v), 0, plus<int>{}, minus<int>{});
}

int main() {
    vector<int> v {1, 2, 1, 4, 3, 7, 0, 7, 1, 6, 2, 1, 4, 5, 5, 5, 5, 5};
    auto water_stagnated = calc_stagnated_water2(v);
    cout<<"Water stagnated: " << water_stagnated << endl;
}