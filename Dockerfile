FROM gcc:latest

WORKDIR /sudoku

COPY . .

RUN make all 
CMD ./sudoku 9 < inputs/sudoku1.txt
