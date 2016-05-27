# Introduction
This project consist of following greate parts designed and developed by me:

* `my_list.h`: Simple but generic and STL-compatible container and its corresponding iterertor.

* `hashset.h hashset.cpp`: A hashset using only C memory allocating function featuring
auto expansion and good collision handling.

* `memory_leak_detection.h memory_leak_detection.cpp`: overloaded global new and delete function
with placement-new expression. Featuring memory problem auto detecion. Using hashset to store
the data.

* `log.h`: Two enhanced buffer class which is fully compatible with iostream (cin, cout).
Teebuf featuring redirecting output to multiple ostream buffer. Logbuf featuring switch output
between multiple ostream buffer with IO manipulators.

