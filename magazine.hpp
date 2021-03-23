#pragma once
#include <iostream>
#include <vector>
#include <string>

/**
* Die Magazinklasse enthaelt alle Informationen ueber ein Magazin. (Autor, Titel, Verlag, etc.) Zudem repraesentiert
* ein Magazinobjekt alle Exemplare dieses Magazins. D. h. Moeglichkeit des Ausleihens und Zurueckgebens werden
* ebenfalls ueber diese Klasse organisiert.
*/
class Magazine {
	public:
		Magazine(const std::string, const std::string, const std::string, const std::string, const std::string, int, int);

		std::string getAuthor();
		std::string getTitle();
		std::string getPublisher();
		std::string getPublishingDate();
		std::string getISSN();
		int getStock();
		int getLent();
		std::string getFullTitle();
		std::string genDBEntry();
		bool isLent();
		bool returnable();
		void borrow();
		void restitute();
		void stockUp();
		static bool isISSN( const std::string&, bool showErr = false );
		static bool isAttribute( const std::string&, bool showErr = false );

	private:
		const std::string author;
		const std::string title;
		const std::string publisher;
		const std::string publishingDate;
		const std::string issn;
		int lent;
		int stock;
};
