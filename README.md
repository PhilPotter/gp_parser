# gp_parser

C++ based parser for the Guitar Pro 5 file format.

This is based upon the parse-gp5 Javascript codebase written by Julian Gruber. I ported it to C++ and added XML and C++ object export functionality. The plan is to continue to work on it. I hope people find it useful :-) To include in a C++ project, just incude the header and add the cpp files to your chosen build system/make file. 

## Usage

```cpp
// Instantiate parser
gp_parser::Parser parser("/home/johnsmith/path_to_tab.gp5");

// Parser XML format is returned via std::string
std::cout << parser.getXML();

// Object containing sub-properties - see gp_parser.h for definitions
auto tabFile = parser.getTabFile(); 
```

# Thanks

Thank you to my buddy Stuart for inspiration, and also to Julian Gruber for the original codebase. It is very concise and was easy to follow.

# License

MIT
