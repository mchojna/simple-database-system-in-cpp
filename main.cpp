#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <string>

#include "db/db.hpp"

/*
 * Model danych:
 *      Projekt implementuje baze danych realizujaca model kartotekowy (kazda tabela to osobny byt)
 *
 * Sposob przechowywania danych:
 *      Obiekt bazy danych przechowuje nazwe bazy danych oraz wektor skladajacy sie z tabel
 *      Obiekt tabeli przechowuje nazwe tabeli oraz wektor skladajacy sie z kolumn
 *      Obiekt kolumny przechowuje nazwe, typ kolumny oraz wektor skladajacy sie z pojedynczych rekordow / wartosci danych
 *      Kolumna moze byc typu tekstkowego lub numerycznego (TEXT, NUMBER)
 *
 * Jezyk zapytan:
 *      DDL:
 *          Tworzenie tabeli:
 *              CREATE_TABLE nazwa_tabeli nazwa_kolumny1 typ_kolumny_1 [nazwa_kolumny2 typ_kolumny_2 ...]
 *                  CREATE_TABLE tab1 col1 TEXT
 *                  CREATE_TABLE tab2 col1 TEXT col2 NUMBER col3 TEXT col4 TEXT col5 NUMBER
 *
 *          Zmiana nazwy tabeli:
 *              REANAME_TABLE stara_nazwa_tabeli nowa_nazwa_tabeli
 *                  REANAME_TABEL tab1 tab3
 *
 *          Usuwanie tabeli:
 *              DROP_TABLE nazwa_tabeli
 *                  DROP_TABLE tab1
 *
 *          Tworzenie kolumny:
 *              ALTER_TABLE nazwa_tabeli ADD_COLUMN nazwa_kolumny1 typ_kolumny_1
 *                  ALTER_TABLE tab2 ADD_COLUMN col6 TEXT
 *
 *          Zmiana nazwy kolumny:
 *              ALTER_TABLE nazwa_tabeli RENAME_COLUMN stara_nazwa_kolumny nowa_nazwa_kolumny
 *                  ALTER_TABLE tab2 ADD_COLUMN col6 col7
 *
 *          Usuwanie kolumny:
 *              ALTER_TABLE nazwa_tabeli DROP_COLUMN nazwa_kolumny
 *                  ALTER_TABLE tab2 DROP_COLUMN col7
 *
 *      DML:
 *          Dodawanie wiersza:
 *              ALTER_TABLE nazwa_tabeli INSERT_ROW wartosc1 wartosc2 wartosc3 wartosc4...
 *                  ALTER_TABLE tab2 INSERT_ROW aaa 111 bbb ccc 222
 *
 *          Aktualizowanie wiersza:
 *              ALTER_TABLE nazwa_tabeli UPDATE_ROW nazwa_kolumny nowa_wartosc [WHERE nazwa_kolumny_wartunkowej operator wartosc_warunkowa]
 *                  ALTER_TABLE tab2 UPDATE_ROW col1 aaa
 *                  ALTER_TABLE tab2 UPDATE_ROW col2 123 WHERE col1 == aaa
 *
 *                  UWAGA 1: dostepne operatory arytmetyczne: > >= == != <= <
 *
 *          Usuwanie wiersza:
 *              ALTER_TABLE nazwa_tabeli DELETE_ROW WHERE nazwa_kolumny operator wartosc
 *                  ALTER_TABLE tab2 DELETE_ROW WHERE col1 == aaa
 *
 *      DQL:
 *          Wypisywanie danych:
 *              SELECT nazwa_kolumny_1 [nazwa_kolumn_2 ...] | * FROM nazwa_tabeli
 *                  [WHERE nazwa_kolumny_wartunkowej_1 operator_1 wartosc_warunkowa_1 [[AND | OR] nazwa_kolumny_wartunkowej_2 operator_2 wartosc_warunkowa_2 ...]]
 *                  [ORDER_BY nazwa_kolumny_1 [ASC | DESC] [nazwa_kolumny_2 [ASC | DESC]...]
 *
 *                  SELECT * FROM tab1
 *                  SELECT col1 col2 col3 FROM tab2
 *
 *                  SELECT * FROM tab1 WHERE col1 == aaa
 *                  SELECT col1 col2 col3 FROM tab2 WHERE col2 > 100 AND col5 < 200
 *
 *                  SELECT * FROM tab1 ORDER_BY col2 ASC
 *                  SELECT col1 col2 col3 FROM tab2 ORDER_BY col1 ASC col2 DESC
 *
 *                  SELECT * FROM tab1 WHERE col1 == aaa ORDER_BY col2 ASC
 *                  SELECT col1 col2 col3 FROM tab2 WHERE col2 > 100 AND col5 < 200 ORDER_BY col1 ASC col2 DESC
 *
 *                  UWAGA 1: dzialanie wielu AND-ow i OR-ow w klauzuli WHERE dziala na zasadzie wiazania lewostronnego
 *                      jezeli mamy "col1 > 200 AND col2 < 3 OR col3 == 10 AND col4 != 100" to zostanie to przetlumaczone
 *                      jako "((((col1 > 200) AND col2 < 3) OR col3 == 10) AND col4 != 100)"
 *
 *                  UWAGA 2: dostepne operatory arytmetyczne: > >= == != <= <
 *
 *      Inne:
 *          Zapisywanie bazy danych:
 *              WRITE_DATABASE sciezka_do_pliku_txt
 *                  WRITE_DATABASE db.txt
 *                  WRITE_DATABASE /dir1/dir2/db.txt
 *
 *          Odczytywanie bazy danych:
 *              READ_DATABASE sciezka_do_pliku_txt
 *                  READ_DATABASE db.txt
 *                  READ_DATABASE /dir1/dir2/db.txt
 *
 *          Wypisywanie nazwy tabel:
 *              TABLES_NAMES
 *
 *          Wypisywanie nazwy kolumn:
 *              COLUMNS_NAMES nazwa_tabeli
 *
 *          Wypisywanie liczby tabel:
 *              TABLES_COUNT
 *
 *          Wypisywanie liczby kolumn:
 *              COLUMNS_COUNT nazwa_tabeli
 *
 *          Zmiana nazwy bazy danych:
 *              RENAME_DATABASE nowa_nazwa
 *                  RENAME_DATABASE db2
 */

auto main() -> int {
    auto db = Db::Database();
    auto parser = Db::Parser{db};

    fmt::println("Simple Database Manager");
    fmt::println("Enter commands (type 'exit' to quit):");

    auto line = std::string();
    while (true) {
        fmt::print("> ");
        if (!std::getline(std::cin, line)) {
            return 0;
        }

        if (line == "exit") {
            return 0;
        }

        try {
            parser.parseQuery(line);
        } catch (const std::exception& e) {
            fmt::println("Error: {}", e.what());
        }
    }
}