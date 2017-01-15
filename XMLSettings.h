/*
 *  XMLConfig.h
 *
 *  Copyright (C) 2011
 *  Sven Lohrmann <sven.lohrmann@gmx.net>
 *  All Rights Reserved.
 *
 *****************************************************************************
 *  $Id: XMLConfig.h 25.02.2011 03:56:57 nexus $
 *****************************************************************************
 *
 *  Description:
 *  Funktionen zum lesen und schreiben von XML-Konfig Dateien
 *
 */

#ifndef XMLCONFIG_H_
#define XMLCONFIG_H_

#include <QIODevice>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>

bool readXmlFile(QIODevice &device, QSettings::SettingsMap &map) {
	QXmlStreamReader xmlReader(&device);
	QStringList elements;

	// Solange Ende nicht erreicht und kein Fehler aufgetreten ist
	while (!xmlReader.atEnd() && !xmlReader.hasError()) {
		// N�chsten Token lesen
		xmlReader.readNext();

		// Wenn Token ein Startelement
		if (xmlReader.isStartElement() && xmlReader.name() != "Settings") {
			// Element zur Liste hinzuf�gen
			elements.append(xmlReader.name().toString());
		// Wenn Token ein Endelement
		} else if (xmlReader.isEndElement()) {
			// Letztes Element l�schen
			if(!elements.isEmpty()) elements.removeLast();
		// Wenn Token einen Wert enth�lt
		} else if (xmlReader.isCharacters() && !xmlReader.isWhitespace()) {
			QString key;

			// Elemente zu String hinzuf�gen
			for(int i = 0; i < elements.size(); i++) {
				if(i != 0) key += "/";
				key += elements.at(i);
			}

			// Wert in Map eintragen
			map[key] = xmlReader.text().toString();
		}
	}

	// Bei Fehler Warnung ausgeben
	if (xmlReader.hasError()) {
        qWarning() << "Error reading project file " << xmlReader.errorString() << " in line " << xmlReader.lineNumber();
		return false;
	}

	return true;
}

bool writeXmlFile(QIODevice &device, const QSettings::SettingsMap &map) {
	QXmlStreamWriter xmlWriter(&device);

	xmlWriter.setAutoFormatting(true);
	xmlWriter.writeStartDocument();
	xmlWriter.writeStartElement("Settings");

	QStringList prev_elements;
	QSettings::SettingsMap::ConstIterator map_i;

	// Alle Elemente der Map durchlaufen
	for (map_i = map.begin(); map_i != map.end(); map_i++) {

		QStringList elements = map_i.key().split("/");

		int x = 0;
		// Zu schlie�ende Elemente ermitteln
		while(x < prev_elements.size() && elements.at(x) == prev_elements.at(x)) {
			x++;
		}

		// Elemente schlie�en
		for(int i = prev_elements.size() - 1; i >= x; i--) {
			xmlWriter.writeEndElement();
		}

		// Elemente �ffnen
		for (int i = x; i < elements.size(); i++) {
			xmlWriter.writeStartElement(elements.at(i));
		}

		// Wert eintragen
		xmlWriter.writeCharacters(map_i.value().toString());

		prev_elements = elements;
	}

	// Noch offene Elemente schlie�en
	for(int i = 0; i < prev_elements.size(); i++) {
		xmlWriter.writeEndElement();
	}

	xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
	return true;
}

#endif /* XMLCONFIG_H_ */
