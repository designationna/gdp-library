#include <iostream>
#include <streambuf>
#include <fstream>
#include <vector>
#include <stdexcept>

#include "database.hpp"
#include "magazine.hpp"

/**
* Konstruktur für die Klasse Database
*
* Liest den Inhalt der Datei des gegebenen Dateinamens aus.
*
* @param f Dateiname der einzulesenden Datenbank
*/
Database::Database( const std::string& f ) : databaseString( readFile( f ) ), databaseFileName( f ) {}

/**
* Wandelt den Datenbanktext in Magazine um
*
* Der Datenbanktext wird in einzelne Zeilen aufgeteilt und pro
* Zeile werden die Attribute ausgelesen, aus denen ein Magazin erstellt wird.
*
* @returns Vektor an Magazinen
*/
std::vector<Magazine> Database::parseDbString() {
	std::vector<Magazine> magazines;

	std::vector<std::string> dbEntries;
	dbEntries = splitByLine( databaseString );

	for (auto entry : dbEntries) {
		magazines.push_back( createMagazine ( splitIntoAttributes( entry ) ) );
	}

	return magazines;
}

/**
* Liest die Datenbankdatei ein und gibt den Text zurueck
*
* @param fileName den Dateiname der einzulesenden Datei
* @returns Datenbanktext
*/
std::string Database::readFile( const std::string& fileName ) {
	std::ifstream ifs( fileName );
	if ( ifs.good() ) {
		return std::string( ( std::istreambuf_iterator<char>( ifs ) ),( std::istreambuf_iterator<char>() ) );	
	} else {
		throw std::runtime_error( "Der Angegebene Dateipfad ist nicht gueltig." );
	}
}

/**
* Liest die Datenbankdatei ein und gibt den Text zurueck
*
* Durchsucht den Text nach Zeilenumbruechen und fuegt so
* Abschnittsweise Strings zum Vektor hinzu.
*
* @param string den zeilenweise aufzuteilenden String
* @returns Vektor an Strings (der nach Zeilen aufgeteilte Text)
*/
std::vector<std::string> Database::splitByLine( std::string& string ) {
	std::string currLine;
	std::vector<std::string> splitted;

	for( char& c : string ) {
		if ( c != '\n' ) {
			currLine += c;
		} else {
			splitted.push_back(currLine);
			currLine.clear();
		}
	}

	splitted.push_back( currLine );
	currLine.clear();

	return splitted;
}

/**
* Teilt Text in Attribute ein
*
* Der Text wird auf Kommata untersucht und so in Abschnitte = Attribute geteilt.
* Falls ein Anfuehrungszeichen " vorkommen sollte, wird der Prozess des aufteilens bis zum
* naechsten " unterbrochen um Kommata in Buchtiteln zu ermoeglichen
*
* @param dbEntry Eine Zeile aus der eingelesenen Datenbankdatei
* @returns Vektor an Attributen
*/
std::vector<std::string> Database::splitIntoAttributes ( std::string& dbEntry ) {
	std::vector<std::string> attributes;
	bool deactivateSplitter { false };
	unsigned long lastAttrPtr { 0 };
	char splitTrigger { ',' };
	
	for( std::string::size_type i = 0; i < dbEntry.size(); ++i ) {
		if ( dbEntry[i] == '"' ) {
			deactivateSplitter = !deactivateSplitter;
		}

		if ( !deactivateSplitter && ( dbEntry[i] == splitTrigger ) ) {
			attributes.push_back( dbEntry.substr(lastAttrPtr, ( static_cast<unsigned long>(i) - lastAttrPtr ) ) );
			lastAttrPtr = i + 2;
		}
	}

	attributes.push_back( dbEntry.substr(lastAttrPtr, ( static_cast<unsigned long>(dbEntry.size()) - lastAttrPtr ) ) );

	return attributes;
}

/**
* Erstellt ein Magazinobjekt mit Hilfe von Attributen
*
* Die einzelenen Attribute werden auf syntaktische Korrektheit geprueft
* (fehlende schliessende Anfuehrungszeichen, ungueltige ISSN, etc.) und gegebenenfalls in
* neue Dateitypen geparsed (Beispiel ISSN: String --> Int).
* Bei erfolgreicher Ueberpruefung wird ein Magazin erstellt und zurueckgegeben
*
* @param attributes Vektor an Attributen generiert durch die Funktion splitIntoAttributes
* @returns Magazin
*/
Magazine Database::createMagazine( std::vector<std::string> attributes ) {

	std::string a;
	std::string t;
	std::string p;
	std::string pD;
	std::string i;
	int s;
	int l;

	// ISSN
	if ( !Magazine::isISSN( attributes[0] ) ) {
		throw std::runtime_error( "Die Formattierung der Datenbankdatei ist fehlerhaft! (ISSN)" );
	} else {
		i = attributes[0];
	}

	// Author
	if ( attributes[1][0] != '"' || attributes[1][attributes[1].size() - 1] != '"' ) {
		throw std::runtime_error( "Die Formattierung der Datenbankdatei ist fehlerhaft! (Autor)" );
	} else {
		a = attributes[1].substr(1, attributes[1].size() -2);
	}

	// Title
	if ( attributes[2][0] != '"' || attributes[2][attributes[2].size() - 1] != '"' ) {
		throw std::runtime_error( "Die Formattierung der Datenbankdatei ist fehlerhaft! (Titel)" );
	} else {
		t = attributes[2].substr(1, attributes[2].size() -2);
	}

	// Publisher
	if ( attributes[3][0] != '"' || attributes[3][attributes[3].size() - 1] != '"' ) {
		throw std::runtime_error( "Die Formattierung der Datenbankdatei ist fehlerhaft! (Verlag)" );
	} else {
		p = attributes[3].substr(1, attributes[3].size() -2);
	}

	// Title
	if ( attributes[4][0] != '"' || attributes[4][attributes[4].size() - 1] != '"' ) {
		throw std::runtime_error( "Die Formattierung der Datenbankdatei ist fehlerhaft! (Erscheinungsdatum)" );
	} else {
		pD = attributes[4].substr(1, attributes[4].size() -2);
	}

	// Anzahl ausgeliehener Exemplare
	for( const char& c : attributes[5] ) {
		if ( !std::isdigit( c ) ) {
			throw std::runtime_error( "Die Formattierung der Datenbankdatei ist fehlerhaft! (Anzahl ausgeliehener Magazine)" );
		} 
	}
	l = std::stoi( attributes[5] );

	// Anzahl Exemplare insgesammt
	for( const char& c : attributes[6] ) {
		if ( !std::isdigit( c ) ) {
			throw std::runtime_error( "Die Formattierung der Datenbankdatei ist fehlerhaft! (Anzahl Magazine insgesammt)" );
		} 
	}
	s = std::stoi( attributes[6] );

    // Mehr Magazine ausgeliehen, als insgesammt zur Verfuegung stehen --> Fehler in der Datenbank
	if (l > s){
		throw std::runtime_error( "Die Formattierung der Datenbankdatei ist fehlerhaft! (Mehr Magazine ausgeliehen als im Inventar)" );
	}

	Magazine tmp( a, t, p, pD, i, l, s );
	return tmp;
}

/**
 * Speichert die Momentanen Werte der Buecherei in der Datenbankdatei ab
 *
 * @param dbString Der generierte Datenbankstring (siehe library.cpp)
 */
void Database::saveChanges( std::string& dbString ) {
	std::ofstream out( databaseFileName );
	out << dbString ;
}
