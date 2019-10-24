#include <stdio.h>	//Ein- und Ausgabe
#include <string.h>	//Zeichenkettenoperatoren z.B. strcpy(), strcat()
#include <stdlib.h> //Funktionen zur Umwandlung von Zahlen, Speicherverwaltung ect.
#include <time.h>	//für die time() Funktion, Datum und Uhrzeit


//Author: Matthias und Levin
//Struktur namens "Person" vorgeben
typedef struct Person {
	char Vorname[40];
	char Nachname[40];
	int Jahrgang;
	struct Person *pNext;
	struct Person *pPrevious;
}struPerson;

//Author: Levin und Matthias
//Liste erstellen und Elemente nach der Struktur "Person" einfügen
struPerson* CreatePerson(unsigned int Anzahl) {
	struPerson *pTempPrevious = NULL;
	struPerson *pStart = NULL;
	for (unsigned int i = 0; i < Anzahl; i++) {	//Liste wird von Vorne nach Hinten erstellt
		struPerson *pNew = (struPerson*)malloc(sizeof(struPerson)); //dynamische Speicherzuweisung, Es wird dynamischer Speicherplatz für die Struktur "Person" reserviert und dem "pNew"-Pointer zugewiesen.
		char TempN[2] = { 'A' + (rand() % 26), '\0' };	//Um ein NULL terminiertes Zeichenarray zu erzeugen
		char TempV[2] = { 'A' + (rand() % 26), '\0' };
		strcpy_s(pNew->Nachname, TempN);	//Jeder char Zeichen in "TempN"-Array wird in das aktuelle Element als Nachnamen abgespeichert, bis es das Letze Zeichen '\0' erreicht hat
		strcpy_s(pNew->Vorname, TempV);
		pNew->Jahrgang = 1900 + (rand() % 118);
		pNew->pPrevious = pTempPrevious;
		pNew->pNext = NULL;
		if (i != 0) {
			pTempPrevious->pNext = pNew;	//Ist es nicht das erste Element, wird dem vorherigem Element als "pNext" das aktuelle Element eingetragen
		}
		else {
			pStart = pNew;	//Ist es das erste Element, wird als Anfangspunkt der Liste dieses Element festgelegt
		}
		pTempPrevious = pNew;	//Bevor ein neues Element erstellt wird, wird das aktuelle Element für den Nachfolger in "pTempPrevious" refferenziert
	}
	return pStart;
}

//Author: Levin
//Inhalt der ganzen Liste löschen
struPerson* DeleteAll(struPerson* plist) {
	if (plist != NULL) {
		while (plist != NULL) {
			struPerson* pTemp = plist->pNext;	//Die Referenz des nachfolgenden Element (vom aktuellen Element) wird in "pTemp" zwischengespeichert
			free(plist);	//der Speicherplatz des Elementes wird frei gegeben
			plist = pTemp;	//Das Element "pTemp" ist nun das aktuelle Element
		}
		printf("info --> Alle Elemente in der Liste sind gel\x94scht.\n");
	}
	else {
		printf("Keine Elemente in der Liste vorhanden.\n");
	}
	return plist;
}

//Author: Levin
//Löscht Personen aus der Liste, welche mit der Eingabe (Vor-,Nachname) übereinstimmen
struPerson* DeletePerson(struPerson* plist, char Vorname, char Nachname) {
	struPerson* pStart = NULL;
	pStart = plist;
	if (plist != NULL) {
		int Count = 0;	//Die Anzahl gelöschten Personen zu zählen, zusätzlich um den Benutzer darüber informieren
		while (plist != NULL) {
			bool hasnext = plist->pNext != NULL;	//Prüft, ob das atkuelle Element ein nächstes Element besitzt
			bool hasprevious = plist->pPrevious != NULL;	//Prüft, on das aktuelle Element ein vorhergehendes Element besitzt
			if (plist->Vorname[0] == Vorname && plist->Nachname[0] == Nachname) {
				Count++;
				if (hasprevious == false && hasnext == true) {	//Wenn es das erste Element in der Liste ist
					pStart = plist->pNext;	//Startpunkt der Liste auf das Nächste Element setzen
					free(pStart->pPrevious);	//Speicherplatz des vorherigen Elements freigeben
					pStart->pPrevious = NULL;
					plist = pStart;
				}
				else if (hasprevious == true && hasnext == true) {
					struPerson* pTemp = plist;	//Die Referenz auf das aktuelle Element wird zwischengespeichert
					plist = plist->pNext;	//Es wird auf das nächste Element gewechselt
					plist->pPrevious->pPrevious->pNext = plist;
					plist->pPrevious = plist->pPrevious->pPrevious;
					free(pTemp);	//Der Speicherplatz des vorhergehenden Element "pTemp" wird freigegeben
				}
				else if (hasprevious == false && hasnext == false) {	//Bei einem Element in der Liste
					free(plist);
					pStart = NULL;
					break;
				}
				else if (hasprevious == true && hasnext == false) {	//Wenn es das letzte Element in der Liste ist
					plist->pPrevious->pNext = NULL;
					free(plist);
					break;
				}
			}
			else {
				plist = plist->pNext;
			}
		}
		printf("info --> Es wurde/n %i Person/en gel\x94scht.\n", Count);	//Benutzer über die Anzahl gelöschten Elemente informieren
	}
	else {
		printf("Keine Elemente zum l\x94schen in der Liste vorhanden.\n");
	}
	return pStart;
}

//Author: Levin
//Sortiert die Liste zuerst nach Nachnamen und dann nach Vornamen
struPerson* SortSlow(struPerson* plist) {
	struPerson* pStart = NULL;
	pStart = plist;
	int Count = 0;	//Speichert die Anzahl der Umsortierungen
	if (plist != NULL) {
		while (plist != NULL) {	//Für zusätzliche Sicherheit "(plist != NULL){", könnte auch "(true){" heissen
			bool hasnext = plist->pNext != NULL;
			bool hasprevious = plist->pPrevious != NULL;
			if (hasprevious == false && hasnext == true) {	//Wenn sich das Element am Anfang der Liste befindet
				if (plist->Nachname[0] >= plist->pNext->Nachname[0]) {
					if (plist->Nachname[0] == plist->pNext->Nachname[0] && plist->Vorname[0] <= plist->pNext->Vorname[0]) {
						plist = plist->pNext;	//Wenn Nachnahmen und Vornamen bei beiden Elementen die selben sind oder der Vorname zusätzlich kleiner oder gleich ist, ist das Element schon am richtigen Ort (= weitergehen)
					}
					else {	//Die Position des aktuelle Element mit dem Nachfolgenden tauschen
						plist->pPrevious = plist->pNext;
						plist->pNext = plist->pNext->pNext;
						plist->pPrevious->pPrevious = NULL;
						plist->pNext->pPrevious = plist;
						plist->pPrevious->pNext = plist;
						pStart = plist->pPrevious;
						Count++;
					}
				}
				else {	//(Element ist schon am richtigen Ort = ) nächstes Element überprüfen
					plist = plist->pNext;
				}
			}
			else if (hasprevious == true && hasnext == true) {	//Wenn sich das Element irgendwo zwischen zwei Elementen befindet
				if (plist->Nachname[0] >= plist->pNext->Nachname[0]) {
					if (plist->Nachname[0] == plist->pNext->Nachname[0] && plist->Vorname[0] <= plist->pNext->Vorname[0]) {
						plist = plist->pNext;
					}
					else {	//Die Position des aktuelle Element mit dem Nachfolgenden tauschen, da der Nachname oder der Vorname des aktuellen Elementes grösser ist
						struPerson* pTemp = NULL;
						pTemp = plist->pNext->pNext;	//Die Referenz auf das übernächste Element zwischenspeichern
						plist->pNext->pPrevious = plist->pPrevious;
						plist->pPrevious->pNext = plist->pNext;
						plist->pPrevious = plist->pNext;
						if (pTemp != NULL) {	//Wenn "pTemp": NULL wäre, gäbe es beim jetztigen aktuellen Element mit der getauschten Position kein weiteres Element
							plist->pNext->pNext->pPrevious = plist;
						}
						plist->pNext->pNext = plist;	//Muss wegen der Reihenfolge hier dazwischen sein
						plist->pNext = pTemp;	//Jetzt setzte ich den neuen Nachfolger des aktuellen Elements: Das früher übernächste Element
						Count++;
						if (pTemp == NULL) {
							plist = pStart;		//Schleife vom Anfang der Liste wieder beginnen
							Count = 0;
						}
					}
				}
				else {
					plist = plist->pNext;
				}
			}
			else if (hasprevious == true && hasnext == false) {	//Wenn sich das Element am Ende der Liste befindet
				if (Count != 0) {	//Die Schleife wird solage druchlaufen, bis keine Umsortierung mehr statt gefunden hat
					plist = pStart;	//Elemente der Liste wieder vom Anfang durchgehen 
					Count = 0;
				}
				else {
					break;
				}
			}
		}
	}
	else {
		printf("Keine Elemente in der Liste vorhanden.\n");
	}
	return pStart;
}

//Author: Levin und Matthias
//Auslagerung von der Sotierung: Prüft, ob nächstes Element das letzte in der Liste ist und reagiert entsprechend
struPerson* SortQuickHelp(struPerson* pNewList, struPerson* plist, struPerson* pStart) {
	if (pNewList->pNext != NULL) {	//Überprüfen, ob das aktuelle Element das Letzte in der Liste ist
		pNewList = pNewList->pNext;	//Es gibt noch ein weiteres Element und es wird zur weiteren Überprüfung auf dieses gewechselt
	}
	else {	//Das aktuelle "plist"-Element als hinterstes in die Liste von "pNewList" setzen
		pNewList->pNext = plist;
		plist->pPrevious = pNewList;
		plist->pNext = NULL;
		pNewList = pStart;	//"pNewList" zeigt wieder auf das Startelement in der Liste und ist bereit für die nächste Überprüfung
	}
	return pNewList;	//Rückgabe, wo der Pointer "pNewList" hinzeigt
}

//Author: Levin, Matthias
//Sortieren mit höheren Geschwindigkeit: zuerst nach Nachnamen und dann nach Vornamen sortieren
struPerson* SortQuick(struPerson* plist, char sortBy) {
	struPerson* pNewList = NULL;	//Erstellen einer zweiten Liste für die Sortierung
	struPerson* pStart = plist;	//Abspeicherung des Startpunktes der Liste
	struPerson* pListpNext = NULL;
	if (plist != NULL) {
		while (plist != NULL) {	//Jedes Element in der Liste "plist" durchgehen
			pListpNext = plist->pNext;	//Abspeicherung des Pointers auf das nächste Element, da das aktuelle Element verändert wird
			if (pNewList == NULL) {	//Existiert kein Element in der "pNewList"-Liste wird das aktuelle "plist"-Element direkt darin referenziert
				pNewList = plist;
				pNewList->pNext = NULL;
				pNewList->pPrevious = NULL;
				pStart = pNewList;
			}
			else {
				while (pNewList != NULL) {	//Jedes Element in der Liste "pNewList" durchgehen
					bool firstCriteria, secondCriteria;
					switch (sortBy) {
					case 'v':
						firstCriteria = plist->Vorname[0] <= pNewList->Vorname[0];
						secondCriteria = plist->Vorname[0] == pNewList->Vorname[0] && plist->Nachname[0] > pNewList->Nachname[0];
						break;
					case 'j':
						firstCriteria = plist->Jahrgang <= pNewList->Jahrgang;
						secondCriteria = plist->Jahrgang == pNewList->Jahrgang && plist->Nachname[0] > pNewList->Nachname[0];
						break;  
						//case 'n':
					default:
						firstCriteria = plist->Nachname[0] <= pNewList->Nachname[0];
						secondCriteria = plist->Nachname[0] == pNewList->Nachname[0] && plist->Nachname[0] > pNewList->Nachname[0];
						break;
					}
					if (firstCriteria) {
						if (secondCriteria) {	//Wenn dies zutrifft, soll weiter überprüft werden
							pNewList = SortQuickHelp(pNewList, plist, pStart);	//Überprüfung ob nächstes Element das letzte Element in der Liste ist
							if (pNewList == pStart) {	//Überprüfen, ob "pNewList" auf das Startelement der Liste zeigt, denn dann ist die Überprüfung dieses Elementes abgeschlossen
								break;
							}
						}
						else {	//Das aktuelle "plist"-Element vor das aktuelle "pNewList"-Element setzen
							if (pNewList->pPrevious == NULL) { //Überprüfen, ob das aktuelle "pNewList"-Element das erste Element in der Liste ist
								pNewList->pPrevious = plist;
								plist->pNext = pNewList;
								plist->pPrevious = NULL;
								pStart = plist;
								pNewList = pStart;
								break;
							}
							else {	//Das aktuelle "plist"-Element vor das aktuelle "pNewList"-Element setzen (Zwischen zwei Elementen)
								plist->pPrevious = pNewList->pPrevious;
								plist->pNext = pNewList;
								pNewList->pPrevious = plist;
								plist->pPrevious->pNext = plist;
								pNewList = pStart;
								break;
							}
						}
					}
					else {	//Ist der Nachname des aktuellen "plist"-Elementes nicht grösser als der Nachname des aktuellen "pNewList"-Elementes:
						pNewList = SortQuickHelp(pNewList, plist, pStart);
						if (pNewList == pStart) {
							break;
						}
					}
				}
			}
			plist = pListpNext;
		}
	}
	else {
		printf("Keine Elemente in der Liste vorhanden.\n");
	}
	return pStart;
}

//Author: Levin
//Personen aus der Liste bis zu einer bestimmten Anzahl ausgeben
void OutputUntil(struPerson* plist, int Anzahl) {
	if (plist != NULL) {
		for (int i = 0; i < Anzahl && plist != NULL; i++) {
			printf("Person: %p\n", plist);
			printf("Name: ");
			for (int i = 0; plist->Nachname[i] != '\0'; i++) {	//Den abgespeicherten Nachnamen im aktuellen Element, Zeichen für Zeichen ausgeben, bis das Ende '\0' erreicht ist
				printf("%c", plist->Nachname[i]);
			}
			printf("\n");
			printf("Vorname: ");
			for (int i = 0; plist->Vorname[i] != '\0'; i++) {
				printf("%c", plist->Vorname[i]);
			}
			printf("\n");
			printf("Jahrgang: %i\n", plist->Jahrgang);
			for (int i = 0; i <= 20; i++) {	//Um die Ergebnisse in der Ausgabe grafisch abzugrenzen
				printf("-");
				if (i == 20)
					printf("\n");
			}
			plist = plist->pNext;
		}
	}
	else {
		printf("Keine Elemente in der Liste vorhanden.\n");
	}
}

//Author: Levin
//Alle Personen in der Liste auf dem Display ausgeben
void OutputAll(struPerson* plist) {
	if (plist != NULL) {	//Befindet sich in der Liste kein Element, kann die Ausgabe nicht stattfinden
		while (plist != NULL) {
			printf("Person: %p\n", plist);
			printf("Name: ");
			for (int i = 0; plist->Nachname[i] != '\0'; i++) {
				printf("%c", plist->Nachname[i]);
			}
			printf("\n");
			printf("Vorname: ");
			for (int i = 0; plist->Vorname[i] != '\0'; i++) {
				printf("%c", plist->Vorname[i]);
			}
			printf("\n");
			printf("Jahrgang: %i\n", plist->Jahrgang);
			for (int i = 0; i <= 20; i++) {
				printf("-");
				if (i == 20)
					printf("\n");
			}
			plist = plist->pNext;
		}
	}
	else {
		printf("Keine Elemente in der Liste vorhanden.\n");
	}
}

//Author: Levin
//für die Ausgabe der möglichen Benutzereingaben
void OutputMenue() {
	printf("Bitte geben Sie eine entsprechenden Buchstaben ein:\n");
	printf("m --> f\x81r das Ausgeben des Men\x81s\n");
	printf("c --> f\x81r das Erstellen von Personen\n");
	printf("d --> f\x81r das L\x94schen von bestimmten Personen in der Liste\n");
	printf("a --> f\x81r das L\x94schen aller Personen in der Liste\n");
	printf("o --> f\x81r das Ausgeben von Personen\n");
	printf("s --> f\x81r das Sortieren der Liste nach Nachnamen und Vornamen der Personen\n");
	printf("q --> f\x81r das Beenden der Applikation\n>> ");
}


//Author: Matthias und Levin
int main() {
	srand((unsigned)time(NULL)); //Generiert eine Zufallszahl mit der akuellen Zeit als Seed. Alle kommenden Zufallszahlen sind je nach Systemzeit unterschiedlich.

	struPerson* plist = NULL;
	
	char selection = NULL;
	OutputMenue();

	while (true) {
		selection = getchar();

		if (selection == '\n') {
			//Um die fehlerhafte getchar(), scanf_s() Eingabe die Eingabetaste abzufangen
		}
		else if (selection == 'm') {
			OutputMenue();
		}
		else if (selection == 'c') {
			int count = NULL;
			printf("Bitte geben Sie die Anzahl an:\n>> ");
			scanf_s("%i", &count);
			plist = CreatePerson(count);
			printf("\nIhre Eingabe wird erwartet:\n>> ");
		}
		else if (selection == 'd') {
			char Nachname = NULL;
			char Vorname = NULL;
			printf("Bitte geben sie den Anfangsbuchstaben des Nachnamens in Grossbuchstaben an:\n>> ");
			Nachname = getchar();
			if (Nachname == '\n') {	//Weil die Entertaste von der vorherigen Eingabe als diese Eingabe speichert wird.
				Nachname = getchar();
			}
			printf("Bitte geben Sie den Anfangsbuchstaben des Vornamens in Grossbuchstaben an:\n>> ");
			Vorname = getchar();
			if (Vorname == '\n') {	//Weil die Entertaste von der vorherigen Eingabe als diese Eingabe speichert wird.
				Vorname = getchar();
			}
			plist = DeletePerson(plist, Vorname, Nachname);
			printf("\nIhre Eingabe wird erwartet:\n>> ");
		}
		else if (selection == 'a') {
			plist = DeleteAll(plist);
			printf("\nIhre Eingabe wird erwartet:\n>> ");
		}
		else if (selection == 'o') {
			int count = NULL;
			printf("Bitte geben Sie die Anzahl an oder den Wert 0 f\x81r das Anzeigen aller Personen:\n>> ");
			scanf_s("%i", &count);
			if (count == 0) {
				OutputAll(plist);
			}
			else {
				OutputUntil(plist, count);
			}
			printf("\nIhre Eingabe wird erwartet:\n>> ");
		}
		else if (selection == 's') {
			printf("Filtern nach: (Leer = Nachname, dann Vorname)\n");
			printf("v --> Vorname (dann Nachname)\n");
			printf("n --> Nachname (dann Vorname)\n");
			printf("j --> Jahrgang (dann Vorname)\n>>");
			char sortBy = getchar();
			if (sortBy == '\n') {	//Weil die Entertaste von der vorherigen Eingabe als diese Eingabe speichert wird.
				sortBy = getchar();
			}
			plist = SortQuick(plist, sortBy);
			printf("\nIhre Eingabe wird erwartet:\n>> ");
		}
		else if (selection == 'q') {
			break;
		}
		else {
			printf("Ung\x81ltige Eingabe!: %c\n>> ", selection);
		}
	}
}
