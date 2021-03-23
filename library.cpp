#include <iostream>
#include <string>
#include "library.hpp"
#include "database.hpp"
#include "magazine.hpp"


/**
* Konstruktur für die Klasse Library
*
* Der die gegebene Datenbank erstellt aus der db.txt Datei ein Vector an Magazinen
*
* @param d Datenbankobjekt
*/
Library::Library(const Database& d) : db( d ) {
	magazines = db.parseDbString();
}

/**
* Gibt einen Hilfetext zur Bedienung des Programms aus
*/
void Library::help() {
	std::cout << "Folgende Befehle stehen zur Verfuegung:" << std::endl;
	std::cout << "neuesMagazin : Mit diesem Befehl koennen Sie neue Magazine in das System aufnehmen. Ein Assistent wird sie durch den Prozess des Hinzufuegens leiten." << std::endl;
	std::cout << "leiheAus <ISSN> : Falls ein Magazin mit der gegebenen ISSN im Inventar ist, wird es mit diesem Befehl ausgeliehen." << std::endl;
	std::cout << "gebeZurueck <ISSN> : Dieser Befehl gibt ausgeliehene Magazine zurueck" << std::endl;
	std::cout << "hilfe : Diese Anzeige wird durch Aufruf dieses Befehls angezeigt." << std::endl;
	std::cout << "suche <Titel / ISSN> : Das System durchsucht das Inventar auf Vorkommen der Anfrage in ISSN oder Titel." << std::endl;
	std::cout << "beenden : Das Program wird beendet." << std::endl;
}

/**
* Durchsucht alle Magazine nach gegebenem Stichwort
*
* Durch Iteration durch alle Magazine wird ISSN und Titel des Magazins auf (teilweise) Uebereinstimmung
* mit dem Stichwort (Parameter) ueberprueft und ggf. zum Suchergebnistext hinzugefuegt.
*
* @param query den zu suchenden text
* @returns Liste (in Textform) der uebereinstimmenden Magazine
*/
std::string Library::search (std::string query) {
	std::string queryResult;

	for (auto magazine : magazines) {
		if (magazine.getISSN().find(query) != std::string::npos || magazine.getTitle().find(query) != std::string::npos) {
			queryResult += magazine.getFullTitle() + "\n";
		}
	}

	if (queryResult.empty()) { queryResult = "Keine Uebereinstimmungen"; }
	else { queryResult = queryResult.substr(0, queryResult.size() - 1 ); }

	return queryResult;
}

/**
* Veranlaesst Eingabe eines Atrtibuts durch den Benutzer und ueberprueft dieses aus Korrektheit
*
* Der Benutzer wird durch den inText zur Eingabe eines Attributs aufgefordert und solange dieses
* Attribut nicht der Norm fuer dieses Attribut entspricht, was durch die gegebene Funktion ueberprueft wird
* Wird der Benutzer erneut zur Eingabe aufgefordert. 
*
* @param inText Auszugebender Text der die Eingabe des Attributs fordert
* @param refVar die Variable in die die Benutzereingabe gespeichert werden soll
* @param checkFun die Funktion zum Ueberpruefen der Benutzereingabe
*
*/
void Library::writeNCheck(std::string inText, std::string& refVar, bool (*checkFun)(const std::string&, bool)) {
	std::cout << inText;
	getline(std::cin, refVar, '\n');
	while ( !checkFun(refVar, true) ) {
		std::cout << inText;
		getline(std::cin, refVar, '\n');
	}
}

/**
* Generiert ein neues Magazin mit Hilfe von Benuztereingaben
*
* Alle benoetigten Attribute zur Erstellung eines Magazins (siehe Magazin Konstruktor)
* werden durch Eingabeaufforderungen erfragt und ggf aus Korrektheit ueberprueft (z. B. ISSN)
* so dass ein neues Magazin zum Magazin-Vektor hinzugefuegt werden kann ("ins Inventar aufgenommen werden kann")
* sollte die vom Benutzer eingegebene ISSN schon im System vorhanden sein, wird das Vorkommen dieses Magazins erhoeht
* und der weitere Erstellungsprozess uebersprungen.
*/
void Library::addMagazine() {
	std::string newAuthor;
	std::string newTitle;
	std::string newIssn;
	std::string newPublisher;
	std::string newPublishingDate;
	std::string confirm;
	std::cout << "Wilkommen beim Assisten zum Hinzufuegen von Magazinen!" << std::endl;

	writeNCheck( "ISSN des Magazins: ", newIssn, &Magazine::isISSN);

	for ( auto& magazine : magazines ) {
		if ( magazine.getISSN() == newIssn ) {
			magazine.stockUp();
			std::cout << "Ein weiteres Exemplar von " << magazine.getFullTitle() << " wurde zum Inventar hinzugefuegt." << std::endl;
			return;
		}
	}

	writeNCheck( "Autor des Magazins: ", newAuthor, &Magazine::isAttribute);
	writeNCheck( "Titel des Magazins: ", newTitle, &Magazine::isAttribute);
	writeNCheck( "Verlag des Magazins: ", newPublisher, &Magazine::isAttribute);
	writeNCheck( "Veroeffentlichungsdatum des Magazins: ", newPublishingDate, &Magazine::isAttribute);

	std::cout << "Sind diese Angaben korrekt ('ja' oder 'nein'): " << newIssn << ": " << newAuthor << " - '" << newTitle << "', " << newPublisher << ", " << newPublishingDate << "? : ";

	getline(std::cin, confirm, '\n');
	while (!(confirm == "ja" || confirm == "nein")) {
		std::cout << "Ungueltige Eingabe! Angaben mit 'ja' bestaetigen oder mit 'nein' den Vorgang abbrechen: " << std::endl;
		getline(std::cin, confirm, '\n');
	}


	if ( confirm == "ja" ) {
		Magazine tmp( newAuthor, newTitle, newPublisher, newPublishingDate, newIssn, 0, 1 );
		magazines.push_back( tmp );
	} else {
		std::cout << "Vorgang abgebrochen." << std::endl;
	}
}

/**
* Leiht ein Magazin anhand der ISSN aus
*
* Durchsucht alle Magazine nach der ISSN und leiht es aus, falls noch Exemplare zur Verfuegung stehen.
*
* @param issn die ISSN des auszuleihenden Magazins
*/
void Library::borrowMagazine( std::string issn ) {
	bool issnFound { false };
	bool magazineLent { false };
	for ( auto& magazine : magazines ) {
		if ( magazine.getISSN() == issn ) {
			issnFound = true;
			if ( !magazine.isLent() ) {
				magazine.borrow();
				magazineLent = true;
				std::cout << "'" << magazine.getTitle() << "' von " << magazine.getAuthor() << " wurde ausgeliehen." << std::endl;
				std::cout << "Es sind noch " << magazine.getStock() - magazine.getLent() << " von " << magazine.getStock() << " Exemplare verfuegbar." << std::endl;
				break;
			}
		}
	}

	if ( !magazineLent ) {
		if ( issnFound ) { std::cout << "Alle verfuegbaren Exemplare des Buches sind momentan ausgeliehen." << std::endl; }
		else { std::cout << "Kein Magazin mit dieser ISSN im Inventar gefunden." << std::endl; }
	}
}

/**
* Gibt ein Magazin mit Hilfe der gegebenen ISSN zurueck
*
* Durchsucht alle Magazine nach der ISSN und gibt es zurueck, falls das Invantar noch nicht vollstaendig ist.
*
* @param issn die ISSN des zurueckzugebenden Magazins
*/
void Library::returnMagazine( std::string issn ) {
	bool issnFound { false };
	bool magazineReturned { false };
	for ( auto& magazine : magazines ) {
		if ( magazine.getISSN() == issn ) {
			issnFound = true;
			if ( magazine.returnable() ) {
				magazine.restitute();
				magazineReturned = true;
				std::cout << "'" << magazine.getTitle() << "' von " << magazine.getAuthor() << " wurde zurueckgegeben." << std::endl;
				std::cout << "Es sind noch " << magazine.getStock() - magazine.getLent() << " von " << magazine.getStock() << " Exemplare verfuegbar." << std::endl;
				break;
			}
		}
	}

	if ( !magazineReturned ) {
		if ( issnFound ) { std::cout << "Alle Exemplare dieses Magazines sind bereits im Inventar." << std::endl; }
		else { std::cout << "Kein Magazin mit dieser ISSN ist im System verzeichnet." << std::endl; }
	}
}

/**
* Aktualisiert die Datenbankdatei
*
* Generiert von allen Magazinen einen Text, der der Datenbanksyntax entspricht
* (siehe einzelne Zeile in der Datenbankdatei) und schreibt die Summe der Texte in die Datenbankdatei
*/
void Library::quit() {
	std::string dbString;
	for ( auto& magazine : magazines ) { dbString += magazine.genDBEntry() + "\n"; }
	dbString = dbString.substr(0, dbString.size() - 1 );
	db.saveChanges( dbString );
}
