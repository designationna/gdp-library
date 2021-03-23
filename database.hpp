#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "magazine.hpp"

/**
* Die Datenbank Klasse liest aus einer Datenbankdatei bisher erstellte Magazine aus und wandelt sie
* in Kompatible Objekte (Magazine) um. Gleichermassen aktualisiert sie beim Beendes des Programmes
* den aktuellen Status der Magazine in der Datenbankdatei
*/
class Database {
	public:
		Database( const std::string& );
		std::vector<Magazine> parseDbString();
		std::vector<std::string> splitByLine( std::string& );
		std::vector<std::string> splitIntoAttributes( std::string& );
		Magazine createMagazine( std::vector<std::string> );
		void saveChanges( std::string& );

	private:
		std::string databaseString;
		std::string databaseFileName;
		std::string readFile( const std::string& );
};
