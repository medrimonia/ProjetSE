BIN=report.pdf
DEPS=$(wildcard sections/*.tex)

.PHONY: all clean

all: $(BIN)

%.pdf: %.tex $(DEPS)
	pdflatex $< && pdflatex $<

clean:
	@ rm -f $(BIN) *~ *.log *.aux *.toc *.out
