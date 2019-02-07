#ifndef MTM4_SORTEDSET_H
#define MTM4_SORTEDSET_H

#include <functional>
#include <assert.h>
#include <iostream>
using namespace std;

template <class T, class Compare = std::less<T> >
class SortedSet {
    T** data;
    int set_size;
    int max_size;

    void expand() {
        int new_size = max_size * EXPAND_RATE;
        T** new_data = new T*[new_size];
        for (int i = 0; i < set_size; ++i) {
            new_data[i] = new T(*data[i]);
        }
        for (int i=0 ; i<size() ; ++i) {
            delete data[i];
        }
        delete[] data;
        data = new_data;
        max_size = new_size;
    }
    static const int EXPAND_RATE = 2;
    static const int INITIAL_SIZE = 10;
public:
    /*
     * iterator implementation
     */
    class iterator {
        const SortedSet* set;
        int index;
        friend class SortedSet;
    public:
        iterator(const SortedSet* set, int index) :
                set(set), index(index) {};
        iterator(const iterator&) = default;
        iterator& operator=(const iterator&) = default;
        ~iterator() = default;

        const T& operator*() const {
            assert( index >= 0 && index < set->size() );
            return *(set->data[index]);
        }

        iterator& operator++() {
            ++index;
            return *this;
        }
        iterator operator++(int) {
            iterator temp = *this;
            ++*this;
            return temp;
        }
        bool operator==(const iterator& it) const {
            assert(set == it.set);
//            return (!(Compare(*this,*it))&&!(Compare(*it,*this)));
            return (this->index==it.index);
        }
        bool operator!=(const iterator& it) const {
            return !(it==*this);
        }
    };

    /*
     * sortedSet implementation
     */
    SortedSet() {
        data = new T*[INITIAL_SIZE];
        set_size = 0;
        max_size = INITIAL_SIZE;
    }
    SortedSet& operator=(const SortedSet& set) {
        if (this == &set) {
            return *this;
        }
        delete[] data;
        max_size = set.size()>=INITIAL_SIZE?set.size():INITIAL_SIZE;
        data = new T*[max_size];
        set_size = set.size();

        for (int i = 0; i < set_size; ++i) {
            data[i] = new T(*set.data[i]);
        }
        return *this;
    };
    SortedSet(const SortedSet& set) {
        max_size = set.size()>=INITIAL_SIZE?set.size():INITIAL_SIZE;
        data = new T*[max_size];
        set_size = set.size();

        for(int i = 0; i < set_size; i++) {
            data[i] = new T(*set.data[i]);
        }
    }
    ~SortedSet() {
        for (int i=0 ; i<size() ; ++i) {
            delete data[i];
        }
        delete[] data;
    }

    iterator begin() const {
        return iterator(this, 0);
    }
    iterator end() const {
        return iterator(this, set_size);
    }
    iterator find(const T& elem) const {
        for (iterator it = begin() ; it != end() ; ++it) {
            if (*it == elem) {
                return it;
            }
        }
        return end();
    }
    bool contains(const T& elem) const {
        return find(elem) != end();
    }
    bool insert(const T& elem) {
        int insertion_index = binaryInsert(elem);
        if (insertion_index == -1) {
            return false;
        }
        if (set_size >= max_size) {
            expand();
        }
        shiftArrayRight (insertion_index);
        data[insertion_index] = new T(elem);
        set_size++;
        return true;
    }
    bool remove(const T& elem) {
        int to_remove = set_size;
        for (int i=0 ; i<set_size ; ++i) {
            if (*data[i] == elem) {
                to_remove = i;
                break;
            }
        }
        if (to_remove == set_size) {
            return false;
        }
        T* to_del = data[to_remove];
        shiftArrayLeft(to_remove);
        set_size--;
        delete to_del;
        return true;
    }
    int size() const {
        return set_size;
    }
    SortedSet& operator|=(const SortedSet& set) {
        iterator it = set.begin();
        while (it != set.end()) {
            this->insert(*it);
            ++it;
        }
        return *this;
    }
    SortedSet& operator&=(const SortedSet& set) {
        iterator it = begin();
        while (it != end()) {
            if (!set.contains(*it)) {
                bool result = this->remove(*it);
                if (result) {
                    it = begin();
                    continue;
                }
            }
            ++it;
        }
        return *this;
    }
    SortedSet& operator-=(const SortedSet& set) {
        iterator it = begin();
        while (it != end()) {
            if (set.contains(*it)) {
                bool result = this->remove(*it);
                if (result) {
                    it = begin();
                    continue;
                }
            }
            ++it;
        }
        return *this;
    }
private:
    /*
    * returns the insertion index
    */
    int binaryInsert(const T& elem) {
        if (set_size == 0)
            return 0;
        int lower_bound = 0;
        int upper_bound = set_size - 1;
        int current_index = 0;
        while (true) {
            current_index = (upper_bound + lower_bound) / 2;
            if (*data[current_index] == elem) {
                return -1;
            } else if (*data[current_index]<elem) {
                lower_bound = current_index + 1; // its in the upper
                if (lower_bound > upper_bound) {
                    return current_index + 1;
                }
            } else {
                upper_bound = current_index - 1; // its in the lower
                if (lower_bound > upper_bound) {
                    return current_index;
                }
            }
        }
    }
    /*
     * shifts the array from the end to the insertion index
     */
    void shiftArrayRight (const int index) {
        for (int i=set_size ; i>index ; --i) {
            data[i]=data[i-1];
        }
    }
    /*
     * shifts the array from the insertion index to the end of the array
     */
    void shiftArrayLeft (const int index) {
        for (int i=index ; i<set_size-1 ; ++i) {
            data[i]=data[i+1];
        }
    }

};
template<class T, class Compare>
SortedSet<T, Compare> operator&(const SortedSet<T, Compare>& a, const SortedSet<T, Compare>& b) {
    return SortedSet<T, Compare>(a) &= b;
}
template<class T, class Compare>
SortedSet<T, Compare> operator|(const SortedSet<T, Compare>& a, const SortedSet<T, Compare>& b) {
    return SortedSet<T, Compare>(a) |= b;
}

template<class T, class Compare>
SortedSet<T, Compare> operator-(const SortedSet<T, Compare>& a, const SortedSet<T, Compare>& b) {
    return SortedSet<T, Compare>(a) -= b;
}

template<class T, class Compare>
SortedSet<T, Compare> operator^(const SortedSet<T, Compare>& a, const SortedSet<T, Compare>& b) {
    return SortedSet<T, Compare>((a-b)|(b-a));
}
#endif //MTM4_SORTEDSET_H
