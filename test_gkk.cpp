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
    link_type labelLink
} MemoryElement;

class MemoryMap{
 private:
    list<MemoryElement> memoryList;
    list<MemoryElement>::iterator memoryIterator;
    int cursor = 0;  // left se par, right se impar

    string computeAddress(int addr);
    string generateLine(string addr, MemoryElement el1, MemoryElement el2);


 public:
    void add(Element *el);
    void printMemoryMap();
    bool isLast();
    //TODO: mais algum metodo?
};

void MemoryMap::add(Element* _elem) {
    //Adiciona um MemoryElement na lista do MemoryMap
    MemoryElement ME;
    ME.elem = _elem;

    if ((*_elem).type == Instruction) {
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
                ME.labelLink = addLabel(_elem->ICC);
                break;
        }
        ME.side = (cursor % 2)  // 0 se left (par), 1 se right (impar)
        ME.addr = (cursor >> 1);  // Seta endereço = cursor/2
        cursor++;
    }

    else if ((*_elem).type == Directive) {
        // TODO: switch-cases
        // Aparentemente diretivas nao causam um incremento em cursor
        switch((*_elem).DirectiveType){
            // Todos tem que funcionar como label, pois podemos ter algo como:
            // .set ORIGIN 0x00
            // .set STD_ALIGNMENT 1
            // .org ORIGIN
            // .org align STD_ALIGNMENT
            // etc...
            case Org:
                // Por enquanto vamos assumir que .set só se encontra no começo no arquivo,
                // portanto o "Label" correspondente de um set ja teria sido adicionado
                getNewCursor(_elem->DCC, &cursor);  // Se DCC é numerico, seta novo cursor, senao procura
                                                    // no mapa de labels por um set para setar novo cursor
                break;
            case Align:
                align(_elem->DCC, &cursor); // Se DCC é numerico, seta novo cursor, senao procura
                                            // no mapa de labels por um set para setar novo cursor
                break;
            case Wfill:
                for (int i = 0; i < convertValue(_elem->DCC.content1); i++) {
                    // Itera sobre o numero de elementos (content1) que Wfill deve criar e adiciona os Elements
                    add(new Element(1st_half_of_word));  // Tem que criar um novo contructor para suportar half_of_words
                    add(new Element(2nd_half_of_word));  // Tem que criar funcao p/ quebrar DCC.content2 em 1st e 2nd half_of_words
                }
                break;
            case Word:
                // quebrar _elem->DCC.content1 em 1st e 2nd half_of_words
                add(new Element(1st_half_of_word));
                add(new Element(2nd_half_of_word));
                break;
            case Set:
                //Parecido com label para uma word??
                //TODO: verificar a semelhanca e possivel juncao
                // O content de labels possui ":" portanto nao há problema de haver o mesmo nome com sets e labels
                addLabel(_elem->DCC.content1);
                break;
        }
    }

    else if ((*_elem).type == Label) {
        addLabel(_elem->LCC);
    }

    //else if ((*_elem).type == Word) // Novo tipo de elemento que deve ser criado

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


