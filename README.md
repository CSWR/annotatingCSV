# Annotating CSV documents

This project is a proof-of-concept of a language for annotating documents.

## Dependencies

This program depends on the boost library. To install in Ubuntu, execute the
following command.

    sudo apt-get install libboost-all-dev

The rest of the dependencies are automatically downloaded and compiled when
building the project with `cmake`.

## Compilation

To compile, execute the following commands.

    $ cd build/
    $ cmake ..
    $ make

This will configure the project, download the dependencies, compile the source,
and generate the project binaries binaries at `/bin`, which are described in the
following section.

## Usage

The project includes two binaries, these are `navigational_bin`, which executes
navigational patterns, `datalog_bin` which executes programs written in the
modified datalog language with navigational patterns.

### Navigational language

The executable `bin/navigational_bin` computes and outputs all the spans in a
file that satisfy a given navigational pattern.

The program receives one argument: the path of the document that will be
scanned. It reads the pattern from the first line of the standard input.

Example:

    $ /bin/navigational_bin examples/alphabet.csv < examples/cell.nav

To benchmark, use the `-b` option:

    $ /bin/navigational_bin -b examples/alphabet.csv < examples/cell.nav

#### Navigational language syntax

Each navigational pattern is a list of expressions. These expressions can be
the following:

*   `any(S)`: Moves to every occurrence of `S` in the document.

*   `next(S)`: Moves to the next occurrence of `S` in the document.

*   `?x:next(S)`: Moves to the next occurrence of `S` in the document and stores
    the advanced text in the `x` variable, excluding `S`.

In these expressions `S` is the anchor, which is a list of word separated by
pipes (`|`). Special characters (`()|$^`) should be escaped with a backslash
(`\`).

The syntax for navigational patterns in EBNF is the following:

    navigational pattern = "/", expression, { "/", expression }, "/";
    expression           = "next", [ repeater ], word set
                         | "any", word set
                         | variable, ":next", word set;
    word set             = "(", word, { "|", word }, ")";
    repeater             = "{", integer, "}";
    variable             = "?", identifier;

For example navigational patterns, check the files at `examples/` with `.nav`
extension.

#### Special anchors

There are two special anchors: `^` and `$`. When used by themselves, these match
the beginning and the end of the document, respectively.

For example, the following pattern captures every line, including the first
one:

    /any(^|\n)/?x:next(\n)/

And the following pattern captures the whole document:

    /?x:next($)/

Escape these anchors with a backslash (`\`) to use them as normal characters.

#### Backrefences (experimental)

It is possible to define a navigational pattern that matches only when a
captured value is found again in another part of the pattern. This is usually
known as a backreference. These are specified using the following syntax:

    /any(\n)/?x:next(,)/any(\n)/&x:next(,)/

This pattern will match cells that are repeated within the first column.
Currently, this feature only works when using one variable and both appearances
of the variable come after an "any".

### Datalog language

The datalog language is a modification of classic datalog that uses navigational
patterns. The executable `bin/datalog_bin` computes and outputs the sets of
spans that satisfy a given datalog program.

The program receives two arguments: the path to the input document and the path
to the datalog program file. The input document path argument can be omitted if
it is specified in the datalog program.

Example:

    $ /bin/datalog_bin examples/alphabet.csv examples/cell.dat

To benchmark, use the `-b` option:

    $ /bin/datalog_bin -b examples/alphabet.csv examples/cell.dat

#### Datalog language syntax

Datalog programs consist of a set of rules. Each rule associates a set of spans
(predicate) with a list of clauses. The program evaluates clauses in the order
they are defined.

The clauses might be the following:

*   Navigational pattern: evaluates a navigational pattern according to the
    preceding section. Example: `/any(\n)/?x:next(\n)/`.

*   Predicate: filters the set of assignments using a previously defined
    predicate. Example: `P(?x)`.

*   Filter: filters the set of assignments using a regular expression.
    Example: `?x."[a-zA-Z]+"`.

*   Content equality: filters the set assignments comparing the content of two
    variables. Example: `&x = &y`.

Additionally, a preamble may be included to specify the input document and
define macro-like shortcuts.

Valid commands are the following:

*   Document path: specifies the input document. Example: `DOCUMENT
    "examples/alphabet.csv"`.

*   Anchor: defines a word set to use in navigational patterns. Example:
    `ANCHOR separator = ,|\n.`

*   Regular expression: defines a regular expression (see [re2 documentation][1]
    for valid syntax). Example: `REGEX number = [-+]?[0-9]+`.

The syntax for the datalog language in EBNF is the following:

    program          = { rule };
    rule             = predicate, ":-", clause, { ",", clause }, ";";
    predicate        = identifier, "(", variable, ")";
    clause           = navigational pattern
                     | predicate
                     | filter
                     | content equality;
    filter           = [ "not" ], variable, ".", '"', regular expression,'"';
    content equality = [ "not" ], "&", identifier, "=", "&", identifier;

For example datalog programs, check the files at `examples/` with `.dat`
extension.

---

Copyright (c) 2015 Arenas, Maturana, Riveros, Vrgoč.
Distributed under the MIT License (see LICENSE.txt).

[1]: https://github.com/google/re2/wiki/Syntax
