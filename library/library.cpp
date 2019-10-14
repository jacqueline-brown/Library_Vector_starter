#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <iostream>

#include "../includes_usr/library.h"
#include "../includes_usr/datastructures.h"
#include "../includes_usr/fileIO.h"
using namespace std;

//NOTE: please ensure patron and book data are loaded from disk before calling the following
//NOTE: also make sure you save patron and book data to disk any time you make a change to them
//NOTE: for files where data is stored see constants.h BOOKFILE and PATRONFILE
vector<book> books;
vector<patron> patrons;
/*
 * clear books and patrons containers
 * then reload them from disk 
 */
void reloadAllData() {
	books.clear();
	patrons.clear();
	loadBooks(books, BOOKFILE.c_str());
	loadPatrons(patrons, PATRONFILE.c_str());
}

/* checkout a book to a patron
 * first load books and patrons from disk
 * make sure patron enrolled (patronid is assigned to a patron in patrons container)
 * make sure book in collection (bookid is assigned to a book in books container)
 * 
 * see if patron can check out any more books 
 * 	if not return TOO_MANY_OUT patron has the MAX_BOOKS_ALLOWED_OUT
 * 	
 * if so then check the book out to the patron, set the following fields for the book in the
 * books container
 * book.loaned_to_patron_id = patronid;
 * book.state = OUT;
 * 
 * Finally save the contents of the books and patrons containers to disk
 * 
 * returns SUCCESS checkout worked
 *         PATRON_NOT_ENROLLED
 * 		   BOOK_NOT_IN_COLLECTION
 *         TOO_MANY_OUT patron has the max number of books allowed checked out
 */
int checkout(int bookid, int patronid) {
	reloadAllData();
	//int num = patrons.at(patronid).number_books_checked_out;
	bool found1 = false;
	if(patronid == BOGUS_USER)
		return PATRON_NOT_ENROLLED;
	if (bookid == BOGUS_BOOK)
		return BOOK_NOT_IN_COLLECTION;
	for (int index = 0; index <= signed(patrons.size()); index++) {
		if (patrons[index].patron_id == patronid) {
			found1 = true;
			break;
		}
	}
	bool found2 = false;
	for (int j = 0; j < signed(books.size()); j++) {
		if (books[j].book_id == bookid) {
			found2 = true;
			break;
		}
	}
	int num = patrons.at(patronid).number_books_checked_out;
	if (found1 && num == MAX_BOOKS_ALLOWED_OUT) {
		return TOO_MANY_OUT;
	}
	else if (found2 && found1) {
		books.at(bookid).loaned_to_patron_id = patronid;
		books.at(bookid).state = book_checkout_state::OUT;
		int newBookNumber = patrons.at(patronid).number_books_checked_out;
		newBookNumber += 1;
		patrons.at(patronid).number_books_checked_out = newBookNumber;

	}
	/*else if (!found1 ) {
	 return PATRON_NOT_ENROLLED;
	 }*/
	/*else if (!found2) {
	 return BOOK_NOT_IN_COLLECTION;
	 }*/
	savePatrons(patrons, PATRONFILE.c_str());
	saveBooks(books, BOOKFILE.c_str());
	return SUCCESS;
}

/* check a book back in 
 * first load books and patrons from disk
 * make sure book in collection (bookid is assigned to a book in books container)
 *  	
 * if so find the the patron the book is checked out to and decrement his/hers number_books_checked_out
 * then check the book back in by marking the book.loaned_to_patron_id = NO_ONE and the book.state = IN;
 
 * Finally save the contents of the books and patrons containers to disk
 * 
 * returns SUCCESS checkout worked
 * 		   BOOK_NOT_IN_COLLECTION
 */
int checkin(int bookid) {
	reloadAllData();
	bool found = false;
	for (int i = 0; i < signed(books.size()); i++) {
		if (books[i].book_id == bookid) {
			found = true;
			break;
		}
	}
	if (found) {
		int patronId = books.at(bookid).loaned_to_patron_id;
		int newBookNumber = patrons.at(patronId).number_books_checked_out;
		newBookNumber -= 1;
		patrons.at(patronId).number_books_checked_out = newBookNumber;
		books.at(bookid).state = book_checkout_state::IN;
	} else {
		return BOOK_NOT_IN_COLLECTION;
	}
	savePatrons(patrons, PATRONFILE.c_str());
	saveBooks(books, BOOKFILE.c_str());
	return SUCCESS;
}

/*
 * enroll a patron, duplicate names are fine as patrons are uniquely identified by their patronid
 * first load books and patrons from disk
 * create a patron object, initialize its fields as appropriate, assign him/her the nextPatronID
 * then push the patron onto the patrons container
 * save all changes to the patrons container to disk
 * return 
 *    the patron_id of the person added
 */
int enroll(std::string &name) {
	reloadAllData();
	patron newPerson;
	int patronid = patrons.size();
	newPerson.patron_id = patronid;
	newPerson.name = name;
	newPerson.number_books_checked_out = 0;
	patrons.push_back(newPerson);
	savePatrons(patrons, PATRONFILE.c_str());
	return newPerson.patron_id;
}

/*
 * the number of books in the books container
 * (ie. if 3 books returns 3)
 * 
 */
int numbBooks() {
	reloadAllData();
	int size = books.size();
	return size;
}

/*
 * the number of patrons in the patrons container
 * (ie. if 3 patrons returns 3)
 */
int numbPatrons() {
	reloadAllData();
	return patrons.size();
}

/*the number of books patron has checked out
 *
 *returns a positive number indicating how many books are checked out 
 *        or PATRON_NOT_ENROLLED         
 */
int howmanybooksdoesPatronHaveCheckedOut(int patronid) {
	reloadAllData();
	if(patronid == BOGUS_USER){
		return PATRON_NOT_ENROLLED;
	}
	return patrons.at(patronid).number_books_checked_out;
}

/* search through patrons container to see if patronid is there
 * if so returns the name associated with patronid in the variable name
 * 
 * returns SUCCESS found it and name in name
 *         PATRON_NOT_ENROLLED no patron with this patronid
 */
int whatIsPatronName(std::string &name, int patronid) {
	bool found = false;
	for (int i = 0; i < signed(patrons.size()); i++) {
		if (patrons[i].patron_id == patronid) {
			found = true;
			break;
		}
	}
	if (found) {
		return SUCCESS;
	}
	return PATRON_NOT_ENROLLED;
}

