Poniżej lista plików w kolejności, w jakiej powinny być uruchamiane
Ścieżki dostępu zostały umieszczone w kodzie - trzeba je zmodyfikować przed uruchomieniem
Kod pisany w pythonie 2.7. Interpreter w wersji 64bit
1. Przygotowanie danych
sqlCreateDatabase.py
	wynik: baza sql, w której umieszczone zostaną dane z plików xml Clinical Trials
sqlPopulateDatabase.py
	wymagane:baza sql przygotowana przez sqlCreateDatabase.py
	wymagane:pliki xml z Clinical trials
	wynik:baza sql, zawierająca dane z Clinical trials
sqlCreateTableInexclude.py
	wymagane:baza sql Clinical trials
	wynik:utworzona tabela na rozdzielone kryteria włączenia i wyłączenia
sqlCreateInExclude.py
	wymagane:baza z tabelą na rozdzielone kryteria włączenia i wyłączenia
	res:kryteria włączenia i wyłączenia umieszczone w osobnych polach

2. Obliczenie word2vec
2018_w2vCalculateClinTrial.py
	wymagane:baza sql zawierająca dane z Clinical Trials
	wynik:plik z wektorami word2vec

3. Przygotowanie zapytań
2018_query_build.py
	wymagane:gene_synonyms_case.txt
	wymagane:topics2018.xml
	wynik:qDiseases.txt ==> termy z chorobami
	wynik:qGenes.txt ==> termy z nazwami genów
	wynik:qVariants.txt ==> termy z wariantami genów

4. Serwer word2vec
2018_w2vServer.py
	wymagane:plik z wektorami word2vec
	wynik:serwer word2vec; musi być uruchomiony aby zadziałał skrypt 2018_w2v_qexp_loop.py; serwer przyjmuje zapytania (lista termów oryginalnych) i zwraca listę termów rozszerzonych

5. qeury expansion	
2018_w2v_qexp_loop.py
	wymagane:qDiseases.txt
	wymagane:qGenes.txt
	wymagane:qVariants.txt
	wymagane:uruchomiony 2018_w2vServer.py
	wynik:wygenerowane pliki z zapytaniami (można skonfigurować wagi dla termów oryginalnych, rozszerzonych, a także zmieniać pliki z wektorami word2vec)

#retrieval and evaluation	
cmdRetrieve_loop.py
	wymagane:wygenerowane pliki z zapytaniami
	wymagane:terrier (v 4.2)
	wynik:wyszukiwanie+ewaluacja; parametry P5-P1000 oraz Rprec zachowane w osobnych plikach (jeden z wartościami średnimi; drugi z wartościami dla każdego zapytania)

