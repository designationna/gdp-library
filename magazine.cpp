#include "magazine.hpp"
#include <iostream>
#include <string>

/**
 * Konstruktur für die Klasse Magazin
 *
 * Alle Werte (Autor, Titel, Verlag, Veroeffentlichungsdatum, 
 * ISSN, anzahl ausgeliehender Exemplare, Anzahl Exemplare insgesammt)
 * werden hier initialisiert
 *
 * @param a Autor, t Titel, p Verlag, pD Veroeffentlichungsdatum,
 *        i ISSN, l Anzahl ausgeliehender Exemplare, s Anzahl Exemplare Insgesamt
 */
Magazine::Magazine(
	const std::string a,
	const std::string t,
	const std::string p,
	const std::string pd,
	const std::string i,
	int l,
	int s):
	author ( a ),
	title ( t ),
	publisher ( p ),
	publishingDate ( pd ),
	issn ( i ),
	lent ( l ),
	stock ( s ) {}

// --- GETTER METHODEN ---

std::string Magazine::getAuthor() { return author; }

std::string Magazine::getTitle() { return title; }

std::string Magazine::getISSN() { return issn; }

int Magazine::getStock() { return stock; }

int Magazine::getLent() { return lent; }

std::string Magazine::getPublisher() { return publisher; }

std::string Magazine::getPublishingDate() { return publishingDate; }

/**
* Gibt die Informationen des Magazins formattiert zur Ausgabe zurueck.
* 
* @returns den formattierten String
*/
std::string Magazine::getFullTitle() {
	std::string fullTitle = issn + ": " + author + " - '" + title + "', " + publisher + ", " + publishingDate;
	return fullTitle;
}

/**
* Generiert "eine Zeile" fuer die Datenbankdatei mit entsprechender Syntax
*
* @returns Text mit allen Attributen dieses Magazinobjekts
*/
std::string Magazine::genDBEntry() {
	std::string lentString = std::to_string( lent );
	std::string stockString = std::to_string( stock );
	return std::string  ( issn + ", \"" + author + "\", \"" + title + "\", \"" + publisher + "\", \"" + publishingDate + "\", " + lentString + ", " + stockString );
}

/**
 * Gibt den Wahrheitswert zurueck, ob ein Magazin nicht mehr vorraetig ist
 * (ja, wenn die Anzahl der ausgeliehenen Exemplare groesser oder 
 * gleich der maximal vorraetigen Exemplare entspricht, ansonsten nein)
 */
bool Magazine::isLent() { return lent >= stock; }

/**
* Gibt den Wahrheitswert zurueck, ob ein Magazin ausleihbar ist.
* (ja, wenn die Anzahl der ausgeliehenen Magazine ungleich 0 ist, ansonsten nein)
*/
bool Magazine::returnable() { return lent != 0; }

/**
 * Erhoeht die Anzahl der ausgeliehenen Exemplare um 1
 */
void Magazine::borrow() { lent++; }

/**
 * Verringert die Anzahl der ausgeliehenen Exemplare um 1
 */
void Magazine::restitute() { lent--; }

/**
 * Ueberprueft den gegebenen String auf korrekte Laenge (8) und
 * Umwandelbarkeit in Ziffern
 *
 * @param issn die zu ueberpruefende Zeichenkette
 * @param showErr optionaler Parameter um bei Fehlern der ISSN eine Fehlermeldung auszugeben
 * @returns Wahrheitswert, ob es sich um eine ISSN handelt
 */
bool Magazine::isISSN( const std::string& issn, bool showErr ) {
	bool areDigits  = true;
    for( const char& c : issn ) {
    	if ( !std::isdigit( c ) ) {
    	 	areDigits = false;
    	 } 
    }

    if (!areDigits || !(issn.size() == 8)) {
    	if (showErr) {
			std::cout << "Eine ISSN besteht aus 8 numerischen Ziffern." << std::endl;
    	}
    	return false;
    } else { return true; }
}

/**
 * Ueberprueft den gegebenen String auf Laenge (braucht mind. 1 Zeichen) und
 * Anfuehrungszeichen (die die Formattierung der Datenbank breaken wuerden)
 *
 * @param attr das zu ueberpruefende Attribut
 * @param showErr optionaler Parameter um bei Fehlern des Attributs eine Fehlermeldung auszugeben
 * @returns Wahrheitswert, ob es sich um ein Attribut handelt
 */
bool Magazine::isAttribute( const std::string& attr, bool showErr ) {
	if ( ( attr.size() == 0 || attr.find( '"' ) != std::string::npos ) ) {
		if (showErr) { std::cout << "Attribute brauchen mindestens Zeichen und koennen keine Anfuehrungszeichen beinhalten." << std::endl; } 
		return false;
	}
	else { return true; }
}

/**
 * Erhoeht das Vorkommen eines Magazins um 1
 */
void Magazine::stockUp() {
	stock++;
}
