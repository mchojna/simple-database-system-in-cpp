#pragma once

#include <vector>
#include <string>

namespace Db {
    enum class ColumnType {
        TEXT=0, NUMBER=1
    };
    struct Column {
        std::string name;
        ColumnType type;
        std::vector<std::string> data = {};
    };

    struct Table {
        std::string name;
        std::vector<Column> columns = {};
    };

    struct Database {
        std::string name = "db1";
        std::vector<Table> tables = {};

        Database() = default;
        Database(const Database& other) = default;
        Database& operator=(const Database& other) = default;
        ~Database() = default;

        auto createTable(std::string const& tableName, std::vector<Column> const& columns) -> void;
        auto renameTable(std::string const& oldTableName, std::string const& newTableName) -> void;
        auto dropTable(std::string const& tableName) -> void;

        auto addColumn(std::string const& tableName, Column const& column) -> void;
        auto renameColumn(std::string const& tableName, std::string const& oldColumnName, std::string const& newColumnName) -> void;
        auto removeColumn(std::string const& tableName, std::string const& columnName) -> void;

        auto insertRow(std::string const& tableName, std::vector<std::string> const& row) -> void;
        auto updateRow(std::string const& tableName, std::string const& columnName, std::string const& newValue, std::string const& conditionColumnName, std::string const& condition, std::string const& conditionValue) -> void;
        auto removeRow(std::string const& tableName, std::string const& conditionColumnName, std::string const& condition, std::string const& conditionValue) -> void;

        auto writeToFile(std::fstream& file) const -> void;
        auto readFromFile(std::fstream& file) -> void;
    };

    struct Parser {
        Database& database;

        auto parseQuery(std::string const& query) -> void;
        auto parseWhereQuery(std::stringstream& stream, Table& table, std::vector<int>& rows) -> void;
        auto parseOrderByQuery(std::stringstream& stream, Table& table, std::vector<int>& rows) -> void;
        auto parseSelectQuery(std::stringstream& stream) -> void;
    };

    namespace Utils {
        auto tableExists(Database const& database, std::string const& table) -> bool;
        auto columnExists(Table const& table, std::string const& column) -> bool;
        auto valueExists(Column const& column, std::string const& value) -> bool;
        auto getTable(Database& database, std::string const& name) -> std::vector<Table>::iterator;
        auto getColumn(Table& table, std::string const& name) -> std::vector<Column>::iterator;
        auto uniqueColumns(std::vector<Column> const& column) -> bool;
        auto uniqueValue(Column const& column, std::string const& value) -> bool;
        auto getNumberOfTables(Database const& database) -> int;
        auto getNumberOfColumns(Table const& table) -> int;
        auto getNamesOfTables(Database const& database) -> std::string;
        auto getNamesOfColumns(Table const& table) -> std::string;
        auto printTable(Table const& table, std::vector<std::string> columns, std::vector<int> rows) -> void;
        auto evaluateCondition(Table& table, const std::string& columnName, const std::string& condition, const std::string& value, int row) -> bool;
        auto isNumber(const std::string& str) -> bool;
        auto checkColumnType(const Column& column, const std::string& value) -> void;
    }
}
