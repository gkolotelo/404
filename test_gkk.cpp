//
// tesk_gkk.cpp
//
#include <mach/mach_time.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>

#include "types.cpp"
#include "reading.cpp"

uint64_t        start_, end_, elapsed_;

using namespace std;




typedef enum{
    Left,
    Right
}Side;

typedef struct {
    Element *elem;
    int addr;
    Side side;
} MemoryElement;

class MemoryMap{
 private:
    list<MemoryElement> memoryList;
    list<MemoryElement>::iterator memoryIterator;
    int cursor = 0;

    string computeAddress(int addr);
    string generateLine(string addr, MemoryElement el1, MemoryElement el2);


 public:
    void add(Element *el);
    void printMemoryMap();
    bool isLast();
    //TODO: mais algum metodo?
};

void MemoryMap::add(Element* _elem){
    //Adiciona um MemoryElement na lista do MemoryMap
    MemoryElement me;
    me.elem = _elem;

    if((*_elem).type == Instruction){
        switch((*_elem).opcode){
            //Instrucoes sem endereco
            case Load_MQ: break;
            case Lsh: break;
            case Rsh: break;
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

    me.addr = _addr;
    me.side = _side;

    if(isLast()){
        //Insere no fim da lista
        memoryList.push_back(me);
        memoryIterator = memoryList.end();
    }
    else{
        //Sobrescreve os elementos da lista
        memoryIterator = memoryList.erase(memoryIterator);
        memoryIterator = memoryList.insert(memoryIterator, me);
    }
}

void MemoryMap::printMemoryMap(fstream fs) {
    MemoryElement e1, e2;
    string line;
    fs.open();
    for (memoryIterator = memoryList.begin(); memoryIterator != memoryList.end(); memoryIterator++)
    {
        e1 = *memoryIterator++;
        e2 = *memoryIterator++;  // Don't need to check if it's past end, because elements must always come in pairs

        line = generateLine(computeAddress(e1.addr), e1, e2);

        fs << line << '\n';
    }
    fs.close();
}

bool MemoryMap::isLast(){
    if((memoryIterator != memoryList.end()) && (next(memoryIterator) == memoryList.end()))
        return true;
    else 
        return false;
}



int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("%s", "File path needed\n");
        return 1;
    }

    fstream fs;
    string tempStr, tempStr2;
    // int pos;
    // TokenContainer TC;
    // char tempStrCA[100];
    // char *token1, *token2;

    start_ = mach_absolute_time();

    fs.open(argv[1], fstream::in);

    while (getline(fs, tempStr, '\n')) {
        // Strip comments out of input line
        tempStr = getLineNoCommentLowercase(tempStr);
        // Cria Element somente dos tokens correspondentes ao label, se existir
        getElement(tokenize(splitLabel(&tempStr)));
        // Cria Element de diretiva ou instrução
        getElement(tokenize(tempStr));
    
        // cout << tempStr << endl;
        // if (tempStr.length() != 0)
        // {
        //   pos = tempStr.find(':');
        //   //cout << tempStr << endl;
        //   if(pos != -1) // Linha possui label:
        //   {
        //       getElement(tokenize(tempStr.substr(0, pos + 1))); // Cria Element somente do label, se existir
        //       tempStr = tempStr.substr(pos + 1);
        //   }
        //   getElement(tokenize(tempStr)); // Cria Element de diretiva ou instrução
        // }
    }







    // DirectiveContentContainer CC;
    // string input = "-0x1010";
    // CC = contentParser(input, "");
    // cout << input << ' ' << CC.content1 << ' ' << CC.content2 << ' ' << CC.amount << endl;



    // //List Example:
    // // #include <list>
    // std::list<string> list_sample;
    // std::list<string>::iterator list_iterator;
    // for (int i = 0; i < 10; i++) {  // Adicionar 10 inteiros a lista
    //     list_sample.push_back( ("Number: " + to_string(i)) ); // Adiciona ao fim da lista, tem outras funcoes de insercao tbm
    //     cout << "Just inserted: '" << list_sample.back() << "' to list_sample" << endl;  // .back acessa ultimo elemento da lista sem remove-lo 
    // }
    // cout << "# of elements in list: " << list_sample.size() << endl;
    // 
    // list_iterator = list_sample.begin(); // Iterator no comeco da lista
    // advance(list_iterator, 5); // avanca iterator em 5 elementos
    // cout << *list_iterator << endl; // imprime elemento na posicao 6 do iterator ("Number: 5")
    // list_iterator = list_sample.erase(list_iterator); // Apaga elemento na posicao do iterator e retorna novo iterator: atencao vc deve pegar o iterator novo pq o antigo nao funciona mais, o elemento foi removido
    // list_iterator = list_sample.insert(list_iterator, "Hello World!!!"); // insere elemento na posicao do iterator, e retorna o iterator para o elemento que acabou de ser inserido 
    // cout << *list_iterator << endl; // imprime novo elemento na posicao do iterator
    // 
    // cout << "Printing list: \n" << endl;
    // for (list_iterator = list_sample.begin(); list_iterator != list_sample.end(); list_iterator++) {  // Iterar pelo numero de elementos
    //     string tmp = *list_iterator;  // pegar elementos do comeco da lista -> FIFO
    //     list_sample.pop_front();  // remover elemento
    //     cout << tmp << endl;  // imprimir elemento
    // }
    // cout << "# of elements in list: " << list_sample.size() << endl;


    end_ = mach_absolute_time();
    elapsed_ = end_ - start_;
    cout << elapsed_ << "µs" << endl;
}


