// +-------------+
// |Notas mentais|
// +-------------+
// A busca de label e efetuada na etapa de escrita.
// Caso nao a busca retorne algo diferente de NULL, este e o endereco desejado.
// Caso contrario, indica um erro, neste ponte de montagem, de referencia a um rotulo nao definido.



#include <iostream>
#include <fstream>
#include <sstream>
#include "types.cpp"
#include "reading.cpp"
#include <list>

using namespace std;

typedef enum{
	Left,
	Right
}Side;

typedef struct {
    Element* elem;
    int addr;
    Side side;
} MemoryElement;

class MemoryMap{
 private:
    list<MemoryElement> memoryList;
    list<MemoryElement>::iterator memoryIterator;
    int cursor;

 public:
    void add(Element el);
    void printMemoryMap();
    //TODO: mais algum metodo?
};

void MemoryMap::add(Element el){
    //TODO
}

void MemoryMap::printMemoryMap(){
    //TODO
}


MemoryMap* assembly(fstream fs){

    MemoryMap* memMap = new MemoryMap();
    string tempStr;
    Element* elem;
    
    //Para cada linha no arquivo
    //Comeca a montar o MemoryMap, elemento a elemento
    while(getline(fs, tempStr, '\n'))
    {
        tempStr = getLineNoCommentLowercase(tempStr); // Strip comments out of input line

        /* * * * * * * * * * * * *
         * Tratamento de labels  *
         * * * * * * * * * * * * */

        //Procura por label
        elem = getElement(tokenize(splitLabel(&tempStr)));
        
        if(elem != NULL){
            //Definicao de uma label
            //Insere no container de labels (labellink?)
            //TODO: chamar o INSERT de label
            //--> se ja existir, so altera o valor do endereco
            //--> se nao existir, cria e insere o valor
        }
        //else //Nao tem label


        /* * * * * * * * * * * * * * * * * * * *
         * Tratamento de diretiva ou instrucao *
         * * * * * * * * * * * * * * * * * * * */

        //Procura por diretiva ou instrucao
        elem = getElement(tokenize(tempStr));

        if((*elem).type == Instruction){
            //Instrucao
            switch((*elem).opcode){
                //Instrucoes sem endereco
                case Load_MQ:
                case Lsh:
                case Rsh:
                    memoryMap.push_back(MemoryMap(elem));
                    //TODO: controle de endereco e lado
                    break;

                //Instrucoes com endereco
                case Load_MQ_MX:
                case Stor_MX:
                case Load_MX:
                case Load_MX_neg:
                case Load_MX_abs:
                case Jump_M:
                case Jump_M_P:
                case Add_MX:
                case Add_MX_abs:
                case Sub_MX:
                case Sub_MX_abs:
                case Mul_MX:
                case Div_MX:
                case Stor_M:
                    //Resolve os labels
                    //TODO: chamar a BUSCA de label
                    //--> se existir: "linka"
                    //--> se nao existir: INSERT vazio
                    break;
            }
        }
        else{
            //Diretiva
            //TODO: switch-cases
            switch((*elem).DirectiveType){
                case Org:
                    //Mudar origem
                    break;
                case Align:
                    //Pular linha
                    break;
                case Wfill:
                    //Preencher varias linhas com a mesma palavra
                    break;
                case Word:
                    //Preencher uma linha com uma palavra
                    break;
                case Set:
                    //Parecido com label para uma word??
                    //TODO: verificar a semelhanca e possivel juncao
                    break;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("%s", "File path needed\n");
        return 1;
    }

    fstream fs;
    fs.open(argv[1], fstream::in);

    MemoryMap* memMap = assembly(fs);
    //writeMap(memMap); //<-- Not defined yet

}


/* * * * * * * * * *
 * Busca da label  *
 * * * * * * * * * */



findLabel(string _label){

}

int getLabelMapAddress(string _label){
    int addr;
    if (findLabel(_label)==NULL){
        //
    }
    else{

    }

}



