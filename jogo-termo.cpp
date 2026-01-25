#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <random>

using namespace std;

void f_ApresentaInterface(int parametro, int qtdTurnos, string palavraSecreta, int qtdLetrasSegredo)
{
    switch (parametro)
    {
    case 1:
        cout << "--------------------------------\n";
        cout << "| BOAS-VINDAS AO JOGO TERMO |\n";
        cout << "--------------------------------\n";
        cout << "Quantidade de Turnos: " << qtdTurnos << endl;
        cout << "Quantidade de Letras da Senha: " << qtdLetrasSegredo << "\n\n";
        break;

    case 2:
        cout << "Parabéns Jogador, você venceu!\n";
        cout << "Palavra Secreta: " << palavraSecreta << "\n\n";
        break;

    case 3:
        cout << "Que pena Jogador, você perdeu!\n";
        cout << "A Palavra Secreta era: " << palavraSecreta << "\n\n";
        break;

    default:
        cout << "--------------------------------\n";
        cout << "| BOAS-VINDAS AO JOGO DA SENHA |\n";
        cout << "--------------------------------\n";
        break;
    }
}

void f_LimpaTela()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int f_BuscaRequisitos(int parametro)
{
    int requisitoBuscado;

    f_ApresentaInterface(0, 0,"", 0);

    switch (parametro)
    {
    case 1:
        cout << "Selecao de dificuldade:\n";
        cout << "1 - Facil (10 turnos)\n";
        cout << "2 - Medio (6 turnos)\n";
        cout << "3 - Dificil (3 turnos)\n";
        cout << "Opcao: ";
        cin >> requisitoBuscado;

        switch (requisitoBuscado)
        {
        case 1: f_LimpaTela();return 10;
        case 2: f_LimpaTela();return 6;
        case 3: f_LimpaTela();return 3;
        default: f_LimpaTela();return 10;
        }
    case 2:
        cout << "Quantidade de letras da palavra secreta:\n";
        cout << "1 - Facil (4 letras)\n";
        cout << "2 - Medio (6 letras)\n";
        cout << "3 - Dificil (8 letras)\n";
        cout << "Opcao: ";
        cin >> requisitoBuscado;

        switch (requisitoBuscado)
        {
        case 1: f_LimpaTela();return 4;
        case 2: f_LimpaTela();return 6;
        case 3: f_LimpaTela();return 8;
        default: f_LimpaTela();return 6;
        }
    }
    return 0;
}

int f_FiltraDicionario(vector<string>& palavras, int qtdLetrasSegredo)
{
    ifstream dicionario("br_sem_acentos.txt");

    if (!dicionario.is_open())
    {
        cout << "Erro na leitura do arquivo!" << endl;
        return 0;
    }else{
        string palavra;
        while (getline(dicionario, palavra)){
            if ((int)palavra.size() == qtdLetrasSegredo){
                palavras.push_back(palavra);
            }
        }
        dicionario.close();
        return palavras.size();
    }
}

string f_GeraSegredo(vector<string>& lista, int qtdPalavrasValidas)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distrib(0, qtdPalavrasValidas -1);
    int idxSorteado = distrib(gen);
    string palavraSecreta = lista[idxSorteado];
    return palavraSecreta;
}

string f_ValidaPalpite(string palpite, int qtdLetrasSegredo, int qtdPalavrasValidas, vector<string>& palavrasValidas)
{
    if(palpite.empty()) return "Palpites nulos ou vazios sao invalidos";
    if(palpite.size() != qtdLetrasSegredo) return "Palpite invalido! Tamanho incorreto";

    for (size_t i = 0; i < palpite.size(); i++) {
        char letraPalpite = palpite[i];
        if(!isalpha(letraPalpite)) return "Palpite invalido! Digite apenas letras";
    }

    int inexistencia=0;
    for(int j=0; j < qtdPalavrasValidas; j++){
        if(palpite != palavrasValidas[j]) inexistencia++; 
    }
    if(inexistencia == qtdPalavrasValidas) return "Palpite invalido! Palavra inexistente";
    return "Palpite Valido";
}

string f_RetornaFeedback(string palpite, string palavraSecreta, int qtdLetras)
{
    string feedback(qtdLetras, '_');
    vector<bool> usado(qtdLetras, false);

    for(int i = 0; i < qtdLetras; i++){
        if(palpite[i] == palavraSecreta[i]){
            feedback[i] = 'o';
            usado[i] = true;
        }
    }
    for(int i = 0; i < qtdLetras; i++){
        if(feedback[i] == 'o') continue;

        for(int j = 0; j < qtdLetras; j++){
            if(!usado[j] && palpite[i] == palavraSecreta[j]){
                feedback[i] = 'x';
                usado[j] = true;
                break;
            }
        }
    }
    return feedback;
}


int main()
{
    bool vitoria = false;
    int qtdTurnos = f_BuscaRequisitos(1), qtdLetrasSegredo = f_BuscaRequisitos(2);
    
    vector<string> listaPalavrasValidas;
    int qtdPalavrasValidas = f_FiltraDicionario(listaPalavrasValidas, qtdLetrasSegredo);

    string palavraSecreta = f_GeraSegredo(listaPalavrasValidas,qtdPalavrasValidas);

    int turno = 1;
    string palpite = "";
    
    f_ApresentaInterface(1, qtdTurnos, palavraSecreta, qtdLetrasSegredo);

    do
    {
        cout<<"Turno:" << turno << endl;
        cin >> palpite;
        
        string statusPalpite = f_ValidaPalpite(palpite, qtdLetrasSegredo,qtdPalavrasValidas,listaPalavrasValidas);
        if(statusPalpite == "Palpite Valido"){
            turno++;
            string feedback = f_RetornaFeedback(palpite, palavraSecreta, qtdLetrasSegredo);
            cout << feedback << '\n' << endl;

            if(palpite == palavraSecreta){
                vitoria = true;
                f_ApresentaInterface(2,qtdTurnos,palavraSecreta,qtdLetrasSegredo);
                break;
            }
        }else{
           cout << statusPalpite << "\n" << endl;
        }

    } while (!vitoria && turno <= qtdTurnos);

    if(!vitoria){ f_ApresentaInterface(3, qtdTurnos, palavraSecreta, qtdLetrasSegredo);}

    return 0;
}
