#include <algorithm>
#include <cctype>
#include <fmt/ranges.h>
#include <fstream>
#include <iostream>
#include <ranges>
#include <set>
#include <string>
#include <sstream>
#include <vector>


#include "db.hpp"

namespace Db {
    namespace Utils {
        auto tableExists(Database const& database, std::string const& table) -> bool {
            return std::ranges::find_if(database.tables.begin(), database.tables.end(),
                    [&table](Table const& tab) -> bool {return tab.name == table;}
                    ) != database.tables.end();
        };
        auto columnExists(Table const& table, std::string const& column) -> bool {
            return std::ranges::find_if(table.columns.begin(), table.columns.end(),
                [&column](Column const& col) -> bool {return col.name == column;}
                ) != table.columns.end();
        };
        auto valueExists(Column const& column, std::string const& value) -> bool {
            return std::ranges::find_if(column.data.begin(), column.data.end(),
                [&value](std::string const& val) -> bool {return val == value;}
                ) != column.data.end();
        };
        auto getTable(Database& database, std::string const& name) -> std::vector<Table>::iterator {
            return std::ranges::find_if(database.tables,
                [&name](Table const& table) {return table.name == name;});
        };
        auto getColumn(Table& table, std::string const& name) -> std::vector<Column>::iterator {
            return std::ranges::find_if(table.columns.begin(), table.columns.end(),
                [&name](Column const& col) {return col.name == name;});
        };
        auto uniqueColumns(std::vector<Column> const& columns) -> bool {
            auto columnNames = std::vector<std::string>();
            auto uniqueColumnNames = std::set<std::string>();
            for(auto const& column: columns) {
                columnNames.push_back(column.name);
                uniqueColumnNames.insert(column.name);
            }
            return columnNames.size() == uniqueColumnNames.size();
        };
        auto uniqueValue(Column const& column, std::string const& value) -> bool {
            return std::ranges::find_if(column.data.begin(), column.data.end(),
                [&value](std::string const& data) -> bool { return data == value; }
                ) != column.data.end();
        };
        auto getNumberOfTables(Database const& database) -> int {
            return database.tables.size();
        };
        auto getNumberOfColumns(Table const& table) -> int {
            return table.columns.size();
        };
        auto getNamesOfTables(Database const& database) -> std::string {
            auto names = std::string();
            for(auto const& table: database.tables) {
                names += table.name + " ";
            }
            return names.substr(0, names.size() - 1);
        };
        auto getNamesOfColumns(Table const& table) -> std::string {
            auto names = std::string();
            for(auto const& column: table.columns) {
                names += column.name + " ";
            }
            return names.substr(0, names.size() - 1);
        };
        auto printTable(Table& table, std::vector<std::string> const& columns, std::vector<int> const& rows) -> void {
            for(auto i = 0; i < columns.size(); i++) {
                fmt::print("+");
                for(auto j = 0; j < 15; j++) {
                    fmt::print("-");
                }
            }
            fmt::print("+");
            fmt::println("");

            for(const auto& col : columns) {
                fmt::print("|{:<15}", col);
            }
            fmt::print("|\n");

            for(auto index: rows) {
                for(auto i = 0; i < columns.size(); i++) {
                    fmt::print("+");
                    for(auto j = 0; j < 15; j++) {
                        fmt::print("-");
                    }
                }
                fmt::print("+");
                fmt::println("");

                for(const auto& columnName : columns) {
                    auto& column = *Utils::getColumn(table, columnName);
                    fmt::print("|{:<15}", column.data[index]);
                }
                fmt::print("|\n");
            }

            for(auto i = 0; i < columns.size(); i++) {
                fmt::print("+");
                for(auto j = 0; j < 15; j++) {
                    fmt::print("-");
                }
            }
            fmt::print("+");
            fmt::println("");
        };
        auto evaluateCondition(Table& table, const std::string& columnName, const std::string& condition, const std::string& conditionValue, int row) -> bool {
            auto column = *getColumn(table, columnName);
            auto value = column.data[row];

            if(column.type == ColumnType::NUMBER) {
                auto numConditionValue = std::stod(conditionValue);
                auto numValue = std::stod(value);

                if(condition == ">") {
                    return numValue > numConditionValue;
                }
                if(condition == ">=") {
                    return numValue >= numConditionValue;
                }
                if(condition == "<") {
                    return numValue < numConditionValue;
                }
                if(condition == "<=") {
                    return numValue <= numConditionValue;
                }
                if(condition == "==") {
                    return numValue == numConditionValue;
                }
                if(condition == "!=") {
                    return numValue != numConditionValue;
                }
            } else {
                if(condition == "==") {
                    return value == conditionValue;
                }
                if(condition == "!=") {
                    return value != conditionValue;
                }
                if(condition == ">") {
                    return value > conditionValue;
                }
                if(condition == ">=") {
                    return value >= conditionValue;
                }
                if(condition == "<") {
                    return value < conditionValue;
                }
                if(condition == "<=") {
                    return value <= conditionValue;
                }
            }

            return false;
        };
        auto isNumber(const std::string& str) -> bool {
            try {
                std::stod(str);
                return true;
            } catch (...) {
                return false;
            }
        }
        auto validateColumnType(const Column& column, const std::string& value) -> void {
            if (column.type == ColumnType::NUMBER && !isNumber(value)) {
                throw std::invalid_argument("Value '" + value + "' is not a valid number for column '" + column.name + "'");
            }
        }
    }

    auto Database::createTable(std::string const& tableName, std::vector<Column> const& columns) -> void {
        this->tables.push_back({tableName, columns});
    }
    auto Database::renameTable(std::string const& oldTableName, std::string const& newTableName) -> void {
        auto& table = *Utils::getTable(*this, oldTableName);
        table.name = newTableName;
    }
    auto Database::dropTable(std::string const& tableName) -> void {
        this->tables.erase(Utils::getTable(*this, tableName));
    }

    auto Database::addColumn(std::string const& tableName, Column const& column) -> void {
        auto& table = *Utils::getTable(*this, tableName);
        table.columns.push_back(column);
    }
    auto Database::renameColumn(std::string const& tableName, std::string const& oldColumnName, std::string const& newColumnName) -> void {
        auto& table = *Utils::getTable(*this, tableName);
        auto& column = *Utils::getColumn(table, oldColumnName);
        column.name = newColumnName;
    }
    auto Database::removeColumn(std::string const& tableName, std::string const& columnName) -> void {
        auto& table = *Utils::getTable(*this, tableName);
        auto column = Utils::getColumn(table, columnName);
        table.columns.erase(column);
    }

    auto Database::insertRow(std::string const& tableName, std::vector<std::string> const& row) -> void {
        auto& table = *Utils::getTable(*this, tableName);
        for (auto i = 0; i < table.columns.size(); ++i) {
            table.columns[i].data.push_back(row[i]);
        }
    }
    auto Database::updateRow(std::string const& tableName, std::string const& columnName, std::string const& newValue, std::string const& conditionColumnName, std::string const& condition, std::string const& conditionValue) -> void {
        auto& table = *Utils::getTable(*this, tableName);
        auto& column = *Utils::getColumn(table, columnName);

        Utils::validateColumnType(column, newValue);

        if (!conditionColumnName.empty()) {
            auto& conditionColumn = *Utils::getColumn(table, conditionColumnName);
            for (auto i = 0; i < conditionColumn.data.size(); ++i) {
                if (Utils::evaluateCondition(table, conditionColumnName, condition, conditionValue, i)) {
                    column.data[i] = newValue;
                }
            }
        } else {
            std::ranges::fill(column.data.begin(), column.data.end(), newValue);
        }
    }
    auto Database::removeRow(std::string const& tableName, std::string const& conditionColumnName, const std::string& condition, std::string const& conditionValue) -> void {
        auto& table = *Utils::getTable(*this, tableName);
        auto indicesToRemove = std::vector<int>();

        for (auto i = 0; i < table.columns[0].data.size(); ++i) {
            if (Utils::evaluateCondition(table, conditionColumnName, condition, conditionValue, i)) {
                indicesToRemove.push_back(i);
            }
        }

        for (auto& column : table.columns) {
            for (auto& offset : std::ranges::reverse_view(indicesToRemove)) {
                column.data.erase(column.data.begin() + offset);
            }
        }
    }

    auto Database::writeToFile(std::fstream& file) const -> void {
        file << this->name << '\n';
        file << this->tables.size() << '\n';

        for (auto const& table : tables) {
            file << table.name << '\n';
            file << table.columns.size() << '\n';

            for (auto const& column : table.columns) {
                file << column.name << '\n';
                file << static_cast<int>(column.type) << '\n';
                file << column.data.size() << '\n';
                for (auto const& value : column.data) {
                    file << value << '\n';
                }
            }
        }
    }
    auto Database::readFromFile(std::fstream& file) -> void {
        this->tables.clear();
        std::getline(file, this->name);

        auto tableCount = std::string();
        std::getline(file, tableCount);

        for(auto i = 0; i < std::stoi(tableCount); ++i) {
            auto table = Table{""};
            std::getline(file, table.name);

            auto columnCount = std::string();
            std::getline(file, columnCount);

            for(auto j = 0; j < std::stoi(columnCount); ++j) {
                auto column = Column{"", ColumnType::TEXT};
                std::getline(file, column.name);

                auto columnType = std::string();
                std::getline(file, columnType);
                column.type = static_cast<ColumnType>(std::stoi(columnType));

                auto dataSize = std::string();
                std::getline(file, dataSize);

                for(auto k = 0; k < std::stoi(dataSize); ++k) {
                    auto value = std::string();
                    std::getline(file, value);
                    column.data.push_back(value);
                }
                table.columns.push_back(column);
            }
            this->tables.push_back(table);
        }
    }

    auto Parser::parseQuery(std::string const& query) -> void {
        auto stream = std::stringstream(query);
        auto command = std::string();
        stream >> command;
        std::ranges::transform(command.begin(), command.end(), command.begin(), toupper);

        if (command == "CREATE_TABLE") {
            auto tableName = std::string();
            stream >> tableName;
            if (Utils::tableExists(database, tableName)) {
                throw std::invalid_argument(fmt::format("Table '{}' already exists in database.", tableName));
            }

            auto columns = std::vector<Column>();
            auto columnName = std::string();
            auto type = std::string();
            while (stream >> columnName >> type) {
                std::ranges::transform(type.begin(), type.end(), type.begin(), toupper);
                auto columnType = (type == "NUMBER" ? ColumnType::NUMBER : ColumnType::TEXT);
                columns.push_back({columnName, columnType});
            }
            if(!Utils::uniqueColumns(columns)) {
                throw std::invalid_argument(fmt::format("Columns should have unique names."));
            }

            database.createTable(tableName, columns);
            fmt::println("Table '{}' created in database.", tableName);
        }
        else if (command == "RENAME_TABLE") {
            auto oldTableName = std::string();
            stream >> oldTableName;
            if (!Utils::tableExists(database, oldTableName)) {
                throw std::invalid_argument(fmt::format("Table '{}' does not exists in database.", oldTableName));
            }

            auto newTableName = std::string();
            stream >> newTableName;
            if (Utils::tableExists(database, newTableName)) {
                throw std::invalid_argument(fmt::format("Table '{}' already exists in database.", newTableName));
            }

            database.renameTable(oldTableName, newTableName);
            fmt::println("Table '{}' renamed to '{}'.", oldTableName, newTableName);
        }
        else if (command == "DROP_TABLE") {
            auto tableName = std::string();
            stream >> tableName;
            if (!Utils::tableExists(database, tableName)) {
                throw std::invalid_argument(fmt::format("Table '{}' does not exists in database.", tableName));
            }

            database.dropTable(tableName);
            fmt::println("Table '{}' dropped from database.", tableName);
        }
        else if (command == "ALTER_TABLE") {
            auto tableName = std::string();
            auto operation = std::string();
            stream >> tableName >> operation;
            std::ranges::transform(operation.begin(), operation.end(), operation.begin(), ::toupper);

            if (!Utils::tableExists(database, tableName)) {
                throw std::invalid_argument(fmt::format("Table '{}' does not exists in database.", tableName));
            }

            auto table = Utils::getTable(database, tableName);

            if (operation == "ADD_COLUMN") {
                auto columnName = std::string();
                stream >> columnName;
                if(Utils::columnExists(*table, columnName)) {
                    throw std::invalid_argument(fmt::format("Column '{}' already exists in table '{}'.", columnName, tableName));
                }

                auto type = std::string();
                stream >> type;
                std::ranges::transform(type.begin(), type.end(), type.begin(), toupper);
                auto columnType = (type == "NUMBER" ? ColumnType::NUMBER : ColumnType::TEXT);

                auto data = std::vector<std::string>();
                if(!table->columns.empty() && !table->columns[0].data.empty()) {
                    data = std::vector<std::string>(table->columns[0].data.size(), "");
                }

                database.addColumn(tableName, {columnName, columnType, data});
                fmt::println("Column '{}' added to table '{}.", columnName, tableName);
            }
            else if (operation == "RENAME_COLUMN") {
                auto oldColumnName = std::string();
                stream >> oldColumnName;
                if(!Utils::columnExists(*table, oldColumnName)) {
                    throw std::invalid_argument(fmt::format("Column '{}' does not exists in table '{}'.", oldColumnName, tableName));
                }

                auto newColumnName = std::string();
                stream >> newColumnName;
                if(Utils::columnExists(*table, newColumnName)) {
                    throw std::invalid_argument(fmt::format("Column '{}' already exists in table '{}'.", newColumnName, tableName));
                }

                database.renameColumn(tableName, oldColumnName, newColumnName);
                fmt::println("Column '{}' renamed to '{}' in table '{}'.", oldColumnName, newColumnName, tableName);
            }
            else if (operation == "DROP_COLUMN") {
                auto columnName = std::string();
                stream >> columnName;
                if(!Utils::columnExists(*table, columnName)) {
                    throw std::invalid_argument(fmt::format("Column '{}' does not exists in table '{}'.", columnName, tableName));
                }

                database.removeColumn(tableName, columnName);
                fmt::println("Column '{}' removed from table '{}'.", columnName, tableName);
            }
            else if (operation == "INSERT_ROW") {
                auto row = std::vector<std::string>();
                auto value = std::string();
                while (stream >> value) {
                    row.push_back(value);
                }

                if (row.size() != table->columns.size()) {
                    throw std::invalid_argument(fmt::format("Row has '{}' values but table has '{}' columns.", row.size(), table->columns.size()));
                }

                for (auto i = 0; i < table->columns.size(); ++i) {
                    auto& column = table->columns[i];
                    const auto& rowValue = row[i];
                    try {
                        if (column.type == ColumnType::NUMBER) {
                            std::stod(rowValue);
                        }
                    } catch (...) {
                        throw std::invalid_argument(fmt::format("Value '{}' is not of type '{}' in column '{}'.", rowValue, static_cast<int>(column.type), column.name));
                    }
                }

                database.insertRow(tableName, row);
                fmt::println("Row inserted to table '{}'.", tableName);
            }
            else if (operation == "UPDATE_ROW") {
                auto columnName = std::string();
                auto value = std::string();
                auto where = std::string();
                auto conditionalColumnName = std::string();
                auto conditionalOperation = std::string();
                auto conditionalValue = std::string();
                stream >> columnName >> value >> where  >> conditionalColumnName >> conditionalOperation >> conditionalValue;

                database.updateRow(tableName, columnName, value, conditionalColumnName, conditionalOperation, conditionalValue);
                fmt::println("Row updated in table '{}'.", tableName);
            }
            else if (operation == "DELETE_ROW") {
                auto where = std::string();
                auto conditionalColumnName = std::string();
                auto conditionalOperation = std::string();
                auto conditionalValue = std::string();
                stream >> where >> conditionalColumnName >> conditionalOperation >> conditionalValue;

                if (conditionalColumnName.empty() || conditionalValue.empty()) {
                    throw std::invalid_argument("Condition column name and value must be provided for row deletion.");
                }
                if (!Utils::columnExists(*table, conditionalColumnName)) {
                    throw std::invalid_argument(fmt::format("Condition column '{}' does not exist.", conditionalColumnName));
                }

                database.removeRow(tableName, conditionalColumnName, conditionalOperation, conditionalValue);
                fmt::println("Row deleted from table '{}'.", tableName);
            }
            else {
                throw std::invalid_argument(fmt::format("Operation '{}' for command '{}' does not exist.", operation, command));
            }
        }
        else if (command == "SELECT") {
            parseSelectQuery(stream);
        }
        else if (command == "WRITE_DATABASE") {
            auto filename = std::string();
            stream >> filename;
            auto file = std::fstream(filename, std::ios::out | std::ios::trunc);

            if (!file) {
                throw std::runtime_error(fmt::format("Cannot open file '{}' for writing.", filename));
            } else {
                database.writeToFile(file);
                fmt::println("Database saved to file '{}'.", filename);
            }
        }
        else if (command == "READ_DATABASE") {
            auto filename = std::string();
            stream >> filename;
            auto file = std::fstream(filename, std::ios::in);

            if (!file) {
                throw std::runtime_error(fmt::format("Cannot open file '{}' for reading.", filename));
            } else {
                database.readFromFile(file);
                fmt::println("Database loaded from file '{}'.", filename);
            }
        }
        else if (command == "TABLES_NAMES") {
            auto names = Utils::getNamesOfTables(database);
            if(names.empty()) {
                throw std::invalid_argument(fmt::format("Database '{}' does not have any tables.", database.name));
            }
            fmt::println("Database '{}' has '{}' tables.", database.name, names);
        }
        else if (command == "COLUMNS_NAMES") {
            auto tableName = std::string();
            stream >> tableName;
            if (!Utils::tableExists(database, tableName)) {
                throw std::invalid_argument(fmt::format("Table '{}' does not exist in database.", tableName));
            }
            auto table = Utils::getTable(database, tableName);
            auto names = Utils::getNamesOfColumns(*table);
            if(names.empty()) {
                throw std::invalid_argument(fmt::format("Table '{}' does not have any columns.", database.name));
            }
            fmt::println("Table '{}' has '{}' columns.", tableName, names);
        }
        else if (command == "TABLES_COUNT") {
            auto count = Utils::getNumberOfTables(database);
            fmt::println("Database '{}' has '{}' tables.", database.name, count);
        }
        else if (command == "COLUMNS_COUNT") {
            auto tableName = std::string();
            stream >> tableName;
            if (!Utils::tableExists(database, tableName)) {
                throw std::invalid_argument(fmt::format("Table '{}' does not exist in database.", tableName));
            }
            auto table = Utils::getTable(database, tableName);
            auto count = Utils::getNumberOfColumns(*table);
            fmt::println("Table '{}' has '{}' columns.", tableName, count);
        }
        else if (command == "RENAME_DATABASE") {
            auto oldName = database.name;
            stream >> database.name;
            fmt::println("Database '{}' renamed to '{}'.", oldName, database.name);
        }
        else {
            throw std::invalid_argument(fmt::format("Command '{}' does not exist.", command));
        }
    }

    auto Parser::parseWhereQuery(std::stringstream& stream, Table& table, std::vector<int>& rows) -> void {
        auto columnName = std::string();
        auto condition = std::string();
        auto value = std::string();
        auto opera = std::string();

        auto columns = std::vector<std::string>();
        auto conditions = std::vector<std::string>();
        auto values = std::vector<std::string>();
        auto operas = std::vector<std::string>();

        while(stream >> columnName >> condition >> value) {
            if(!Utils::columnExists(table, columnName)) {
                throw std::invalid_argument(fmt::format("Column '{}' does not exist in table '{}'.", columnName, table.name));
            }
            if(!(condition == ">" || condition == ">=" || condition == "<" || condition == "<=" || condition == "==" || condition == "!=")) {
                throw std::invalid_argument(fmt::format("Operator '{}' is not valid.", opera));
            }
            auto column = *Utils::getColumn(table, columnName);
            if(column.type == ColumnType::NUMBER) {
                try {
                    std::stod(value);
                } catch(...) {
                    throw std::invalid_argument(fmt::format("Value '{}' is not a valid number for column '{}'.", value, columnName));
                }
            }

            columns.push_back(columnName);
            conditions.push_back(condition);
            values.push_back(value);

            if(!(stream >> opera)) {
                break;
            }

            std::ranges::transform(opera.begin(), opera.end(), opera.begin(), toupper);
            if(opera == "AND" || opera == "OR") {
                operas.push_back(opera);
            } else {
                stream.seekg(-(static_cast<int>(opera.length()) + 1), std::ios::cur);
                break;
            }
        }

        auto newRows = std::vector<int>();
        for(auto row : rows) {
            auto include = Utils::evaluateCondition(table, columns[0], conditions[0], values[0], row);

            for(auto i = 0; i < operas.size(); ++i) {
                auto nextCondition = Utils::evaluateCondition(table, columns[i + 1], conditions[i + 1], values[i + 1], row);

                if(operas[i] == "AND") {
                    include = include && nextCondition;
                } else {
                    include = include || nextCondition;
                }
            }

            if(include) {
                newRows.push_back(row);
            }
        }
        rows = newRows;
    }
    auto Parser::parseOrderByQuery(std::stringstream& stream, Table& table, std::vector<int>& rows) -> void {
        auto columnName = std::string();
        auto order = std::string();

        auto columns = std::vector<std::string>();
        auto orders = std::vector<std::string>();

        while(stream >> columnName >> order) {
            std::ranges::transform(order.begin(), order.end(), order.begin(), toupper);
            if(!Utils::columnExists(table, columnName)) {
                throw std::invalid_argument(fmt::format("Column '{}' does not exist in table '{}'.", columnName, table.name));
            }
            if(order == "ASC" || order == "DESC") {
                columns.push_back(columnName);
                orders.push_back(order);
            } else {
                throw std::invalid_argument(fmt::format("Order '{}' is invalid.", order));
            }
        }

        std::ranges::sort(rows.begin(), rows.end(), [&table, &columns, &orders](const int indexA, const int indexB) -> bool {
            for(auto i = 0; i < columns.size(); ++i) {
                const auto column = Utils::getColumn(table, columns[i]);
                auto order = true;
                if(orders[i] == "DESC") {
                    order = false;
                }

                auto valueA = column->data[indexA];
                auto valueB = column->data[indexB];

               if(valueA != valueB) {
                   if (column->type == ColumnType::NUMBER) {
                       double numberA = std::stod(valueA);
                       double numberB = std::stod(valueB);
                       return order ? numberA < numberB : numberA > numberB;
                   } else {
                       return order ? valueA < valueB : valueA > valueB;
                   }
               }
            }
            return false;
        });
    }
    auto Parser::parseSelectQuery(std::stringstream& stream) -> void {
        auto columns = std::vector<std::string>();
        auto column = std::string();
        while(stream >> column && column != "FROM" && column != "from") {
            columns.push_back(column);
        }

        auto tableName = std::string();
        stream >> tableName;
        if (!Utils::tableExists(database, tableName)) {
            throw std::invalid_argument(fmt::format("Table '{}' does not exist.", tableName));
        }
        auto const& table = Utils::getTable(database, tableName);

        if(columns.size() == 1 && columns[0] == "*") {
            columns.clear();
            std::ranges::transform(table->columns.begin(),table->columns.end(), std::back_inserter(columns),
                [](Column const& column) -> std::string { return column.name; });
        }

        for(auto const& column: columns) {
            if(!Utils::columnExists(*table, column)) {
                throw std::invalid_argument(fmt::format("Column '{}' does not exist in table '{}'.", column, tableName));
            }
        }

        auto firstIndex = 0;
        auto rows = std::vector<int>(table->columns[0].data.size());
        std::ranges::generate(rows.begin(), rows.end(), [&firstIndex]() -> int { return firstIndex++; });

        auto operation = std::string();
        auto streamPosition = stream.tellg();

        stream >> operation;
        if(operation == "WHERE" || operation == "where") {
            parseWhereQuery(stream, *table, rows);
        } else {
            stream.seekg(streamPosition);
        }

        streamPosition = stream.tellg();
        stream >> operation;
        if(operation == "ORDER_BY" || operation == "order_by") {
            parseOrderByQuery(stream, *table, rows);
        } else {
            stream.seekg(streamPosition);
        }

        Utils::printTable(*table, columns, rows);
    }
}