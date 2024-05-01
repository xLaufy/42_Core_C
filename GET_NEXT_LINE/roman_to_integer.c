Funkcja get_next_line :

	Funkcja get_next_line to główna funkcja odpowiedzialna za odczytywanie danych z deskryptora pliku,
	która wypełnia box zawartością aż do znalezienia co najmniej jednego znaku nowej linii ‘\n’.
	box to statyczna zmienna zadeklarowana wewnątrz funkcji get_next_line.

	Zachowanie box:

	box to trwały bufor do przechowywania częściowej zawartości odczytanej z deskryptora pliku za każdym razem i istnieje do końca programu.
	Stąd jest zadeklarowany jako statyczny.Zachowuje swoją wartość między kolejnymi wywołaniami funkcji get_next_line.

	/*------------------------------------------------------------------------ */
	/* Główna funkcja do pobierania następnej linii z deskryptora pliku. */
	/* Alex (get_next_line) elegancko odczytuje linie z                        */
	/* pliku wejściowego za pomocą bufora. Alex ma na celu znalezienie */
	/* i zwrócenie każdej linii po kolei, nie przeszkadzając */
	/* reszcie pliku i unikając jednoczesnego odczytywania całego pliku */
	/* Odkrywca również elegancko radzi sobie z przypadkami brzegowymi i błędami. */
	/* ----------------------------------------------------------------------- */

	char * get_next_line(int fd)
{
	static char *box;
	char *line;

	if (fd < 0 || read(fd, NULL, 0) < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!box)
		box = ft_calloc(1, sizeof(char));
	if (!ft_strchr(box, '\n'))
		box = read_from_file(box, fd);
	if (!box)
		return (free(box), NULL);
	line = extract_line(box);
	box = obtain_remaining(box);
	return (line);
}

Czytanie z pliku :

	box ułatwia efektywne buforowane odczytywanie z deskryptora pliku.Funkcja read_from_file dodaje dane do box podczas buforowanych odczytów.

	/* -------------------------------------------------------------------------- */
	/* Funkcja do odczytywania danych z pliku i dołączania ich do częściowej zawartości. */
	/* Odkrywca zanurza swój magiczny naparstek (bufor) w akwarium (plik) */
	/* i pobiera łyżkę wody (znaki z pliku). Odkrywca */
	/* kontynuuje czerpanie, aż natrafi na specjalną rybę (nową linię) lub */
	/* zbada cały naparstek (bufor). */
	/* -------------------------------------------------------------------------- */

	char * read_from_file(char *box, int fd)
{
	char *temp_buffer;
	int bytes_read;

	temp_buffer = ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!temp_buffer)
		return (NULL);
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, temp_buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(temp_buffer), NULL);
		temp_buffer[bytes_read] = '\0';
		box = append_buffer(box, temp_buffer);
		if (ft_strchr(box, '\n'))
			break ;
	}
	free(temp_buffer);
	return (box);
}

Dodawanie temp_buffer do box Dynamiczna modyfikacja wskaźnika : Nawigacja po magicznym basenie w celu dodania i zarządzania pamięcią”

Aby stworzyć nowy,
większy basen, przydaje się funkcja ft_strjoin.Łączy ona wody z kubka z basenem,
tworząc nowy, przestronny bufor.Ale czy najpierw powiększamy basen, zanim połączymy wodę ? Czy najpierw łączymy wody,
a potem wyrzucamy(w sensie logicznym) stary basen i napełniamy je nowym,
odpowiednio wymiarowanym basenem ? Coś na kształt problemu kury i jajka.W każdym przypadku istnieje duże prawdopodobieństwo,
że wody znikną magicznie i doprowadzą do niezdefiniowanych zachowań i poważnych problemów z zarządzaniem pamięcią.Prowadzi to do osobnego posta tutaj[TODO]

/* -------------------------------------------------------------------------- */
/* Funkcja do dołączania odczytanych danych bufora do częściowej zawartości (linii). */
/* Odkrywca (get_next_line) z gracją dodaje łyżkę wody (odczyt */
/* bufor) do obecnego kubka (linii) i zwraca zaktualizowaną linię do */
/* dalszego badania. */
/* -------------------------------------------------------------------------- */

char *append_buffer(char *box, char *read_buffer)
{
	char *temp;

	temp = ft_strjoin(box, read_buffer);
	free(box);
	return (temp);
}

Aby upewnić się, że żadna woda nie została zmarnowana,
	Alex zwolnił oryginalny basen(box),
	bezpiecznie przechowując jego dane w nowym buforze(temp).W ten sposób,
	połączona woda pozostała dostępna,
	a Alex kontynuował swoją podróż przez akwarium,
	badając każdą linię(rybę)
		.

	Funkcja append_buffer przyjmuje dwa argumenty : char *box i char *read_buffer
		.

Celem tej funkcji jest skonkatenowanie zawartości read_buffer na końcu box i zwrócenie zaktualizowanej
połączonej łańcucha.Aby to osiągnąć bez powodowania problemów z pamięcią,
funkcja radzi sobie z alokacją pamięci i przypisaniem wskaźników w inteligentny sposób.

Gdy funkcja append_buffer jest wywoływana,
otrzymuje adres pamięci box jako pierwszy argument i adres pamięci read_buffer jako drugi argument.Ponieważ są to wskaźniki,
reprezentują one adresy pamięci,
a nie rzeczywiste dane.

Aby przeprowadzić konkatenację,
funkcja musi utworzyć nowy bufor, który jest na tyle duży,
aby pomieścić połączone dane.Funkcja dynamicznie alokuje pamięć dla nowego box za pomocą funkcji ft_strjoin,
która przyjmuje stary box i read_buffer jako argumenty,
i łączy je razem w nowym buforze.Co istotne,
funkcja ft_strjoin zapewnia,
że dla nowego bufora jest alokowana wystarczająca ilość pamięci,
biorąc pod uwagę oba zestawy danych.

Po pomyślnym połączeniu danych w nowym box,
funkcja następnie przypisuje wskaźnik box do nowego adresu pamięci zawierającego połączone dane.Jest to inteligentne przypisanie,
które pozwala funkcji zaktualizować oryginalny box poza funkcją.

Przypisując box,
funkcja skutecznie aktualizuje oryginalny wskaźnik poza zakresem funkcji.Teraz oryginalny box wskazuje na nowo utworzony bufor,
który zawiera połączone dane z obu buforów(box i read_buffer).
To inteligentne przypisanie zapewnia, że wywołujący funkcję append_buffer otrzymuje zaktualizowany wskaźnik,
wskazujący na połączone dane, a wszelkie dalsze operacje lub użycie box poza funkcją odzwierciedlają zmiany dokonane w funkcji.
Podsumowując, funkcja append_buffer demonstruje koncepcję przekazywania wskaźników w celu modyfikacji wartości oryginalnego wskaźnika,
umożliwiając efektywną konkatenację danych bez powodowania wycieków pamięci czy utraty danych.Inteligentne zarządzanie pamięcią i przypisywanie wskaźników sprawiają,
że funkcja jest niezawodna i bezpieczna podczas pracy z dynamicznie alokowaną pamięcią w C.
