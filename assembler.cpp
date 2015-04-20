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
    bool insert_into_map = FALSE;

    if ((*_elem).GetElementType() == Instruction) {
        insert_into_map = TRUE;
        switch((*_elem).GetOpCodeType()){
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
                // Retorna labelLink para uma label com nome em InstructionContentContainer, se a label existir,
                // somente retorna o labelLink, se nao, cria uma nova label nao inicializada e retorna labelLink
                // Atencao! Nao há ':' na label em ICC, tratar isso corretamente no LabelMap
                // Atencao! considerar caso em que content nao é label e sim valor hex
                ME.labelLink = updateLabel(_elem->GetInstructionContentContainer());
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
                getNewCursor(_elem->GetDirectiveContentContainer(), &cursor);  // Se DCC é numerico, seta novo cursor, senao procura
                                                    // no mapa de labels por um set para setar novo cursor
                break;
            case Align:
                align(_elem->GetDirectiveContentContainer(), &cursor); // Se DCC é numerico, seta novo cursor, senao procura
                                            // no mapa de labels por um set para setar novo cursor
                break;
            case Wfill:
                for (int i = 0; i < convertValue(_elem->GetDirectiveContentContainer().content1); i++) {
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
                updateLabel(_elem->GetDirectiveContentContainer().content1);
                break;
        }
    }

    else if ((*_elem).GetElementType()== Label) {
        // Adiciona nova label se esta ainda nao existe com address = cursor/2, se a label ja existe apenas insere o endereco
        updateLabel(_elem->GetLabelContentContainer(), (cursor/2));
    }

    else if ((*_elem).GetElementType() == Word) { // Novo tipo de elemento que deve ser criado
        insert_into_map = TRUE;
        
    }

    if (insert_into_map) {
        ME.side = (cursor % 2)  // 0 se left (par), 1 se right (impar)
        ME.addr = (cursor >> 1);  // Seta endereço = cursor/2
        cursor++;
        if(isLast()){
            //Insere no fim da lista
            memoryList.push_back(ME);
            memoryIterator = memoryList.end();
        }
        else{
            //Sobrescreve os elementos da lista
            memoryIterator = memoryList.erase(memoryIterator);
            memoryIterator = memoryList.insert(memoryIterator, ME);
        }
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