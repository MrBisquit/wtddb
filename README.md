# WTD DB
A small custom database because why not

There's comments everywhere so hopefully it should be easy and straightforward to understand how
everything works, even if you don't know C

## Building
On windows, use `build.bat`, on linux, use `build.sh`. There are instructions at the top of each file.
You will need `vcpkg` to build.
After building, the executable should *magically* appear in the `bin` directory, unless it doesn't, and
in which case it probably didn't compile.

## Features
- [ ] REPL
  - [x] Basic I/O commands (E.g. `.exit`)
  - [x] ANSI highlighting (Using [MrBisquit/ansi_console_v2](https://github.com/MrBisquit/ansi_console_v2))
  - [ ] Parsing SQL queries
- [ ] SQL (Using the [ISO/IEC 9075](https://en.wikipedia.org/wiki/ISO/IEC_9075) standard as a base)
- [ ] Database internals
  - [ ] Table schemas
  - [ ] Table indexes
  - [ ] Table data
- [ ] Functionality
  - [ ] Creating tables + schemas
  - [ ] Deleting tables + schemas
  - [ ] Updating table schemas
  - [ ] Duplicating tables (with their data)
  - [ ] Generating indexes
  - [ ] Regenerating indexes
  - [ ] Searching indexes
  - [ ] Removing indexes

## Useful links
- https://cstack.github.io/db_tutorial/parts/part1.html
- [Build Your Own Database (https://blog.wtdawson.info/programming/building-a-database-from-scratch-in-c-part-1)](https://go.wtdawson.info/byodb)

## Design
Below is a table which shows the different "chunks" of the file, I tried to design the database
file in a way so that the parts that are modified less come earlier in the file, so data does not
have to be moved around as much.

<table>
    <tbody>
        <tr>
            <th colspan="2">Database file</th>
        </tr>
        <!-- Basic db metadata -->
        <tr>
            <td>
                Database metadata header
            </td>
            <td>
                Contains metadata for the db, including pointers to other headers
            </td>
        </tr>
        <tr>
            <td>
                Database config header
            </td>
            <td>
                Contains basic db config
            </td>
        </tr>
        <!-- Schemas -->
        <tr>
            <th colspan="2">
                Schema section
            </th>
        </tr>
        <tr>
            <td>
                Schema metadata header
            </td>
            <td>
                Contains metadata for the schemas, mainly holding information
                about how many schemas there are
            </td>
        </tr>
        <tr>
            <td></td>
            <td>
                These are the least likely to change, which is why they are so high up
                in the file
            </td>
        </tr>
        <tr>
            <td colspan="2">
                Schemas
            </td>
        </tr>
        <!-- Indexes -->
        <tr>
            <th colspan="2">
                Index section
            </th>
        </tr>
        <tr>
            <td>
                Index metadata header
            </td>
            <td>
                Contains metadata for the indexes, mainly holding information
                about how many indexes there are
            </td>
        </tr>
        <tr>
            <td></td>
            <td>
                Indexes are semi-variable width, because they can be wide, but they
                are statically sized based on the index metadata, so they are sort of
                variable-width, while being statically sized
            </td>
        </tr>
        <tr>
            <td></td>
            <td>
                Indexes are allocated in chunks, rather than individually. Rather than
                adding 1 on, it instead adds a chunk of empty indexes.
            </td>
        </tr>
        <tr>
            <td colspan="2">
                Indexes
            </td>
        </tr>
        <!-- Tables -->
        <tr>
            <th colspan="2">
                Index section
            </th>
        </tr>
        <tr>
            <td>
                Tables metadata header
            </td>
            <td>
                Contains metadata for the tables, mainly holding information
                about how many tables there are
            </td>
        </tr>
        <tr>
            <td colspan="2">
                Tables
            </td>
        </tr>
    </tbody>
</table>

If you delete a table that's in between 2 other tables, it can cause indexing issues,
because the indexes need to be updated, so nothing is pointing to blank spaces.

### Schemas
<table>
    <tbody>
        <!-- Schemas -->
        <tr>
            <th colspan="2">
                Schema section
            </th>
        </tr>
        <tr>
            <td>
                Schema metadata header
            </td>
            <td>
                Contains metadata for the schemas, mainly holding information
                about how many schemas there are
            </td>
        </tr>
        <tr>
            <th colspan="2">
                Schema A (Example)
            </th>
        </tr>
        <tr>
            <td>Schema head</td>
            <td>
                This is a small header, containing the index of the linked table,
                and number of items
            </td>
        </tr>
        <tr>
            <td colspan="2">
                Schema item 1
            </td>
        </tr>
        <tr>
            <td colspan="2">
                Schema item 2
            </td>
        </tr>
        <tr>
            <td colspan="2">
                Schema item 3
            </td>
        </tr>
        <tr>
            <td colspan="2">
                Etc
            </td>
        </tr>
    </tbody>
</table>

### Indexes

### Tables