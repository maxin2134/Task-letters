#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cerrno>

/*
* Ponieważ Jaś uwielbia łamigłówki, zaczął zastanawiać się, ile co najmniej zamian
* sąsiednich liter trzeba wykonać, żeby przekształcić jego nazwisko w nazwisko Małgosi.
* Nie jest to łatwe zadanie dla kilkuletniego dziecka, dlatego Jaś poprosił Ciebie, głównego
* programistę w przedszkolu, o napisanie programu, który znajdzie odpowiedź na nurtujące go pytanie.
*/

/*
* Program posiada konsolowe menu, w którym umieszczone zostały m.in.:
* 1. Ogólny opis
* 2. Trzy opcje do wyboru:
*	a) Wczytanie danych z pliku .txt;
*	b) Wprowadzenie danych ręcznie przez użytkownika;
*	c) Wyjście z programu.
* 3. Komunikaty w trakcie działania programu;
* 4. Obsługę małych i dużych liter;
*
* UWAGA!
* Wczytywany plik musi posiadać dane w następującej kolejności:
* 1. Liczba liter w imionach;
* 2. Pierwsze słowo;
* 3. Drugie słowo.
*/

// Funkcja odpowiedzialna za łączenie ze sobą części wyrazów
long long merge(std::vector<unsigned>& nums, int l, int m, int r) {
	std::vector<int> tmp(r - l + 1); // tablica pomocnicza
	long long counter = 0; // minimalna liczba zamian
	int i = l; // lewa podtablica
	int j = m; // prawa podtablica
	int k = 0; // indeks tymczasowej tablicy

	while (i <= m - 1 && j <= r)
	{
		if (nums[i] <= nums[j])
		{
			tmp[k++] = nums[i++];
		}
		else
		{
			tmp[k++] = nums[j++];
			counter += (m - i);
		}
	}

	while (i <= m - 1)
	{
		tmp[k++] = nums[i++];
	}

	while (j <= r)
	{
		tmp[k++] = nums[j++];
	}

	for (i = 0; i < k; i++)
	{
		nums[l + i] = tmp[i];
	}

	return counter;
}

// Funkcja odpowiedzialna za podział tablicy na podtablicy.
// Funkcja rekurencyjna.
long long mergeSort(std::vector<unsigned>& nums, int l, int r)
{
	if (l >= r)
	{
		return 0;
	}

	int m = l + (r - l) / 2;
	long long counter = 0;

	counter += mergeSort(nums, l, m);
	counter += mergeSort(nums, m + 1, r);
	counter += merge(nums, l, m + 1, r);
	return counter;

}

// Funkcja odpowiedzialna za sprawdzanie czy w wybranym wyrazie znajduje się litera.
bool checkString(const std::string& humanChoice) {

	while (std::cin.fail() || humanChoice.find_first_not_of("0123456789")
		|| humanChoice.find_first_not_of('\n') != std::string::npos) {
		std::cout << "Type number!\n" << std::endl;

		if (humanChoice.find_first_not_of("0123456789") == std::string::npos) {
			std::cin.clear();
			std::cin.ignore(256, '\n');
		}
		return 0;
	}
	return 1;
}

// Funkcja odpowiedzialna za sprawdzenie wyboru użytkownika po zakończonym sortowaniu.
bool checkExit(std::string& choice) {
	bool exit = false;
	while (!exit)
	{
		if (choice == "Y" || choice == "y") {
			return 1;
		}
		else if (choice == "N" || choice == "n") {
			return 0;
		}
		else
		{
			std::cout << "Write correct answer!";
			std::cin >> choice;
		}
	}

}

// Funkcja odpowiedzialna na wyświetlenie powitalnej wiadomości razem z krótkim opisem.
void welcomeMessage() {
	std::cout << R"(This is the solution to one of the algorithm problem from the https://szkopul.edu.pl/ site called Litery.
There are three (3) parameters that have to be provided:
1. The number of letters in the following strings (2 <= n <= 1,000,000).
2. String #1 (without spaces).
3. String #2 (without spaces).

The algorithm counts the minimum number of adjacent letters swapps to transform first string into the second one.

For the recruitment process there have been implemented several improvements:
1. Comments in the source code.
2. Algorithm handles lower case letters.
3. Program handles errors gracefully with proper message.
4. There are descriptions at each stage of the program operation.
5. Program can read input data from files on the disk.


)";
}

// Funkcja odpowiedzialna za przygotowanie tablicy, która zostanie posortowana.
long long prepareVectors(std::string& wordOne, std::string& wordTwo, unsigned lettersNumber) {

	long long counter = 0;

	std::vector<std::vector<unsigned>> tab_u;
	tab_u.resize(52); // Liczba znaków w alfabecie
	for (int i = 0; i < lettersNumber; ++i)
	{
		unsigned wordOneLetter = static_cast<unsigned>(wordOne[i]);
		// Sprawdzenie z jaką literą mamy do czynienia i odpowiednia operacja arytmetyczna z nią związana.
		if (wordOneLetter >= 'A' && wordOneLetter <= 'Z')
		{
			wordOneLetter -= 'A';
		}
		else if (wordOneLetter >= 'a' && wordOneLetter <= 'z')
		{
			wordOneLetter = wordOneLetter - 'a' + 26;
		}

		tab_u[wordOneLetter].push_back(i);
	}

	std::vector<std::vector<unsigned>> tab_w;
	tab_w.resize(52);
	for (int i = 0; i < lettersNumber; ++i)
	{
		unsigned wordTwoLetter = static_cast<unsigned>(wordTwo[i]);

		if (wordTwoLetter >= 'A' && wordTwoLetter <= 'Z')
		{
			wordTwoLetter -= 'A';
		}
		else if (wordTwoLetter >= 'a' && wordTwoLetter <= 'z')
		{
			wordTwoLetter = wordTwoLetter - 'a' + 26;
		}

		tab_w[wordTwoLetter].push_back(i);
	}
	std::vector<unsigned> p;
	p.resize(lettersNumber);
	for (int ch = 'A'; ch <= 'z'; ++ch)
	{
		//Sprawdzenie czy wczytany znak jest alfanumeryczny (nie jest znakiem)
		if (!std::isalpha(ch)) continue;
		int final_char = ch;

		if (ch <= 'Z') final_char = ch - 'A';
		else final_char = ch - 'a' + 26;

		for (size_t i = 0; i < tab_u[final_char].size(); ++i)
		{
			p[tab_u[final_char][i]] = tab_w[final_char][i];
		}
	}
	// Uruchomienie sortowania przez scalanie
	return counter = mergeSort(p, 0, lettersNumber - 1);
}

// Funkcja odpowiedzialna za wczytanie wymaganych danych od użytkownika.
long long getDataFromUser() {
	std::string wordOne, wordTwo, number;
	unsigned lettersNumber; // liczba liter
	long long counter; // minimalna liczba przesunięć
	bool result = 0;

	std::cout << "How many letters names have? \n";
	std::cin >> number;
	result = checkString(number);

	// Sprawdzenie czy w podanej licznie nie znajdują się litery.
	if (!result) {
		return -2;
	}
	else {
		lettersNumber = std::stoi(number, nullptr, 10);
	}

	std::cout << "Write first name: \n";
	std::cin >> wordOne;
	std::cout << "Write second name: \n";
	std::cin >> wordTwo;

	// Sprawdzenie czy obydwa wyrazy są takiej samej długości.
	if (wordOne.length() != wordTwo.length())
	{
		return -1;
	}
	// Uruchomienie funkcji odpowiedzialnej za przygotowanie tablicy do posortowania na podstawie uzyskanych
	// danych od użytkownika.
	counter = prepareVectors(wordOne, wordTwo, lettersNumber);

	return counter;
}

// Funkcja odpowiedzialna za wczytanie wymaganych danych z pliku.
long long getDataFromFile(std::string& filePath) {

	std::ifstream file(filePath); //utworzenie zmiennej plikowej i powiązanie z plikiem na dysku
	long long counter; // minimalna liczba przesunięć
	unsigned lettersNumber; // liczba liter
	std::string wordOne, wordTwo;
	bool result;

	// Zczytanie danych
	file >> lettersNumber;
	file >> wordOne;
	file >> wordTwo;
	// Zamknięcie zmiennej plikowej
	file.close();

	// Sprawdzenie wielkości odczytanych wyrazów
	if (wordOne.length() != wordTwo.length())
	{
		return -1;
	}

	// Sprawdzenie czy w odczytanych wyrazach nie znajdują sie liczby
	if (checkString(wordOne) || checkString(wordTwo)) {
		return -2;
	}
	// Uruchomienie funkcji odpowiedzialnej za przygotowanie tablicy do posortowania na podstawie uzyskanych
	// danych od użytkownika.
	counter = prepareVectors(wordOne, wordTwo, lettersNumber);
	return counter;
}

// Funkcja odpowiedzialna za wyświetlenie menu wyboru.
void showMainMenu() {
	std::cout << "Select one option: \n";
	std::cout << "1. Load data from file \n";
	std::cout << "2. Write data manually \n";
	std::cout << "3. Exit \n";
	std::cout << "Selection: ";
}


int main()
{
	std::ios_base::sync_with_stdio(false);
	std::string choice, restart, filePath;
	bool isDigit;
	long long counter = 0; // minimalna liczba przesunięć

	welcomeMessage();
	showMainMenu();
	std::getline(std::cin, choice);
	isDigit = checkString(choice);

	//Pętla nieskończona służąca za poruszanie się po konsolowym interfejsie użytkownika
	while (true) {

		if (isDigit) {
			// Rzutowanie(konwersja) string na int
			int rightChoice = std::stoi(choice);

			// W zależności od wartości rightChoice wybierany jest odpowiedni warunek
			// 1 - Wczytanie danych z pliku
			// 2 - Wczytanie danych od użytkownika
			// 3 - Wyjście z programu
			// Pozostałe - ponowne wyświetlenie opcji wyboru

			if (rightChoice == 1) {

				counter = 0;
				std::cout << "Write or paste file path. \n";
				std::cin >> filePath;
				counter = getDataFromFile(filePath);

				// W zależności od wartości counter wybierany jest odpowiedni warunek
				// 1. -1 - wyrazy nie mają identycznej długości
				// 2. -2 - wyrazy posiadają liczby
				// 3. wszystkie inne - wypisanie na standardowe wyjście minimalnej liczby zamian i zapytanie
				// użytkownika czy chce kontynuować

				if (counter == -1) {
					std::cout << "Words aren't equal! Try again. \n";
				}

				else if (counter == -2) {
					std::cout << "Words have digits! \n";
				}

				else if (counter != -1) {
					std::cout << "Minimum letters swap in this two words is: " << counter << "\n";
					std::cout << "Do You wan't restart? Y/N \n";
					std::cin >> restart;
					if (checkExit(restart)) {
						std::cin.ignore(256, '\n');
						showMainMenu();
						std::getline(std::cin, choice);
						isDigit = checkString(choice);
					}
					else {
						std::cout << "Closing program...";
						return 0;
					}
				}
			}

			// Wczytanie danych od użytkownika
			else if (rightChoice == 2) {
				counter = 0;
				counter = getDataFromUser();

				// W zależności od wartości counter wybierany jest odpowiedni warunek
				// -1 - wyrazy nie mają identycznej długości
				// -2 - wyrazy posiadają liczby
				// wszystkie inne - wypisanie na standardowe wyjście minimalnej liczby zamian i zapytanie
				// użytkownika czy chce kontynuować
				if (counter == -1) {
					std::cout << "Words aren't equal! Try again. \n";
				}
				else if (counter == -2) {}
				else if (counter != -1) {
					std::cout << "Minimum letters swap in this two words is: " << counter << "\n";
					std::cout << "Do You wan't restart? Y/N \n";
					std::cin >> restart;
					if (checkExit(restart)) {
						std::cin.ignore(256, '\n');
						showMainMenu();
						std::getline(std::cin, choice);
						isDigit = checkString(choice);
					}
					else {
						std::cout << "Closing program...";
						return 0;
					}
				}
			}

			// Wyjście z programu
			else if (rightChoice == 3) {
				std::cout << "Closing program...";
				return 0;
			}

			// Wyświetlenie komunikatu o wybraniu złej opcji
			else
			{
				std::cout << "Type right number!\n";
				showMainMenu();
				std::getline(std::cin, choice);
				isDigit = checkString(choice);
			}

		}
		else {
			showMainMenu();
			std::getline(std::cin, choice);
			isDigit = checkString(choice);
		}
	}
}