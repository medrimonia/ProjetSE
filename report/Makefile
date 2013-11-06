BIN=report.pdf

.PHONY: all clean

all: $(BIN)

%.pdf: %.tex
	pdflatex $< && pdflatex $<

clean:
	@ rm -f $(BIN) *~ *.log *.aux *.toc *.out
