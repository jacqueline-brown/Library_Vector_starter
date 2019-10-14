#include "../includes_usr/fileIO.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
/* clears, then loads books from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries in books
 * 			SUCCESS if all data is loaded
 * */
int loadBooks(std::vector<book> &books, const char *filename) {
	books.clear();
	ifstream bookFile;
	bookFile.open(filename);
	if (!(bookFile.is_open()))
		return COULD_NOT_OPEN_FILE;
	while (!(bookFile.eof())) {
		string line;
		getline(bookFile, line);
		stringstream ss(line);
		string vectorItem;
		book item;
		getline(ss, vectorItem, ',');
		if(vectorItem == ""){
			break;
		}
		int bookidInt = stoi(vectorItem);
		item.book_id = bookidInt;
		getline(ss, vectorItem, ',');
		item.title.append(vectorItem);
		getline(ss, vectorItem, ',');
		item.author.append(vectorItem);
		getline(ss, vectorItem, ',');
		if (vectorItem == "0")
			item.state = book_checkout_state::UNKNOWN;
		if (vectorItem == "1")
			item.state = book_checkout_state::IN;
		if (vectorItem == "-1")
			item.state = book_checkout_state::OUT;
		getline(ss, vectorItem, ',');
		int loanedInt = stoi(vectorItem);
		item.loaned_to_patron_id = loanedInt;
		books.push_back(item);
		/*while (getline(ss, vectorItem, ',')) {
		 count = count + 1;
		 book item;
		 switch (count) {
		 case 1: {
		 item.book_id = stoi(vectorItem);
		 break;
		 }
		 case 2: {
		 item.title.append(vectorItem);
		 break;
		 }
		 case 3: {
		 item.author.append(vectorItem);
		 break;
		 }
		 case 4: {
		 if (vectorItem == "0")
		 item.state = book_checkout_state::UNKNOWN;
		 if (vectorItem == "1")
		 item.state = book_checkout_state::IN;
		 if (vectorItem == "-1")
		 item.state = book_checkout_state::OUT;
		 break;
		 }

		 case 5: {
		 item.loaned_to_patron_id = stoi(vectorItem);
		 break;
		 }
		 }
		 if (count == 5) {
		 books.push_back(item);
		 }
		 }*/
	}
	if (books.empty())
		return NO_BOOKS_IN_LIBRARY;
	bookFile.close();
	return SUCCESS;
}

/* serializes books to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries books (do not create file)
 * 			SUCCESS if all data is saved
 * */
int saveBooks(std::vector<book> &books, const char *filename) {
	if (books.empty()) {
		return NO_BOOKS_IN_LIBRARY;
	}
	ofstream bookFile;
	bookFile.open(filename);
	if (!(bookFile.is_open())) {
		return COULD_NOT_OPEN_FILE;
	}
	for (int i = 0; i < signed(books.size()); ++i) {
		string bookInfo;
		bookInfo = to_string(books[i].book_id) + "," + books[i].title + ","
				+ books[i].author + "," + to_string(books[i].state) + ","
				+ to_string(books[i].loaned_to_patron_id);
		bookFile << bookInfo << endl;
	}
	bookFile.close();
	return SUCCESS;
}

/* clears, then loads patrons from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons
 * 			SUCCESS if all data is loaded
 * */
int loadPatrons(std::vector<patron> &patrons, const char *filename) {
	patrons.clear();
	ifstream patronFile;
	patronFile.open(filename);
	if (!(patronFile.is_open()))
		return COULD_NOT_OPEN_FILE;
	while (!(patronFile.eof())) {
		string line;
		getline(patronFile, line);
		stringstream ss(line);
		string vectorItem;
		patron person;
		getline(ss, vectorItem, ',');
		if(vectorItem == ""){
			break;
		}
		person.patron_id = stoi(vectorItem);
		getline(ss, vectorItem, ',');
		person.name.append(vectorItem);
		getline(ss, vectorItem, ',');
		person.number_books_checked_out = stoi(vectorItem);
		patrons.push_back(person);
//		while (getline(ss, vectorItem, ',')) {
//			count = count + 1;
//			patron person;
//			switch (count) {
//			case 1: {
//				person.patron_id = stoi(vectorItem);
//				break;
//			}
//			case 2: {
//				person.name.append(vectorItem);
//				break;
//			}
//			case 3: {
//				person.number_books_checked_out = stoi(vectorItem);
//				break;
//			}
//			}
//			if (count == 3) {
//				patrons.push_back(person);
//			}
//		}
	}
	if (patrons.empty())
		return NO_PATRONS_IN_LIBRARY;
	patronFile.close();
	return SUCCESS;
}

/* serializes patrons to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons  (do not create file)
 * 			SUCCESS if all data is saved
 * */
int savePatrons(std::vector<patron> &patrons, const char *filename) {
	if (patrons.empty()) {
		return NO_PATRONS_IN_LIBRARY;
	}
	ofstream patronFile;
	patronFile.open(filename);
	if (!(patronFile.is_open())) {
		return COULD_NOT_OPEN_FILE;
	}
	for (int i = 0; i < signed(patrons.size()); ++i) {
		string patronInfo;
		patronInfo = to_string(patrons[i].patron_id) + "," + patrons[i].name
				+ "," + to_string(patrons[i].number_books_checked_out);
		patronFile << patronInfo << endl;
	}
	patronFile.close();
	return SUCCESS;
}
