#include<iostream>
#include<vector>
#include<algorithm>
#include<stack>
using namespace std;

void quick_sort_helper(vector<int>& vec, int begin, int end) {
    if(begin >= end) {
        return;
    }

    int pivot = vec[begin];
    int begin_copy = begin;
    int end_copy = end;

    for(;;) {
        for(;vec[end] > pivot; end--){}
        for(;vec[begin] <= pivot && begin != end; begin++){}
        if(end == begin) {
            int tmp = vec[begin];
            vec[begin] = pivot;
            vec[begin_copy] = tmp;
            break;
        }

        int tmp = vec[begin];
        vec[begin] = vec[end];
        vec[end] = tmp;
        --end;
    }

    quick_sort_helper(vec, begin_copy, end - 1);
    quick_sort_helper(vec, end + 1, end_copy);
}

void quick_sort(vector<int>& vec){
    quick_sort_helper(vec, 0, vec.size() - 1);
}

// 实现堆排序(大根堆排序)
void push_heap(vector<int>& vec, int val) {
    vec.push_back(val);
    int child = vec.size() - 1;
    int parent = (child - 1) / 2;

    while(parent > 0 && val > vec[parent]) {
        vec[child] = vec[parent];
        child = parent;
        parent = (child - 1) / 2;
    }
    if(vec[parent] < val) {
        vec[child] = vec[parent];
        child = parent;
    }

    vec[child] = val;

}

void pop_heap(vector<int>& vec, int length) {
    if(length <= 1) {return;}

    // 交换第一个与最后一个元素
    int tmp = vec[0];
    vec[0] = vec[length - 1];
    vec[length - 1] = tmp;

    int parent = 0;
    int right_child = parent * 2 + 2;
    int val = vec[0];
    while(right_child < length - 1) {
        int index = vec[right_child - 1] > vec[right_child] ? right_child - 1 : right_child;
        if(vec[index] > val) {
            vec[parent] = vec[index];
            parent = index;
            right_child = parent * 2 + 2;
            continue;
        }
        vec[parent] = val;
        return;
    }

    if(right_child == length - 1) {
        if(vec[length - 2] > val) {
            vec[parent] = vec[length - 2];
            parent = length - 2;
        }
    }

    vec[parent] = val;
}

void make_heap(vector<int>& vec) {
    if(vec.empty()) return;

    vector<int> heap;
    heap.reserve(vec.size());

    for(int val : vec) {
        push_heap(heap, val);
    }

    vec.swap(heap);
}

void sort_heap(vector<int>& vec) {
    make_heap(vec);
    int times = vec.size() - 1;
    for(int i = vec.size(); i > 1; i--) {
        pop_heap(vec, i);
        for(int val : vec) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    vector<int> vec(1);
    int& cur = vec[0];
    vec.push_back(1);
    cout << &cur << endl;
    cout << &vec[0] << endl;

    return 0;
}