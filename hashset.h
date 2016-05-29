#ifndef HASHSET_H_INCLUDE
#define HASHSET_H_INCLUDE

#include <cstdint>

class hashset;

class hashset_iter
{
public:
    hashset_iter(const hashset &set);
    const void *value() const;
    bool has_next() const;
    void next();
private:
    const hashset &set;
    std::size_t index;
};


class hashset
{
    friend class hashset_iter;
public:
    hashset();
    ~hashset();

    void add(const void *item);
    void remove(const void *item);
    void clear();

    bool contains(const void *item) const;
    hashset_iter iter() const;
protected:
    std::size_t capacity() const
    {
        return (std::size_t) (1u << nbits);
    }
    std::size_t mask() const
    {
        return (std::size_t) ((1u << nbits) - 1);
    }
    std::size_t hash(std::uintptr_t value) const
    {
        return mask() & (PRIME1 * value);
    }
    std::size_t next_hash(std::size_t value) const
    {
        return mask() & (value + PRIME2);
    }
    void maybe_rehash();
    void add_member(const void *item);
private:
    static const unsigned int PRIME1;
    static const unsigned int PRIME2;

    std::size_t nbits;
    std::uintptr_t *items;
    std::size_t nitems;
};

#endif
