#ifndef POLYINDEX_H
#define POLYINDEX_H
#include <type_traits>
#include <vector>
#include <bitset>
#include <stdexcept>

//Holds pointers of derived from Base classes and allow's to access them by type.
template<typename Base, size_t N>
class PolyIndex {
public:
    template<typename Derived>
    void add(Derived& elem) {
        static_assert(std::is_base_of<Base, Derived>::value);
        if (getDerivedClassID<Derived>() >= N) {
            throw std::out_of_range{"There is more derived from Base classes then N parameter."};
        }
        pointers[getDerivedClassID<Derived>()] = static_cast<Base*>(&elem);
        membership[getDerivedClassID<Derived>()] = true;
    }

    template<typename Derived>
    bool has() const {
        static_assert(std::is_base_of<Base, Derived>::value);
        return membership[getDerivedClassID<Derived>()];
    }
    template<typename Derived>
    Derived& get() const {
        static_assert(std::is_base_of<Base, Derived>::value);
        if (empty() || !has<Derived>()) {
            throw std::out_of_range{""};
        }
        Base* ptr = pointers[getDerivedClassID<Derived>()];
        return *static_cast<Derived*>(ptr);
    }
    size_t count() const {
        return membership.count();
    }
    bool empty() const {
        return membership.none();
    }
private:
    static inline size_t callCounter() {
        static size_t count = 0;
        return count++;
    }
    template<typename Derived> static inline size_t getDerivedClassID() noexcept {
        static_assert(std::is_base_of<Base, Derived>::value);
        static size_t id = callCounter();
        return id;
    }
    std::array<Base*, N> pointers;
    std::bitset<N> membership;
};

#endif
