#include <cstdlib>
#include "hashset.h"

using namespace std;

hashset_iter::hashset_iter(const hashset &set)
	: set(set),
	index(0)
{
	if (set.items[index] == 0 || set.items[index] == 1)
		next();
}

const void *hashset_iter::value() const
{
	return (const void *)set.items[index];
}


bool hashset_iter::has_next() const
{
	if (set.nitems == 0 || index == set.capacity())
		return false;
	size_t ii = index;
	while (ii < set.capacity()) {
		if (set.items[ii] != 0 && set.items[ii] != 1)
			return true;
		++ii;
	}
	return false;
}

void hashset_iter::next()
{
	if (index == set.capacity())
		return;
	do {
		++index;
	} while ((set.items[index] == 0 || set.items[index] == 1) && index < set.capacity());
}

const unsigned int hashset::PRIME1 = 73;
const unsigned int hashset::PRIME2 = 5009;

hashset::hashset()
	: nbits(3),
	items((uintptr_t *)calloc(capacity(), sizeof(uintptr_t))),
	nitems(0)
{}

hashset::~hashset()
{
	free(items);
}

void hashset::add_member(const void *item)
{
	uintptr_t value = (uintptr_t)item;
	if (value == 0 || value == 1)
		return;
	size_t ii = hash(value);
	while (items[ii] != 0 && value != 1) {
		if (items[ii] == value)
			return;
		else
			ii = next_hash(ii);
	}
	nitems++;
	items[ii] = value;
}

void hashset::maybe_rehash()
{
	if (nitems > (size_t)(capacity() * 0.6)) {
		uintptr_t *old_items;
		size_t old_capacity;
		old_items = items;
		old_capacity = capacity();
		nbits++;
		items = (uintptr_t *)calloc(capacity(), sizeof(uintptr_t));
		nitems = 0;
		for (size_t ii = 0; ii < old_capacity; ii++)
			add_member((const void *)old_items[ii]);
		free(old_items);
	}
}


void hashset::add(const void *item)
{
	add_member(item);
	maybe_rehash();
}

void hashset::remove(const void *item)
{
	uintptr_t value = (uintptr_t)item;
	size_t ii = hash(value);
	while (items[ii] != 0) {
		if (items[ii] == value) {
			items[ii] = 1;
			nitems--;
			return;
		}
		else
			ii = next_hash(ii);
	}
}

void hashset::clear()
{
	free(items);
	nbits = 3;
	items = (uintptr_t *)calloc(capacity(), sizeof(uintptr_t));
	nitems = 0;
}

bool hashset::contains(const void *item) const
{
	size_t value = (size_t)item;
	if (value == 0 || value == 1)
		return false;
	size_t ii = hash(value);
	while (items[ii] != 0) {
		if (items[ii] == value)
			return true;
		else
			ii = next_hash(ii);
	}
	return false;
}

hashset_iter hashset::iter() const
{
	return hashset_iter(*this);
}
