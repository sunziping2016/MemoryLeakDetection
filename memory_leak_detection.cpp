#include <iostream>
#include "hashset.h"
#include "log.h"

using namespace std;

struct extra_info {
	const char *file;
	long line;
	bool single;
};

size_t nnew = 0, ndelete = 0, nerror = 0;

hashset set;
bool memory_leak_detect = false;

void memory_leak_detect_on()
{
	if (!memory_leak_detect) {
		memory_leak_detect = true;
		clog << info << "INFO: memory leak detection on" << endl;
	}
}

void memory_leak_detect_off()
{
	if (memory_leak_detect) {
		memory_leak_detect = false;
		clog << info << "INFO: memory leak detection off" << endl;
	}
}

void memory_leak_detect_clear()
{
	nnew = 0;
	ndelete = 0;
	nerror = 0;
	set.clear();
	clog << info << "INFO: memory leak detection data clear" << endl;
}
void memory_leak_detect_report()
{
	hashset_iter iter = set.iter();
	while (iter.has_next()) {
		extra_info *einfo = (extra_info *)iter.value();
		clog << error << "ERROR: memory leak @" << (einfo + 1) << " (" << einfo->file << ":" << einfo->line << ")" << endl;
		++nerror;
		iter.next();
	}
	clog << info << "INFO: " << nnew << " new call(s)\t" << ndelete << " delete call(s)\t" << nerror << " errors" << endl;
}

void * operator new(std::size_t sz)
{
	void *ptr = malloc(sizeof(extra_info) + sz);
	extra_info *einfo = (extra_info *)ptr;
	einfo->file = "";
	einfo->line = 0;
	einfo->single = true;
	if (memory_leak_detect) {
		++nnew;
		clog << info << "INFO: new @" << (einfo + 1) << " (" << einfo->file << ":" << einfo->line << ")" << endl;
		set.add(ptr);
	}
	return (void *)(einfo + 1);
}

void * operator new[](std::size_t sz)
{
	void *ptr = malloc(sizeof(extra_info) + sz);
	extra_info *einfo = (extra_info *)ptr;
	einfo->file = "";
	einfo->line = 0;
	einfo->single = false;
	if (memory_leak_detect) {
		++nnew;
		clog << info << "INFO: new[] @" << (einfo + 1) << " (" << einfo->file << ":" << einfo->line << ")" << endl;
		set.add(ptr);
	}
	return (void *)(einfo + 1);
}

void * operator new(std::size_t sz, const char *file, long line)
{
	void *ptr = malloc(sizeof(extra_info) + sz);
	extra_info *einfo = (extra_info *)ptr;
	einfo->file = file;
	einfo->line = line;
	einfo->single = true;
	if (memory_leak_detect) {
		++nnew;
		clog << info << "INFO: new @" << (einfo + 1) << " (" << einfo->file << ":" << einfo->line << ")" << endl;
		set.add(ptr);
	}
	return (void *)(einfo + 1);
}

void * operator new[](std::size_t sz, const char *file, long line)
{
	void *ptr = malloc(sizeof(extra_info) + sz);
	extra_info *einfo = (extra_info *)ptr;
	einfo->file = file;
	einfo->line = line;
	einfo->single = false;
	if (memory_leak_detect) {
		++nnew;
		clog << info << "INFO: new[] @" << (einfo + 1) << " (" << einfo->file << ":" << einfo->line << ")" << endl;
		set.add(ptr);
	}
	return (void *)(einfo + 1);
}

void operator delete(void * ptr) noexcept
{
	extra_info *einfo = (extra_info *)ptr - 1;
	ptr = (void *)einfo;
	if (!memory_leak_detect) {
		free(ptr);
		return;
	}
	++ndelete;
	if (!set.contains(ptr)) {
		clog << error << "ERROR: delete wild pointer @" << (einfo + 1) << endl;
		++nerror;
	}
	else {
		if (!einfo->single) {
			clog << warning << "WARNING: mismatch delete on array pointer @" << (einfo + 1) << " (" << einfo->file << ":" << einfo->line << ")" << endl;
			++nerror;
		}
		clog << info << "INFO: delete @" << (einfo + 1) << " (" << einfo->file << ":" << einfo->line << ")" << endl;
		set.remove(ptr);
		free(ptr);
	}
}

void operator delete[](void * ptr) noexcept
{
	extra_info *einfo = (extra_info *)ptr - 1;
	ptr = (void *)einfo;
	if (!memory_leak_detect) {
		free(ptr);
		return;
	}
	++ndelete;
	if (!set.contains(ptr)) {
		clog << error << "ERROR: delete wild pointer @" << (einfo + 1) << endl;
		++nerror;
	}
	else {
		if (einfo->single) {
			clog << warning << "WARNING: mismatch delete[] on single-object pointer @" << (einfo + 1) << " (" << einfo->file << ":" << einfo->line << ")" << endl;
			++nerror;
		}
		clog << info << "INFO: delete[] @" << (einfo + 1) << " (" << einfo->file << ":" << einfo->line << ")" << endl;
		set.remove(ptr);
		free(ptr);
	}
}
