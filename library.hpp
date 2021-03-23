#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "database.hpp"
#include "magazine.hpp"

/*
* Die Library Klasse organisiert alle Magazine. Sie benoetigt ein Datenbankobjekt zur Initialisierung
* und spaeteren Aktualisierung der Magazin-Informationen. Ueber diese Klasse werden die Magazine Ausge- und Verliehen.
* sowie neue Magazine ins System hinzugefuegt. Auch Hilfe und Such Funktion werden ueber diese Klasse ausgefuehrt
*/
class Library {
	public:
		Library( const Database& );
		void help();
		std::string search( std::string );
		void addMagazine();
		void borrowMagazine( std::string );
		void returnMagazine( std::string );
		void writeNCheck(std::string inText, std::string& refVar, bool (*checkFun)(const std::string&, bool));
		void quit();
	private:
		Database db;
		std::vector<Magazine> magazines;
};
