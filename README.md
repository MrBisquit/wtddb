# WTD DB
A small custom database because why not

There's comments everywhere so hopefully it should be easy and straightforward to understand how
everything works, even if you don't know C

## Building
On windows, use `build.bat`, on linux, use `build.sh`. There are instructions at the top of each file.
You will need `vcpkg` to build.
After building, the executable should *magically* appear in the `bin` directory, unless it doesn't, and
in which case it probably didn't compile.

## Useful links
https://cstack.github.io/db_tutorial/parts/part1.html

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