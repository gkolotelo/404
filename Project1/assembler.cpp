#include "assembler.h"
using namespace std;



/* ------ Metodos de AddressMap ------ */

AddressElement* AddressMap::setAddress(string _name, int _addr, Side _side) {
    
    // Procura se a label ja existe
    for (addrIterator = addrList.begin(); addrIterator != addrList.end(); addrIterator++) {
        if (addrIterator->name == _name) {
            // Verifica se a label ainda nao foi definida
            if (addrIterator->addr == LABEL_NOT_DEFINED) {
                addrIterator->addr = _addr;
                addrIterator->side = _side;
                return &(*addrIterator);
            }
            // Instrucao tentando setar label como LABEL_NOT_DEFINED sendo que ela ja foi setada por um label
            else if (_addr == LABEL_NOT_DEFINED)
                return &(*addrIterator);
            else callError(Label_Set_Duplicate);
        }
    }

    // Se nao existe, insere:
    AddressElement *_e = new AddressElement();
    _e->name = _name;
    _e->addr = _addr;
    _e->side = _side;
    addrList.push_back(*_e);
    return _e;
}



int AddressMap::getAddress(string _name) {
    // Se nao achar, adiciona e atribui LABEL_NOT_DEFINED como endereco

    // Procura pela label
    for (addrIterator = addrList.begin(); addrIterator != addrList.end(); addrIterator++) {
        if (addrIterator->name == _name)
            return addrIterator->addr;
    }

    // Se nao achar, retorna LABEL_NOT_DEFINED como endereco
    return LABEL_NOT_DEFINED;
}

Side AddressMap::getSide(string _name) {
    // Se nao achar, adiciona e atribui LABEL_NOT_DEFINED como endereco

    // Procura pela label
    for (addrIterator = addrList.begin(); addrIterator != addrList.end(); addrIterator++) {
        if (addrIterator->name == _name)
            return addrIterator->side;
    }

    // Se nao achar, retorna LABEL_NOT_DEFINED como endereco
    return Left;
}

/* ------ Metodos de MemoryMap ------ */

// Construtor da classe MemoryMap
MemoryMap::MemoryMap(){
    addrMap = new AddressMap();
    memoryIterator = memoryList.begin();
    cursor = 0;
}

void MemoryMap::add(Element* _elem) {

    //Adiciona um MemoryElement na lista do MemoryMap
    if (_elem == NULL) return;
    if (cursor > MAX_MEMORY_LINES * 2)
        callError(Max_Memory_Lines);
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
            case Load_MQ: 	break;
            case Lshift: 	break;
            case Rshift: 	break;

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
                // Retorna addrLink para uma label com nome em InstructionContentContainer, se a label existir,
                // somente retorna o addrLink, se nao, cria uma nova label nao inicializada e retorna addrLink
                // Check if ICC contains a valid address (starts with a number!) if so, addrLink = NULL, and rewrite ICC stripping 'm()' from it and converting to hex, will use ICC
                // value on final print. If ICC has a label or setValue, we must setAddress and get addrLink, in this case, addrLink->addr will be used on final print
                if (isDigit((string)_elem->GetInstructionContentContainer())) {
                   _elem->SetInstructionContentContainer(to_string(convertValue(_elem->GetInstructionContentContainer())));
                }
                else {
                    ME.addrLink = addrMap->setAddress((string)_elem->GetInstructionContentContainer(), LABEL_NOT_DEFINED, Left);  // addr = -1 porque nao sabemos ainda, side nao importa
                }
                break;
        }
    }

    else if ((*_elem).GetElementType() == Directive) {
        // Diretivas nao causam um incremento em cursor
        switch((*_elem).GetDirectiveType()){
            case Org:
                if (memoryList.empty()){
                    getNewCursor(_elem->GetDirectiveContentContainer(), &cursor);
                    break;
                }
                if (getNewCursor(_elem->GetDirectiveContentContainer(), &cursor) == TRUE) {  // Goes to an earlier address
                    // Se cursor novo é menor que cursor atual
                    //cout << "ORG SMALLER!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                    memoryIterator = memoryList.begin();
                    if (memoryIterator->addr > cursor/2)
                        return;
                    if (memoryList.size() == 1)
                        return;
                    for (memoryIterator = memoryList.begin(); memoryIterator != memoryList.end(); memoryIterator++) {
                        if (memoryIterator->addr == cursor/2) return;
                        if (((memoryIterator->addr < cursor/2) && (cursor/2 < next(memoryIterator)->addr))) return;
                    }                    
                }
                else {
                    // Cursor novo e maior ou igual que o cursor atual
                    if (memoryList.size() == 1)
                        return;
                    for (memoryIterator = memoryList.begin(); memoryIterator != memoryList.end(); memoryIterator++) {
                        if (memoryIterator->addr == cursor/2) return;
                        if (next(memoryIterator) == memoryList.end()) {
                            memoryIterator = memoryList.end();
                            return;
                        }
                        if (((memoryIterator->addr < cursor/2) && (cursor/2 < next(memoryIterator)->addr))) return;
                    }
                }
                // Se DCC é numerico, seta novo cursor, senao procura
                 // no mapa de labels por um set para setar novo cursor
                break;
            case Align: {
                if (ME.side == Right)  // Devemos completar Right com zeros (Element())
                    add(new Element());
                align(_elem->GetDirectiveContentContainer(), &cursor);
                // Se DCC é numerico, seta novo cursor, senao procura no mapa de labels por um set para setar novo cursor
                break;
            }
            case Wfill: {
                Element *half_1 = new Element();
                Element *half_2 = new Element();
                splitWord(_elem->GetDirectiveContentContainer().content2, half_1, half_2);  // Content2 porque o valor a ser inserido é o segundo argumento
                for (int i = 0; i < convertValue(_elem->GetDirectiveContentContainer().content1); i++) {
                    // Itera sobre o numero de elementos (content1) que Wfill deve criar e adiciona os Elements
                    add(half_1);  // Tem que criar um novo contructor para suportar half_of_words
                    add(half_2);  // Tem que criar funcao p/ quebrar DCC.content2 em 1st e 2nd half_of_words
                }
                break;
            }
            case Word: {
                // quebrar _elem->DCC.content1 em 1st e 2nd half_of_words
                Element *half_1 = new Element();
                Element *half_2 = new Element();
                splitWord(_elem->GetDirectiveContentContainer().content1, half_1, half_2);
                add(half_1);
                add(half_2);
                break;
            }
            case Set:
                //Parecido com label para uma word??
                //TODO: verificar a semelhanca e possivel juncao
                // O content de labels possui ":" portanto nao há problema de haver o mesmo nome com sets e labels
                addrMap->setAddress(_elem->GetDirectiveContentContainer().content1, convertValue(_elem->GetDirectiveContentContainer().content2), Left);
                break;
        }
    }
    
    else if ((*_elem).GetElementType() == Label) {
        // Adiciona nova label se esta ainda nao existe com address = cursor/2, se a label ja existe apenas insere o endereco
        addrMap->setAddress(_elem->GetLabelContentContainer(), (cursor/2), (Side)(cursor % 2));
    }
    
    else if ((*_elem).GetElementType() == WordElement) {
        insert_into_map = TRUE;
    }
    
    if (insert_into_map) {
        //cout << "Cursor: " << cursor << endl << endl;
        cursor++;
        //memoryList.push_back(ME);
        if(isLast()){
            //Insere no fim da lista
            memoryList.push_back(ME);
            memoryIterator = memoryList.end();
        }
        else{
            if(memoryIterator->addr == ME.addr){
               //Sobrescreve o elemento da lista
               memoryIterator = memoryList.erase(memoryIterator);
           }
           memoryIterator = memoryList.insert(memoryIterator, ME);
           memoryIterator++;
        }
    }
}

void MemoryMap::printMemoryMap(stringstream *outputFS) {
    MemoryElement e1, e2;
    string line;
    //outputFS.open();
    for (memoryIterator = memoryList.begin();memoryIterator != memoryList.end(); memoryIterator++)
    {
        e1 = *memoryIterator;
        memoryIterator++;
        e2 = *memoryIterator;  // Don't need to check if it's past end, because elements must always come in pairs

        line = generateLine(e1.addr, e1, e2);

        (*outputFS) << line << '\n';
    }

}

bool MemoryMap::isLast() {
    if(memoryIterator == memoryList.end())
        return TRUE;
    return FALSE;
}

string MemoryMap::getWordHexStr(string addr) {
    stringstream str;
    str << setfill('0') << setw(5) << std::hex << convertValue(addr);
    return str.str();
}

string MemoryMap::getInstContentHexStr(string addr) {
    stringstream str;
    str << setfill('0') << setw(3) << std::hex << convertValue(addr);
    return str.str();
}

string MemoryMap::generateLine(int addr, MemoryElement ME1, MemoryElement ME2) {
    string ME1string, ME2string;
    if (ME1.elem->GetElementType() == Instruction) {
        ME1string = GetOpCodeString(ME1) + " " + getInstContentHexStr(GetContentString(ME1));
    }
    else
        ME1string = getWordHexStr(GetContentString(ME1));

    if (ME2.elem->GetElementType() == Instruction) {
        ME2string = GetOpCodeString(ME2) + " " + getInstContentHexStr(GetContentString(ME2));
    }
    else
        ME2string = getWordHexStr(GetContentString(ME2));
    
    string output = getInstContentHexStr(to_string(addr)) + " " + ME1string + " " + ME2string;
    
    return output;
}

bool MemoryMap::getNewCursor(DirectiveContentContainer DCC, int *cursor) {
    int new_cursor = 2*convertValue(DCC.content1);  // DCC.content1 é o argumento de .org ou .align
    if (new_cursor < *cursor) {
        *cursor = new_cursor;
        return TRUE;
    }
    *cursor = new_cursor;
    return FALSE;
}

void MemoryMap::align(DirectiveContentContainer DCC, int *cursor) {
    int align_arg = convertValue(DCC.content1); 
    if (align_arg == 1) return;
    int correction = (align_arg - (((*cursor)/2) % align_arg));
    *cursor = *cursor + 2*correction;
}


void MemoryMap::splitWord(string word, Element *half_1, Element *half_2) {
    uint64_t converted_value = convertValue(word);
    half_1->SetWordContentContainer(to_string(((converted_value&0xFFFFF00000) >> 20)));
    half_2->SetWordContentContainer(to_string((converted_value&0xFFFFF)));
}

bool MemoryMap::isDigit(string in) {
    if ('0' <= in.front() && in.front() <= '9')
        return TRUE;
    return FALSE;
}

void MemoryMap::finishUp() {
    if((Side)(cursor % 2) == Right) {
        // Tem um elmento a esquerda, mas nada a direita, preencher com zeros:
        add(new Element());
    }

}

string MemoryMap::GetOpCodeString(MemoryElement ME) {
    if (ME.elem->GetOpCodeType() == Stor_M || ME.elem->GetOpCodeType() == Jump_M_P || ME.elem->GetOpCodeType() == Jump_M)
        return ME.elem->GetOpCodeString(addrMap->getSide(ME.elem->GetWordContentContainer()));
    return ME.elem->GetOpCodeString(Left);
}

string MemoryMap::GetContentString(MemoryElement ME) {
    int addr;
    if (ME.elem->GetElementType() == Instruction) {
        if(isDigit((string)ME.elem->GetInstructionContentContainer()))
            return ME.elem->GetInstructionContentContainer();
        addr = (addrMap->getAddress(ME.elem->GetInstructionContentContainer()));
    }
    else {  // (ME.elem->GetElementType() == WordElement) 
        if(isDigit((string)ME.elem->GetWordContentContainer()))
            return ME.elem->GetWordContentContainer();
        addr = (addrMap->getAddress(ME.elem->GetWordContentContainer()));
    }
    if (addr == LABEL_NOT_DEFINED) {
        callError(Label_Set_Not_Defined);
    }
    return to_string(addr);
    
}














