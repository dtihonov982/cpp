#include <iostream>
#include <stdexcept>
#include <set>
#include <vector>


using namespace std;


template <typename T>
class Stack {
public:
    void push(T x) {
        if(ubound + 1 >= MAX_SIZE) return;
        ubound++;
        data[ubound] = x;
    }
    T pop() {
        return data[ubound--];
    }
    T top() {
        if(!empty()) return data[ubound];
    }
    bool empty() {
        return ubound < 0;
    }
    int size() {
        return ubound + 1;
    }
private:
    static const int MAX_SIZE = 100;
    T data[MAX_SIZE];
    int ubound = -1;
};

template <typename T>
class Queue {
public:
    Queue(int max_length_): max_length(max_length_), head(0), tail(0)  {
        ptr = new T[max_length];
    }
    ~Queue() {
        delete [] ptr;
    }
    void enqueue(T x) {
        // throw std::overflow_error("Stack overflow!");
        ptr[tail] = x;
        if(tail == max_length - 1)
            tail = 0;
        else
            tail++;
    }
    T dequeue() {
        T x = ptr[head];
        if(head == max_length - 1)
            head = 0;
        else
            head++;
        return x;
    }
    int size() {

    }
    bool empty() {
        return tail == head;
    }
    void dprint() {
        for(int i = 0; i < max_length; ++i) {
            cout << ptr[i] << ' ';
        }
        cout << endl;
    }

private:
    T* ptr;
    int max_length;
    int head;
    int tail;

};

//https://leetcode.com/problems/valid-parentheses/description/
namespace ValidParenthesis {
    struct node {
        char val;
        node* prev;
        node(char c, node* prev_): val(c), prev(prev_) {}
    };

    class stack {
    private:
        node* top_ptr;
    public:
        stack(): top_ptr(nullptr) {}
        void push(char c) {
            node* n = new node(c, top_ptr);
            top_ptr = n;
        }
        node* pop() {
            if(!top_ptr) return nullptr;
            node* result = top_ptr;
            top_ptr = top_ptr->prev;
            return result;
        }
        node* top() {
            return top_ptr;
        }
    };
    
    bool isValid(string s) {
        stack opened;
        char last;

        for(char c: s) {
    /*
            print(opened);
            cout << " + " << c << endl;
    */
            if(c == '{' || c == '[' || c == '(') {
                opened.push(c);
                continue;
            }
            //If stack is empty and parenthesis is closing then return false
            if(!opened.top())
                return false;
            last = opened.top()->val;

            if(last == '(' && c == ')') {
                opened.pop();
                continue;
            }
            if(last == '[' && c == ']') {
                opened.pop();
                continue;
            }
            if(last == '{' && c == '}') {
                opened.pop();
                continue;
            }

            return false;
        }

        return !opened.top();
       
    }

}

//https://leetcode.com/problems/search-insert-position/
int searchInsert(vector<int>& nums, int target) {
    int down = 0;
    int top = nums.size();
    int i;
    while (top - down > 1) {
        i = (top - down)/2 + down;
        if (target < nums[i])
            top = i;
        else if (nums[i] < target)
            down = i + 1;
        else
            return i;
    }
    //there is only one element in diapason [donw;top)
    if (top - down > 0)
        return nums[down] >= target ? down : down + 1;
    else 
        return down;
}

//https://leetcode.com/problems/remove-duplicates-from-sorted-array/
int removeDuplicates(vector<int>& nums) {
    int repeats = 0;
    for(int i = 1; i < nums.size(); i++) {
        if(nums[i] != nums[i-1]) {
            if(repeats) nums[i - repeats] = nums[i];
        }
        else {
            repeats++;
        }
    }
    return nums.size() - repeats;
}

//https://leetcode.com/problems/length-of-last-word/
int lengthOfLastWord(string s) {
    bool wasLettersRight = false;
    int i;
    int endIndex;
    for (i = s.length() - 1; i >= 0; --i) {
        if (wasLettersRight) {
            if (s[i] == ' ') {
                break;
            }
        }
        else {
            if (s[i] != ' ') {
                endIndex = i;
                wasLettersRight = true;
            }
        }
    }

    if (wasLettersRight) {
        return endIndex - i;
    } 
    else {
        return 0;
    }
}

//https://leetcode.com/problems/pascals-triangle/
vector<vector<int>> generatePascalTriangle(int numRows) {
    vector<vector<int>> rows(numRows);
    rows[0] = vector<int>{1};
    if(numRows == 1) return rows;

    rows[1] = vector<int>{1, 1};

    vector<int>* prev = &rows[1];
    vector<int>* curr;
    for (int i = 2; i < numRows; ++i) {
        rows[i] = vector<int>(i + 1);
        curr = &rows[i];
        (*curr)[0] = 1;

        for (int j = 1; j < i; ++j) {
            (*curr)[j] = (*prev)[j-1] + (*prev)[j];
        }
        (*curr)[i] = 1;
        prev = curr;
    }

    return rows;
}

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

//https://leetcode.com/problems/linked-list-cycle/
bool hasCycle(ListNode *head) {
    if (!head) return false;
    set<ListNode*> Set;
    ListNode* node = head;
    while  (Set.find(node) == Set.end()) {
        Set.insert(node);
        node = node->next;
        if (!node) 
            return false;
    } 
    return true;
}

class ListIterator {
public:
    ListNode* ptr;
    int index;
    ListIterator(ListNode* ptr_): ptr(ptr_), index(0) {}
    ListNode& operator++() {
        ptr = ptr->next;
        return *ptr;
    }
    ListNode& operator*() {
        return *ptr;
    }
    int operator-(const ListIterator& other) {
        return ptr->val - other.ptr->val;
    }       
};



//https://leetcode.com/problems/remove-nth-node-from-end-of-list/
ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode* target = head;
    int indexTarget = 0;
    ListNode* ptarget = head;
    int indexPTarget = 0;
    ListNode* current = head;
    int indexCurrent = 0;
    while (current->next) {
        current = current->next;
        indexCurrent++;
        if (indexCurrent - indexTarget > n - 1) {
            target = target->next;
            indexTarget++;
        }
        if (indexCurrent - indexPTarget > n) {
            ptarget = ptarget->next;
            indexPTarget++;
        }            
    }
    if (target == head) {
        head = head->next;
    }
    else {
        ptarget->next = target->next;
    }
    return head;
}

int main()
{

    return 0;
}
