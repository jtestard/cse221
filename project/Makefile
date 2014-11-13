C = pdflatex
BIB=-bibtex
CFLAGS= 

all: main

biblio: main.aux 
	-bibtex $<
	pdflatex main.tex
	pdflatex main.tex

main: main.tex
	$(C) $<
	$(BIB) $@
	$(C) $<

clean:
	rm *.aux
	rm *.log
	rm *.pdf
	rm *.blg
	rm *.bbl
