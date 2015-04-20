// +-------------+
// |Notas mentais|
// +-------------+
// A busca de label e efetuada na etapa de escrita.
// Caso nao a busca retorne algo diferente de NULL, este e o endereco desejado.
// Caso contrario, indica um erro, neste ponte de montagem, de referencia a um rotulo nao definido.

#define LABEL_NOT_DEFINED -1

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
    bool isLast();
    void moveTo(int _addr);
    //TODO: mais algum metodo?
};

void MemoryMap::add(Element* _elem, int _addr, Side _side){
    //Adiciona um MemoryElement na lista do MemoryMap
    MemoryElement me;
    me.elem = _elem;
    me.addr = _addr;
    me.side = _side;

    if(isLast()){
        //Insere no fim da lista
        memoryList.push_back(me);
        memoryIterator = memoryList.end();
    }
    else{
        if(memoryList.elem.addr == me.addr){
            //Sobrescreve o elemento da lista
            memoryIterator = memoryList.erase(memoryIterator);
        }
        memoryIterator = memoryList.insert(memoryIterator, me);
    }
}

bool MemoryMap::isLast(){
    if((memoryIterator != memoryList.end()) && (next(memoryIterator) == memoryList.end()))
        return true;
    else 
        return false;
}

void moveTo(int _addr){

}

//void MemoryMap::printMemoryMap(){
//    //TODO
//}


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
            insertLabel();
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
                    //memoryMap.push_back(MemoryMap(elem));
                    memMap.add()
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

// Arvore digital com subcadeia eh suficiente para nomes diferentes entre si no comeco da cadeia
// Ha a possibilidade de usar HashTable tambem, da pra usar um desses espalhamentos: http://www.cse.yorku.ca/~oz/hash.html

typedef struct{
    string name;
    int addr;
}LabelElement;

class LabelMap {
 private:
    list<LabelElement> labelList;
    list<LabelElement>::iterator labelIterator;
    //hashstring()
    //inserthash()
 public:
    void setLabelAddress(string _labelname, int _addr);
    int getLabelAddress(string _labelname);
};

void setLabelAddress(string _labelname, int _addr){
    
    //Procura se a label ja existe
    for (labelIterator = labelList.begin(); labelIterator != labelList.end(); labelIterator++){
        if(labelIterator->name == _labelname){
            //Verifica se a label ainda nao foi definida
            if(labelIterator->addr == LABEL_NOT_DEFINED){
                labelIterator->addr = _addr;
            }
            //else //<--TODO: ADD_ERROR: Definicao dupla de label
        }
    }

    //Se nao existe, insere:
    LabelElement le;
    le.name = _labelname;
    le.addr = _addr;
    
    labelList.push_back(le);
    //inserthash(hashstring(_labelname))
}
//Se nao achar, adiciona e atribui LABEL_NOT_DEFINED como endereco
int getLabelAddress(string _labelname){
    //Procura pela label
    for (labelIterator = labelList.begin(); labelIterator != labelList.end(); labelIterator++){
        if(labelIterator->name == _labelname)
            return labelIterator->addr; 
    }

    //Se nao achar, retorna LABEL_NOT_DEFINED como endereco
    return LABEL_NOT_DEFINED;
}

unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */ //<--- 33 = magic number

    return hash;
}

