// assembler.cpp
// 

#include <iomanip>
#include <sstream>
#include <string>
#include <list>

using namespace std;

#ifndef TYPES_H
    #include "types.h"
#endif
#ifndef ERRORS_H
    #include "errors.h"
#endif
#ifndef READING_H
    #include "reading.h"
#endif
#define ASSEMBLER_H

#define LABEL_NOT_DEFINED -1
#define MAX_MEMORY_LINES 2048

#define FALSE 0
#define TRUE 1


/* Classe para mapa de enderecos de labels e nomes definidos pela diretiva .set */
class AddressMap {
 public:
    std::list<AddressElement> addrList;
    std::list<AddressElement>::iterator addrIterator;
 public:
    AddressElement* setAddress(string _name, int _addr, Side _side);
    void set2();
    int getAddress(string _name);
    Side getSide(string _name);
    AddressMap() {
        addrIterator = addrList.begin();
    }
};

/*  Classe para o mapa de memoria.
    Organiza metadados sobre cada elemento (Element) lido da entrada 
    em elementos do mapa de memoria (MemoryElement).
*/
class MemoryMap {
 private:
    list<MemoryElement> memoryList;
    list<MemoryElement>::iterator memoryIterator;

    AddressMap *addrMap;

    int cursor;  //Par=esquerda, Impar=Direita
    
    // Gera a string de da palavra para o endereco indicado por addr
    // de acordo com o conteudo dos elementos de memoria el1 e el2.
    string generateLine(int addr, MemoryElement el1, MemoryElement el2);
    
    // Obtem novo cursor dada uma diretiva .org
    bool getNewCursor(DirectiveContentContainer DCC, int *cursor);
    
    // Obtem novo cursor dada uma diretiva .align
    void align(DirectiveContentContainer DCC, int *cursor);
    
    // Retorna o endereco decimal addr em hexadecimal com 5 digitos de largura fixa.
    string getWordHexStr(string addr);
    
    // Retorna o endereco decimal addr em hexadecimal com 3 digitos de largura fixa.
    string getInstContentHexStr(string addr);
    
    // Retorna verdadeiro se iterador aponta para a ultima posicao da lista.
    bool isLast();
    
    // Retorna verdadeiro se o primeiro digito for um algarismo.
    bool isDigit(string in);
    
    // Quebra uma .word de 40 bits em 2 de 20 bits para ser inserida em 2 elementos
    void splitWord(string word, Element *half_1, Element *half_2);
    
    // Retorna o OpCode do elemento de memoria ME.
    string GetOpCodeString(MemoryElement ME);
    
    // Retorna o endereço usado na instrução, seja ele 'hardcoded' ou atraves de um label ou .set
    string GetContentString(MemoryElement ME);
 public:
    // Construtor da classe MemoryMap
    MemoryMap();

    // Recebe um elemento, interpreta seu conteudo e efetua as
    // devidas atividades de armazenamento ou controle no 
    // mapa de memoria.
    void add(Element *el);
    
    // Completa o espaço da direita do mapa de memoria com zeros para preparo da impressão.
    void finishUp();
    
    // Percorre o a lista de MemoryElement, MemoryList, do mapa de memoria
    // MemoryMap, e imprime os enderecos e as palavras de acordo com os
    // conteudos armazenados.
    void printMemoryMap(stringstream *outputFS);
};
