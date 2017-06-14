/*
 * lang.c
 * 
 * Copyright 2017 Obiwac
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include "lang.h"

static string LANG = "en-uk";//"en-uk" for english

void set_lang(string lang) {
	LANG = lang;
	
	if (str_eql(lang, "fr-be")) set_layout("azerty");
	else if (str_eql(lang, "hu")) set_layout("qwertz");
	else if (str_eql(lang, "ge")) set_layout("qwertz");
	else set_layout("qwerty");
	
}

string get_lang(void) {
	return LANG;
	
}

string translate(string text) {
	if (str_eql(LANG, "fr-be")) {
		if (str_eql(text, "Tools")) return "Outils";
		if (str_eql(text, "Games")) return "Jeux";
		if (str_eql(text, "Random")) return "Ale`aatoire";
		if (str_eql(text, "My Tab")) return "Mon onglet";
		
		if (str_eql(text, "Please enter your password.")) return "Entrez votre mot de passe s'il vous plai`^t.";
		if (str_eql(text, "Are you sure you want to shutdown the computer?")) return "Etes-vous su`^r de vouloir e`atindre l'ordinateur ?";
		if (str_eql(text, "About the AQUA OS")) return "A propos du AQUA OS";
		
		if (str_eql(text, "Warning:")) return "Avertissement:";
		if (str_eql(text, "You do not have enough RAM.")) return "Vous n'avez pas assez de me`amoire vive.";
		if (str_eql(text, "You have")) return "Vous avez";
		if (str_eql(text, "megabytes of RAM.")) return "megaoctets de me`amoire vive.";
		if (str_eql(text, "Recommended amount is 512 megabytes of RAM.")) return "Recommende`a est 512 megaoctets.";
		
		if (str_eql(text, "Watch out:")) return "Attention:";
		if (str_eql(text, "Your file system seems to not be formatted.")) return "Ton systeme de fichiers n'a pas l'air d'e`^tre formatte.";
		if (str_eql(text, "The signature is")) return "La signature est";
		if (str_eql(text, "Your disk will be reformatted.")) return "Ton disque va e`^tre reformatte`a.";
		
		if (str_eql(text, "Draw")) return "Dessiner";
		if (str_eql(text, "Settings")) return "Parame`gtres";
		
		if (str_eql(text, "Random laser game")) return "Jeu de laser";
		if (str_eql(text, "Langton's ant")) return "La fourmille de Langton";
		
		if (str_eql(text, "About")) return "A propos";
		if (str_eql(text, "Log")) return "Registre";
		
		if (str_eql(text, "My notepad")) return "Mon bloc notes";
		if (str_eql(text, "My script")) return "Mon scripte";
		
		if (str_eql(text, "st of ")) return "er ";
		if (str_eql(text, "nd of ")) return "e`gme ";
		if (str_eql(text, "rd of ")) return "e`gme ";
		if (str_eql(text, "th of ")) return "e`gme ";
		
		if (str_eql(text, "January")) return "janvier";
		if (str_eql(text, "February")) return "fevrier";
		if (str_eql(text, "March")) return "mars";
		if (str_eql(text, "April")) return "avril";
		if (str_eql(text, "May")) return "mai";
		if (str_eql(text, "June")) return "juin";
		if (str_eql(text, "July")) return "juillet";
		if (str_eql(text, "August")) return "aou`^t";
		if (str_eql(text, "September")) return "septembre";
		if (str_eql(text, "October")) return "octobre";
		if (str_eql(text, "November")) return "novembre";
		if (str_eql(text, "December")) return "decembre";
		
		if (str_eql(text, "Hello! What is your name?")) return "Bonjour ! Comment vous apelez-vous ?";
		if (str_eql(text, "Hello")) return "Bonjour";
		if (str_eql(text, "Run dialog. Enter a command:")) return "Dialogue d'exe`acution. Entrez une commande:";
		
		if (str_eql(text, "You have unsaved changes. Are you sure you want to quit?")) return "Vous avez des changements non sauvgarde`aes. Etes vous su`^r de vouloir quitter ?";
		if (str_eql(text, "Are you sure you want to reformat the disk?")) return "Etes vous su`^r de vouloir reformatter le disque?";
		if (str_eql(text, "Your disk has now been formatted. (Shutdown is recomended)")) return "Votre disque a e`ate`a reformatte`a. (Fermeture de l'ordinateur reccomende`a)";
		if (str_eql(text, "Are you really sure? Disk may cause corruption.")) return "Etes vous vraiment su`^r ? Le disque risque de causer une corruption.";
		if (str_eql(text, "Are you sure you want to reset your settings?")) return "Etes vous su`^r de vouloir re`ainitialiser les parame`gtres ?";
		
		if (str_eql(text, "Reformat disk")) return "Reformatter le disque";
		if (str_eql(text, "Reset settings")) return "Re`ainitialiser les parame`gtres";
		if (str_eql(text, "Startup assistant")) return "Assistant de de`amarrage";
		if (str_eql(text, "Disable warnings")) return "De`asactiver les avertissements";
		if (str_eql(text, "Fast transitions")) return "Transitions rapides";
		
		return "UPDATE THIS!!!";
		
	} else if (str_eql(LANG, "hu")) {
		if (str_eql(text, "Tools")) return "Eszko`:zo`:k";
		if (str_eql(text, "Games")) return "Ja`ate`akok";
		if (str_eql(text, "Random")) return "Egye`ab";
		if (str_eql(text, "My Tab")) return "En lapfu`:lem";
		
		if (str_eql(text, "Please enter your password.")) return "Ke`arlek, i`ard be a jelszavad.";
		if (str_eql(text, "Are you sure you want to shutdown the computer?")) return "Ki szeretne`ad kapcsolni a sza`ami`ato`age`apet?";
		if (str_eql(text, "About the AQUA OS")) return "Informa`acio`a az AQUA OS-rol";
		
		if (str_eql(text, "Warning:")) return "Figyelem:";
		if (str_eql(text, "You do not have enough RAM.")) return "Nincs ele`ag memo`aria.";
		if (str_eql(text, "You have")) return "Rendelkeze`asre a`all";
		if (str_eql(text, "megabytes of RAM.")) return "Mbyte memo`aria.";
		if (str_eql(text, "Recommended amount is 512 megabytes of RAM.")) return "Az aja`anlott mennyise`ag 512 Mbyte memo`aria.";
		
		if (str_eql(text, "Watch out:")) return "Figyelem:";
		if (str_eql(text, "Your file system seems to not be formatted.")) return "Ugy tu`~nik, a fa`ajlrendszer nincs forma`azva.";
		if (str_eql(text, "The signature is")) return "Az ala`ai`ara`asod";
		if (str_eql(text, "Your disk will be reformatted.")) return "A lemezed le lesz forma`azva.";
		
		if (str_eql(text, "Draw")) return "Rajzolo`a";
		if (str_eql(text, "Settings")) return "Bea`alli`ata`asok";
		
		if (str_eql(text, "Random laser game")) return "Ve`aletlenszeru`~ le`azer ja`ate`ak";
		if (str_eql(text, "Langton's ant")) return "Langton hangya`aja";
		
		if (str_eql(text, "About")) return "Informa`acio`a";
		if (str_eql(text, "Log")) return "Hajo`anaplo`a";
		
		if (str_eql(text, "My notepad")) return "Noteszem";
		if (str_eql(text, "My script")) return "Szkriptem";
		
		if (str_eql(text, "st of ")) return ".";
		if (str_eql(text, "nd of ")) return ".";
		if (str_eql(text, "rd of ")) return ".";
		if (str_eql(text, "th of ")) return ".";
		
		if (str_eql(text, "January")) return "Janua`gr";
		if (str_eql(text, "February")) return "Februa`gr";
		if (str_eql(text, "March")) return "Ma`arcius";
		if (str_eql(text, "April")) return "Aprilis";
		if (str_eql(text, "May")) return "Ma`ajus";
		if (str_eql(text, "June")) return "Ju`anius";
		if (str_eql(text, "July")) return "Ju`alius";
		if (str_eql(text, "August")) return "Augusztus";
		if (str_eql(text, "September")) return "Szeptember";
		if (str_eql(text, "October")) return "Okto`aber";
		if (str_eql(text, "November")) return "November";
		if (str_eql(text, "December")) return "December";
		
		if (str_eql(text, "Hello! What is your name?")) return "Udvo`:zletem, hogy hi`avnak?";
        if (str_eql(text, "Hello")) return "Udvo`:zlu`:nk";
        if (str_eql(text, "Run dialog. Enter a command:")) return "Futtata`as. Ird be a parancsot:";

        if (str_eql(text, "Reformat disk")) return "Lemez u`ajraforma`aza`asa";
        if (str_eql(text, "Reset settings")) return "Bea`allíta`asok to`:rle`ase";
        if (str_eql(text, "Startup assistant")) return "Indi`ato`a asszisztens";
        if (str_eql(text, "Disable warnings")) return "Figyelmeztete`asek kikapcsola`asa";
        if (str_eql(text, "Fast transitions")) return "Gyors a`atviteleze`as";

        if (str_eql(text, "You have unsaved changes. Are you sure you want to quit?")) return "Me`ag nincsenek elmentve a bea`alli`ata`asok. Biztos kile`apsz?";
        if (str_eql(text, "Are you sure you want to reformat the disk?")) return "Biztos, hogy u`ajraforma`azod a lemezt?";
        if (str_eql(text, "Your disk has now been formatted. (Shutdown is recomended)")) return "A lemezt forma`aztad. (Aja`anlott a ge`apet u`ajraindi`atani)";
        if (str_eql(text, "Are you really sure? Disk may cause corruption.")) return "Biztos? Megse`aru`:lhet a fa`ajlrendszer.";
        if (str_eql(text, "Are you sure you want to reset your settings?")) return "Biztos, hogy visszatérsz az alapbea`alli`ata`asokhoz?";
		
		return "UPDATE THIS!!!";
		
	} else if (str_eql(LANG, "ge")) {
		if (str_eql(text, "Tools")) return "Werkzeuge";
		if (str_eql(text, "Games")) return "Spiele";
		if (str_eql(text, "Random")) return "Sonstiges";
		if (str_eql(text, "My Tab")) return "Mein Tab";
		
		if (str_eql(text, "Please enter your password.")) return "Bitte geben Sie Ihr Passwort ein.";
		if (str_eql(text, "Are you sure you want to shutdown the computer?")) return "Mo`:chten Sie den Rechner ausschalten?";
		if (str_eql(text, "About the AQUA OS")) return "Uber AQUA OS";
		
		if (str_eql(text, "Warning:")) return "Warnung:";
		if (str_eql(text, "You do not have enough RAM.")) return "Nicht genug Arbeitsspeicher.";
		if (str_eql(text, "You have")) return "Sie haben";
		if (str_eql(text, "megabytes of RAM.")) return "MB Arbeitsspeicher.";
		if (str_eql(text, "Recommended amount is 512 megabytes of RAM.")) return "Es werden mindestens 512 Megabyte Arbeitsspeicher empfohlen.";
		
		if (str_eql(text, "Watch out:")) return "Achtung:";
		if (str_eql(text, "You file system seems to not be formatted.")) return "Ihr Dateisystem scheint nicht formatiert zu sein.";
		if (str_eql(text, "The signature is")) return "Die Signatur ist";
		if (str_eql(text, "Your disk will be reformatted.")) return "Ihre Festplatte wird formatiert.";
		
		if (str_eql(text, "Draw")) return "Draw";
		if (str_eql(text, "Settings")) return "Einstellungen";
		
		if (str_eql(text, "Random laser game")) return "Zufa`:lliges Laserspiel";
		if (str_eql(text, "Langton's ant")) return "Langtons Ameise";
		
		if (str_eql(text, "About")) return "Uber";
		if (str_eql(text, "Log")) return "Protokoll";
		
		if (str_eql(text, "My notepad")) return "Notizen";
        if (str_eql(text, "My script")) return "Mein Skript";
		
		if (str_eql(text, "st of ")) return "st von ";
		if (str_eql(text, "nd of ")) return "te von ";
		if (str_eql(text, "rd of ")) return "te von";
		if (str_eql(text, "th of ")) return "te von ";
		
		if (str_eql(text, "January")) return "Januar";
		if (str_eql(text, "February")) return "Februar";
		if (str_eql(text, "March")) return "Ma`:rz";
		if (str_eql(text, "April")) return "April";
		if (str_eql(text, "May")) return "Mai";
		if (str_eql(text, "June")) return "Juni";
		if (str_eql(text, "July")) return "Juli";
		if (str_eql(text, "August")) return "August";
		if (str_eql(text, "September")) return "September";
		if (str_eql(text, "October")) return "Oktober";
		if (str_eql(text, "November")) return "November";
		if (str_eql(text, "December")) return "Dezember";
		
        if (str_eql(text, "Hello! What is your name?")) return "Hallo! Wie heibt du?"; //heißt
        if (str_eql(text, "Hello")) return "Hallo";
        if (str_eql(text, "Run dialog. Enter a command:")) return "Ausfu`:hren. Geben sie einen Befehl ein:";

        if (str_eql(text, "Reformat disk")) return "Festplatte formatieren";
        if (str_eql(text, "Reset settings")) return "Einstellungen zuru`:cksetzen";
        if (str_eql(text, "Startup assistant")) return "Einrichtungsassistent";
        if (str_eql(text, "Disable warnings")) return "Warnungen abschalten";
        if (str_eql(text, "Fast transitions")) return "Schnelle Uberga`:nge";

        if (str_eql(text, "You have unsaved changes. Are you sure you want to quit?")) return "Sie haben ungespeicherte Vera`:nderungen vorgenummen. Mo`:chten Sie das Programm trotzdem beenden?";
        if (str_eql(text, "Are you sure you want to reformat the disk?")) return "Sind Sie sich sicher, dass Sie die Festplatte formatieren mo`:chten?";
        if (str_eql(text, "Your disk has now been formatted. (Shutdown is recomended)")) return "Ihre Festplatte wurde formatiert. (Herunterfaren wird empfohlen)";
        if (str_eql(text, "Are you really sure? Disk may cause corruption.")) return "Sind Sie sich sicher? Die Festplatte ko`:nnte bescha`:digt werden.";
        if (str_eql(text, "Are you sure you want to reset your settings?")) return "Mo`:chten Sie Ihre Einstellungen wirklich zuru`:cksetzen?";
		
		return "UPDATE THIS!!!";
		
	} else if (str_eql(LANG, "en-uk")) {
		return text;
		
	} else {
		return text;
	
	}
	
}
