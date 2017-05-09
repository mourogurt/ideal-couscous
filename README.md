# ideal-couscous
Ideal Couscous is header only library, that providing compile-time reflection utility for c++ (with some limitation)

1 Licence
==========
Apache License 2.0

2 Prerequisites
===============
- C++1z (clang 3.9+)
- Boost.Hana

3 Limitations
==============
- No inheritance
- No variable/method flags (public, private, protected). Reflected private/protected methods/vars can be called outside of class
- Constructors can't be reflected
- No signals/slots
- No paramters
- No enums

4 Usage examples
=================
You can find examples in [examples/tutorial](https://github.com/maxis11/ideal-couscous/tree/master/examples/tutorial) directory to learn how library work
