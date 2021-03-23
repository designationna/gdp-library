// C++ Libraries
#include <iostream>
#include <vector>
#include <string>

// External Classes
#include "magazine.hpp"
#include "database.hpp"
#include "library.hpp"

// Prototypes
std::vector<std::string> parseCmd ( std::string );
bool checkCmd ( std::vector<std::string> );

/**
* Spaltet den vom Benutzer eingegebenen Text in Befehl und ggf Argument auf
*
* Alle Leerzeichen am Ende des Textes werden eleminiert.
* Sucht danach nach Leerzeichen im Text und spaltet beim Erstvorkommen den Text
* so wird beispielsweise "suche 12 34" zu ["suche", "12 34"]
*
* @param cmd der vom Benutzer eingegebene Text
* @returns Vektor der an Stelle 0 den Befehl und an Stelle 1 ggf das dazug. Argument beinhaltet
*/
std::vector<std::string> parseCmd ( std::string cmd ) {
	std::vector<std::string> splitCmd;
	bool foundSpace { false };

	while (cmd.back() == ' ') {
		cmd = cmd.substr(0, cmd.size() - 1);
	}

	for (std::string::size_type i = 0; i < cmd.size(); ++i) {
		if ( cmd[i] == ' ' ) {
			foundSpace = true;
			splitCmd.push_back( cmd.substr( 0, i ) );
			splitCmd.push_back( cmd.substr( i + 1, cmd.size() - i ) );
			break;
		}
	}

	if (!foundSpace) { 
		splitCmd.push_back( cmd );
	}
	
	return splitCmd;
}

/**
* Ueberprueft auf Korrektheit des eingegebenen Befehls
*
* Ueberprueft ob der gegebene Text ein Befehl ist, und ggf. ob ein Argument bereitgestellt wurde,
* indem durch die Liste an verfuegbaren Befehlen iteriert wird und auf Gleichheit geprueft wird.
* Falls erfolgreich wird an der gefundenen Position der wert im cmdTakesArg Array abgefragt und somit ermittelt
* ob dieser Befehl ein Argument benoetigt.
*
* @param splitCmd der durch parseCmd aufgeteile Befehl im Vektor
* @returns Wahrheitswert, ob der Befehl korrekt ist
*/
bool checkCmd ( std::vector<std::string> splitCmd ) {
	bool validCmd { false };
	bool validArgs { false };

	const std::vector<std::string> cmds {"hilfe", "beenden", "suche", "neuesMagazin", "leiheAus", "gebeZurueck"};
	const std::vector<bool> cmdTakesArg {false, false, true, false, true, true};

	for (std::vector<std::string>::size_type i = 0; i < cmds.size(); ++i) {	
		if (cmds[i] == splitCmd[0]) {
			validCmd = true;

			if ( ( cmdTakesArg[i] && ( splitCmd.size() == 2 ) ) || ( !cmdTakesArg[i] && ( splitCmd.size() == 1 ) ) ) {
				validArgs = true;
			}
		}
	}

	if ( validCmd && validArgs ) {
		return true; 
	}
	else if (validCmd && !validArgs) {
		if (splitCmd.size() == 1) {
			std::cout << "Der Befehl '" << splitCmd[0] << "' benoetigt ein Argument." << std::endl;
			
		} else {
			std::cout << "Der Befehl '" << splitCmd[0] << " benoetigt keine Argumente." << std::endl;
		}
		return false;
	} else {
		std::cout << "Unbekannter Befehl" << std::endl;
		return false;
	}
}

/**
* Erstellt ein Datenbankobjekt mit gegebenenem Dateipfad, der auf die Datenbankdatei zeigt
* und erstellt mithilfe der Datenbank ein Library Objekt, in dem alle Magaine gemanaged werden.
* Daraufhin wird eine Eingabeaufforderungsschleife gestartet, die den Benuzter nach Eingabe fragt
* diese Eingabe verarbeitet, prueft und ggf die dazugehoerigen Funktionen ausfuehrt.
* nach Abbruch der Schleife (durch den 'beenden' Befehl) wird die Datenbank aktualisiert und das Programm beendet.
*/
int main () {
	Database myDb( "db.csv" );
	Library myLib( myDb );
	std::cout << "Die Datenbank wurde erfolgreich ins Dateisystem geladen" << std::endl;
	std::string cmd = "";
	std::vector<std::string> splitCmd;

	while ( cmd != "beenden" ) {
		std::cout << "Erwarte Eingabe ('hilfe' fuer moegliche Eingaben): ";
		getline(std::cin, cmd, '\n');

		splitCmd = parseCmd( cmd );
		if ( checkCmd( splitCmd ) ) {
			if (splitCmd[0] == "hilfe") {
				myLib.help();
			} else if (splitCmd[0] == "suche") {
				std::string queryResult = myLib.search( splitCmd[1] );
				std::cout << queryResult << std::endl;
			} else if (splitCmd[0] == "neuesMagazin") {
				myLib.addMagazine();
			} else if (splitCmd[0] == "leiheAus") {
				myLib.borrowMagazine( splitCmd[1] );
			} else if (splitCmd[0] == "gebeZurueck") {
				myLib.returnMagazine( splitCmd[1] );
			}
		}
	}

	std::cout << "Das Programm wird beendet." << std::endl;
	myLib.quit();
	return 0;
}
