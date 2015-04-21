// assembler.cpp
// 


#include <string>

using namespace std;

#ifndef types_cpp
    #include "types.cpp"
#endif
#define assembler_cpp

#define LABEL_NOT_DEFINED -1


typedef enum{
    Left,
    Right
} Side;

//typedef struct {
//    
//} LabelType;

// Temporary!
typedef string LabelType;


typedef struct{
    string name;
    int addr;
}AddressElement;

typedef struct {
    Element *elem;
    int addr;
    Side side;
    LabelType labelLink;
    AddressElement* addrLink;
} MemoryElement;

class AddressMap {
 private:
    list<AddressElement> addrList;
    list<AddressElement>::iterator addrIterator;
    //hashstring()
    //inserthash()
 public:
    AddressElement* setAddress(string _name, int _addr);
    int getAddress(string _name);
};

AddressElement* AddressMap::setAddress(string _name, int _addr){
    
    //Procura se a label ja existe
    for (addrIterator = addrList.begin(); addrIterator != addrList.end(); addrIterator++){
        if(addrIterator->name == _name){
            //Verifica se a label ainda nao foi definida
            if(addrIterator->addr == LABEL_NOT_DEFINED){
                addrIterator->addr = _addr;
                return addrIterator;
            }
            //else //<--TODO: ADD_ERROR: Definicao dupla de label
        }
    }

    //Se nao existe, insere:
    AddressElement* _e = new AddressElement();
    _e->name = _name;
    _e->addr = _addr;
    addrList.push_back(*_e);

    return _e;
    //inserthash(hashstring(_name))
}
//Se nao achar, adiciona e atribui LABEL_NOT_DEFINED como endereco
int AddressMap::getAddress(string _name){
    //Procura pela label
    for (addrIterator = addrList.begin(); addrIterator != addrList.end(); addrIterator++){
        if(addrIterator->name == _name)
            return addrIterator->addr; 
    }

    //Se nao achar, retorna LABEL_NOT_DEFINED como endereco
    return LABEL_NOT_DEFINED;
}



class MemoryMap {
 public:
    list<MemoryElement> memoryList;
    list<MemoryElement>::iterator memoryIterator = memoryList.begin();

    //LabelMap *labelMap = new LabelMap();
    //Para evitar conflito caso haja um nome de SET identico a um nome de LABEL, criam-se dois mapas separados
    AddressMap *labelMap = new AddressMap();
    AddressMap *nameMap = new AddressMap();

    int cursor = 0;  // left se par, right se impar

    string generateLine(string add, MemoryElement el1, MemoryElement el2);
    bool getNewCursor(DirectiveContentContainer DCC, int *cursor);
    void align(DirectiveContentContainer DCC, int *cursor);
    string getAddressHexStr(int addr);
    bool isLast();
    void splitWord(Element *word, Element *half_1, Element *half_2);

 public:
    void add(Element *el);

    void printMemoryMap(fstream outputFS);
    //TODO: mais algum metodo?
};

void MemoryMap::add(Element* _elem) {
    //Adiciona um MemoryElement na lista do MemoryMap
    if (_elem == NULL) return;
    //if (cursor > MAX_MEMORY_LINES * 2)  // ADD_ERROR maximo numero de addresses usados
    MemoryElement ME;
    ME.elem = _elem;
    ME.side = (Side)(cursor % 2);  // 0 se left (par), 1 se right (impar)
    ME.addr = (cursor >> 1);  // Seta endereço = cursor/2
    bool insert_into_map = FALSE;  // Change to FALSE

    if ((*_elem).GetElementType() == Instruction) {
        insert_into_map = TRUE;
        switch((*_elem).GetOpCodeType()){
            // Instrucoes sem endereco
            // Fully defined
            case Load_MQ: break;
            case Lsh: break;
            case Rsh: break;
            case Null: break;
            // Instrucoes com endereco
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
                // Retorna labelLink para uma label com nome em InstructionContentContainer, se a label existir,
                // somente retorna o labelLink, se nao, cria uma nova label nao inicializada e retorna labelLink
                // Atencao! Nao há ':' na label em ICC, tratar isso corretamente no LabelMap
                // Atencao! considerar caso em que content nao é label e sim valor hex
                //ME.labelLink = _elem->GetInstructionContentContainer();//labelMap->updateLabel(_elem->GetInstructionContentContainer());
                break;
        }
    }

    else if ((*_elem).GetElementType() == Directive) {
        // TODO: switch-cases
        // Aparentemente diretivas nao causam um incremento em cursor
        switch((*_elem).GetDirectiveType()){
            // Todos tem que funcionar como label, pois podemos ter algo como:
            // .set ORIGIN 0x00
            // .set STD_ALIGNMENT 1
            // .org ORIGIN
            // .org align STD_ALIGNMENT
            // etc...
            case Org:
                // Por enquanto vamos assumir que .set só se encontra no começo no arquivo,
                // portanto o "Label" correspondente de um set ja teria sido adicionado
                if (getNewCursor(_elem->GetDirectiveContentContainer(), &cursor) == TRUE) {  // Goes to an earlier address

                }
                // Se DCC é numerico, seta novo cursor, senao procura
                 // no mapa de labels por um set para setar novo cursor
                break;
            case Align:
                if (ME.side == Right)  // Devemos completar Right com zeros (Element())
                    add(new Element());
                align(_elem->GetDirectiveContentContainer(), &cursor);
                // Se DCC é numerico, seta novo cursor, senao procura no mapa de labels por um set para setar novo cursor
                break;
            case Wfill:
                //Element *half_1 = new Element();
                //Element *half_2 = new Element();
                //splitWord(_elem, half_1, half_2);
                for (int i = 0; i < convertValue(_elem->GetDirectiveContentContainer().content1); i++) {
                    // Itera sobre o numero de elementos (content1) que Wfill deve criar e adiciona os Elements
                    //add(new Element(half_1));  // Tem que criar um novo contructor para suportar half_of_words
                    //add(new Element(half_2));  // Tem que criar funcao p/ quebrar DCC.content2 em 1st e 2nd half_of_words
                }
                break;
            case Word:
                // quebrar _elem->DCC.content1 em 1st e 2nd half_of_words
                // Fazer funcao splitWord()
                //add(new Element(1st_half_of_word));
                //add(new Element(2nd_half_of_word));
                break;
            case Set:
                //Parecido com label para uma word??
                //TODO: verificar a semelhanca e possivel juncao
                // O content de labels possui ":" portanto nao há problema de haver o mesmo nome com sets e labels
                nameMap->setAddress(_elem->GetDirectiveContentContainer().content1, convertValue(_elem->GetDirectiveContentContainer().content2));
                break;
        }
    }
    
    else if ((*_elem).GetElementType() == Label) {
        // Adiciona nova label se esta ainda nao existe com address = cursor/2, se a label ja existe apenas insere o endereco
        labelMap->setAddress(_elem->GetLabelContentContainer(), (cursor/2));
    }
    
    else if ((*_elem).GetElementType() == Word) { // Novo tipo de elemento que deve ser criado
        insert_into_map = TRUE;
        
    }
    
    if (insert_into_map) {
        cout << "Cursor: " << cursor << endl;
        cursor++;
        memoryList.push_back(ME);
        //if(isLast()){
        //    //Insere no fim da lista
        //    memoryList.push_back(ME);
        //    memoryIterator = memoryList.end();
        //}
        //else{
        //	if(memoryIterator->addr == ME.addr){
	    //       //Sobrescreve o elemento da lista
	    //       memoryIterator = memoryList.erase(memoryIterator);
	    //   }
	    //   memoryIterator = memoryList.insert(memoryIterator, ME);
        //}
    }
}

void MemoryMap::printMemoryMap(fstream outputFS) {
    MemoryElement e1, e2;
    string line;
    //outputFS.open();
    for (memoryIterator = memoryList.begin(); memoryIterator != memoryList.end(); memoryIterator++)
    {
        e1 = *memoryIterator++;
        e2 = *memoryIterator++;  // Don't need to check if it's past end, because elements must always come in pairs

        line = generateLine(getAddressHexStr(e1.addr), e1, e2);

        outputFS << line << '\n';
    }
    outputFS.close();
}

bool MemoryMap::isLast(){
    if((memoryIterator != memoryList.end()) && (next(memoryIterator) == memoryList.end()))
        return true;
    else 
        return false;
}

string MemoryMap::getAddressHexStr(int addr) {

}

string MemoryMap::generateLine(string add, MemoryElement el1, MemoryElement el2) {

}

bool MemoryMap::getNewCursor(DirectiveContentContainer DCC, int *cursor) {
    int new_cursor = 2*convertValue(DCC.content1);  // DCC.content1 é o argumento de .align
    if (new_cursor < *cursor) {
        *cursor = new_cursor;
        return TRUE;
    }
    *cursor = new_cursor;
    return FALSE;
}

void MemoryMap::align(DirectiveContentContainer DCC, int *cursor) {
    int align_arg = convertValue(DCC.content1); 
    int correction = (align_arg - (*cursor % align_arg));
    *cursor = *cursor + correction;
}


void MemoryMap::splitWord(Element *word, Element *half_1, Element *half_2) {

}


















