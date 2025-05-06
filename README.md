# Simple Database System in C++

This project implements a simple database management system in C++ that supports basic operations for managing tables, columns, and rows. It also includes a custom query language for interacting with the database.

## Features

### Data Model

- **Database**: Contains a name and a collection of tables.
- **Table**: Contains a name and a collection of columns.
- **Column**: Contains a name, type (`TEXT` or `NUMBER`), and a collection of data values.

### Query Language

The system supports a custom query language for performing the following operations:

#### Data Definition Language (DDL)

- **Create a table**:

    ```plaintext
    CREATE_TABLE table_name column_name1 column_type1 [column_name2 column_type2 ...]
    ```

    Example:

    ```plaintext
    CREATE_TABLE tab1 col1 TEXT
    CREATE_TABLE tab2 col1 TEXT col2 NUMBER col3 TEXT
    ```

- **Rename a table**:

    ```plaintext
    RENAME_TABLE old_table_name new_table_name
    ```

    Example:

    ```plaintext
    RENAME_TABLE tab1 tab3
    ```

- **Drop a table**:

    ```plaintext
    DROP_TABLE table_name
    ```

    Example:

    ```plaintext
    DROP_TABLE tab1
    ```

- **Add a column**:

    ```plaintext
    ALTER_TABLE table_name ADD_COLUMN column_name column_type
    ```

    Example:

    ```plaintext
    ALTER_TABLE tab2 ADD_COLUMN col4 TEXT
    ```

- **Rename a column**:

    ```plaintext
    ALTER_TABLE table_name RENAME_COLUMN old_column_name new_column_name
    ```

    Example:

    ```plaintext
    ALTER_TABLE tab2 RENAME_COLUMN col4 col5
    ```

- **Drop a column**:

    ```plaintext
    ALTER_TABLE table_name DROP_COLUMN column_name
    ```

    Example:

    ```plaintext
    ALTER_TABLE tab2 DROP_COLUMN col5
    ```

#### Data Manipulation Language (DML)

- **Insert a row**:

    ```plaintext
    ALTER_TABLE table_name INSERT_ROW value1 value2 ...
    ```

    Example:

    ```plaintext
    ALTER_TABLE tab2 INSERT_ROW aaa 123 bbb
    ```

- **Update rows**:

    ```plaintext
    ALTER_TABLE table_name UPDATE_ROW column_name new_value [WHERE condition_column operator condition_value]
    ```

    Example:

    ```plaintext
    ALTER_TABLE tab2 UPDATE_ROW col1 new_value WHERE col2 > 100
    ```

- **Delete rows**:

    ```plaintext
    ALTER_TABLE table_name DELETE_ROW WHERE condition_column operator condition_value
    ```

    Example:

    ```plaintext
    ALTER_TABLE tab2 DELETE_ROW WHERE col1 == aaa
    ```

#### Data Query Language (DQL)

- **Select data**:

    ```plaintext
    SELECT column1 [column2 ...] | * FROM table_name [WHERE conditions] [ORDER_BY column1 [ASC|DESC] ...]
    ```

    Example:

    ```plaintext
    SELECT * FROM tab1
    SELECT col1 col2 FROM tab2 WHERE col2 > 100 ORDER_BY col1 ASC
    ```

#### Other Commands

- **Save database to file**:

    ```plaintext
    WRITE_DATABASE file_path
    ```

    Example:

    ```plaintext
    WRITE_DATABASE db.txt
    ```

- **Load database from file**:

    ```plaintext
    READ_DATABASE file_path
    ```

    Example:

    ```plaintext
    READ_DATABASE db.txt
    ```

- **List table names**:

    ```plaintext
    TABLES_NAMES
    ```

- **List column names**:

    ```plaintext
    COLUMNS_NAMES table_name
    ```

- **Count tables**:

    ```plaintext
    TABLES_COUNT
    ```

- **Count columns**:

    ```plaintext
    COLUMNS_COUNT table_name
    ```

- **Rename database**:

    ```plaintext
    RENAME_DATABASE new_name
    ```

## Build Instructions

Prerequisites

- C++20 compiler
- CMake 3.29 or higher

Steps

1. Clone the repository:

    ```bash
    git clone https://github.com/your-username/simple-database-system-in-cpp.git
    cd simple-database-system-in-cpp
    ```

2. Build the project:

    ```bash
    cmake -B build
    cmake --build build
    ```

3. Run the executable:

    ```bash
    ./build/simple_database
    ```

## Usage

After running the program, you can enter commands interactively. Type `exit` to quit the program.

Example session:

```plaintext
> CREATE_TABLE tab1 col1 TEXT col2 NUMBER
> ALTER_TABLE tab1 INSERT_ROW hello 123
> SELECT * FROM tab1
+---------------+---------------+
| col1          | col2          |
+---------------+---------------+
| hello         | 123           |
+---------------+---------------+
> exit
```

## Dependencies

- fmt (included via CMake FetchContent)

## Acknowledgments

Inspired by basic database management concepts
