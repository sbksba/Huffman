CC= gcc
CFLAGS= -W -Wall -ansi -pedantic
LDFLAGS= -L $(LIB) -lhuffman
INCFLAGS= -Iinclude
LIB=lib
SRC=src
OBJ=obj
BIN=bin
EXEC= HUFFMAN

all : directories $(EXEC)

#Creation bin , obj , lib
directories : ${OBJ} ${LIB} ${BIN}

${OBJ}:
	mkdir -p ${OBJ}
${BIN}:
	mkdir -p ${BIN}
${LIB}:
	mkdir -p ${LIB}

#Fichier Executable
$(EXEC) : $(OBJ)/main.o $(LIB)/libhuffman.a
	$(CC) $(CFLAGS) -o $(BIN)/$@ $^ $(LDFLAGS)

# Fichiers Objet
$(OBJ)/huffman.o : $(SRC)/huffman.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCFLAGS)

$(OBJ)/main.o : $(SRC)/main.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCFLAGS)

# Bibliotheques
$(LIB)/libhuffman.a : $(OBJ)/huffman.o
	ar -rs $@ $^

.PHONY : all proper clean cleanall runCom runDec bench

#Nettoyage
proper:
	rm -f *~ $(SRC)/*~ include/*~ test/*.huff test/*.decomp

clean : proper
	rm -f $(OBJ)/*.o $(BIN)/$(EXEC) $(LIB)/*

cleanall: clean
	rm -rf $(OBJ) $(BIN) $(LIB)

runCom:
	@echo ""
	@echo "        COMPRESSION          "
	@echo ""
	@echo "# Test Compression : test.txt"
	@./$(BIN)/$(EXEC) -c test/test.txt
	@echo ""
	@echo "# Test Compression : test.doc"
	@./$(BIN)/$(EXEC) -c test/test.doc
	@echo ""
	@echo "# Test Compression : test.csv"
	@./$(BIN)/$(EXEC) -c test/test.csv
	@echo ""
	@echo "# Test Compression : test.avi"
	@./$(BIN)/$(EXEC) -c test/test.avi
	@echo ""
	@echo "# Test Compression : test.mp4"
	@./$(BIN)/$(EXEC) -c test/test.mp4
	@echo ""

runDec:
	@echo ""
	@echo "        DECOMPRESSION          "
	@echo ""
	@echo "# Test Decompression : test.txt.huff => test.txt.decomp"
	@./$(BIN)/$(EXEC) -d test/test.txt.huff
	@echo "# Test Decompression : test.doc.huff => test.doc.decomp"
	@./$(BIN)/$(EXEC) -d test/test.doc.huff
	@echo "# Test Decompression : test.csv.huff => test.csv.decomp"
	@./$(BIN)/$(EXEC) -d test/test.csv.huff
	@echo "# Test Decompression : test.avi.huff => test.avi.decomp"
	@./$(BIN)/$(EXEC) -d test/test.avi.huff
	@echo "# Test Decompression : test.mp4.huff => test.mp4.decomp"
	@./$(BIN)/$(EXEC) -d test/test.mp4.huff
	@echo ""
	@ls -l test

bench: all runCom runDec cleanall
