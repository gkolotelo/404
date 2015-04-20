
#include <iostream>
#include <fstream>
#include <sstream>
#include "types.cpp"
#include "reading.cpp"
#include <list>

using namespace std;

//Main para testes
//int main(){
//	Element lab("ThisIsALabel");
//	Element op(Sub_MX, "ThisIsAOperation");
//	Element dir(Wfill,"ThisIsADirective");
//	
//	cout 	<< "Label: " << lab.content << " | tipo: " << lab.type << endl 
//			<< "Operacao: " << op.content << " | tipo: " << op.type << " | " << op.opcode << endl
//			<< "Diretiva: " << dir.content << " | tipo: " << dir.type << " | " << dir.dir << endl;
//}

typedef enum{
	Left,
	Right
}Side;

class MemoryMap{
    Element* elem;
    int addr;
    Side side;

};



MemoryMap::MemoryMap(Element* _e){
    elem = _e;
    addr = 0;
    //Outras possiveis propriedades
}


int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("%s", "File path needed\n");
        return 1;
    }

    fstream fs;
    string tempStr, tempStr2;

    fs.open(argv[1], fstream::in);

    std::list<MemoryMap> memoryMap;
    std::list<MemoryMap>::iterator mIterator;
    Element* elem;

    //assembly();
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
            //TODO: chamar a insercao de label
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
                    //TODO: chamar o "resolvedor de label"
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
