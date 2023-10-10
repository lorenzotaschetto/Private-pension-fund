#include <iostream>
#include <iomanip>
#include <list>

using namespace std;

struct Registros
{
    double valor_aporte;
    double indice_aporte;
    int ano_aporte;
};

// Protótipos Funções
void Aporte(std::list<Registros> &lista, double valor, double indice, int ano);

void Resgate(std::list<Registros> &lista, double valor, double indice, int ano);

void Remove_valor_principal(std::list<Registros> &lista, double &valor_principal, double indice_resgate,
                            int ano_resgate, int &controle, double &rendimento, double &rendimento_total, double &valor_imposto);

float Calculo_rendimento(std::list<Registros> &lista, double &valor_principal,
                         double indice_resgate, double &rendimento);

void Calculo_imposto_de_renda(double rendimento, int ano_resgate, int ano_base, double &valor_imposto,
                              int &controle, std::list<Registros> &lista);

void Looping_do_resgate(std::list<Registros> &lista, double &valor_principal, double indice_resgate,
                        int ano_resgate, int &controle, double &rendimento, double &rendimento_total, double &valor_imposto);

void Looping_do_resgate(std::list<Registros> &lista, double &valor_principal, double indice_resgate,
                        int ano_resgate, int &controle, double &rendimento, double &rendimento_total, double &valor_imposto)
{
    while (valor_principal > 0)
    {
        // ano_base é o ano do aporte
        int ano_base = lista.front().ano_aporte;

        rendimento_total += Calculo_rendimento(lista, valor_principal, indice_resgate, rendimento);

        // função para atualizar o valor na conta, descontando o valor principal
        Remove_valor_principal(lista, valor_principal, indice_resgate, ano_resgate, controle,
                               rendimento, rendimento_total, valor_imposto);
        if (controle != 0)
        {
            // função para calcular o imposto de renda
            Calculo_imposto_de_renda(rendimento, ano_resgate, ano_base, valor_imposto, controle, lista);
        }
    }
}

void Calculo_imposto_de_renda(double rendimento, int ano_resgate, int ano_base, double &valor_imposto,
                              int &controle, list<Registros> &lista)
{
    double aliquota = 0.0;

    if (ano_resgate - ano_base < 2)
    {
        aliquota = 0.35;
    }
    else if (ano_resgate - ano_base >= 2 && ano_resgate - ano_base < 4)
    {
        aliquota = 0.30;
    }
    else if (ano_resgate - ano_base >= 4 && ano_resgate - ano_base < 6)
    {
        aliquota = 0.25;
    }
    else if (ano_resgate - ano_base >= 6 && ano_resgate - ano_base < 8)
    {
        aliquota = 0.20;
    }
    else if (ano_resgate - ano_base >= 8 && ano_resgate - ano_base < 10)
    {
        aliquota = 0.15;
    }
    else if (ano_resgate - ano_base >= 10)
    {
        aliquota = 0.10;
    }
    // valor do imposto de renda
    valor_imposto += rendimento * aliquota;

    if (lista.empty() && controle == 1)
    {
        controle = 0;
    }
}

float Calculo_rendimento(std::list<Registros> &lista, double &valor_principal, double indice_resgate,
                         double &rendimento)
{
    // se o valor do aporte for maior que o valor principal, o rendimento é calculado com o valor principal
    if (lista.front().valor_aporte > valor_principal)
    {
        rendimento = (indice_resgate - lista.front().indice_aporte) * valor_principal;
    }
    // se o valor do aporte for menor que o valor principal, o rendimento é calculado com o valor do aporte
    else
    {
        rendimento = (indice_resgate - lista.front().indice_aporte) * lista.front().valor_aporte;
    }
    return rendimento;
}

void Remove_valor_principal(std::list<Registros> &lista, double &valor_principal, double indice_resgate,
                            int ano_resgate, int &controle, double &rendimento, double &rendimento_total, double &valor_imposto)
{
    // se o valor do aporte for maior que o valor principal, o valor principal é zerado
    if (lista.front().valor_aporte > valor_principal)
    {
        lista.front().valor_aporte -= valor_principal;
        valor_principal = 0;
    }
    // se o valor do aporte for menor que o valor principal, o valor principal é atualizado
    if (valor_principal > lista.front().valor_aporte)
    {
        valor_principal -= lista.front().valor_aporte;
        lista.pop_front();
    }
    // se o valor do aporte for igual ao valor principal, o valor principal é zerado e o aporte é removido
    if (valor_principal == lista.front().valor_aporte)
    {
        lista.pop_front();
        valor_principal = 0;
    }
}

void Aporte(std::list<Registros> &lista, double valor, double indice, int ano)
{
    Registros registro;
    registro.valor_aporte = valor / indice; // valor do aporte dividido pelo índice de aporte
    registro.indice_aporte = indice;
    registro.ano_aporte = ano;
    lista.push_back(registro);
}

void Resgate(std::list<Registros> &lista, double valor, double indice, int ano)
{
    // variável que controla quando o usuário digita -1
    int controle = -1;
    // rendimento é o acréscimo no aporte em razão da auferição de rendimentos
    double rendimento = 0;
    // rendimento total é a soma dos rendimentos
    double rendimento_total = 0;
    // imposto inicia como zero
    double valor_imposto = 0.0;

    double valor_total = 0;

    double valor_resgate = valor;
    double indice_resgate = indice;
    int ano_resgate = ano;
    do
    {

        // se o valor do resgate for -1, então o usuário quer resgatar todo o valor
        if (valor_resgate == -1 || controle == 1)
        {
            valor_resgate = lista.front().valor_aporte * indice_resgate;
            controle = 1;
        }
        valor_total += valor_resgate;
        // valor principal é o valor a ser deduzido do aporte
        double valor_principal = valor_resgate / indice_resgate;
        Looping_do_resgate(lista, valor_principal, indice_resgate, ano_resgate, controle, rendimento,
                           rendimento_total, valor_imposto);

    } while (controle == 1);
    cout << fixed << setprecision(2) << valor_total << " ";
    cout << fixed << setprecision(2) << valor_imposto << endl;
}

int main()
{
    list<Registros> lista;
    char tipo;
    double valor, indice;
    int ano;

    while (cin >> tipo >> valor >> indice >> ano)
    {
        // A = aporte, R = resgate
        if (tipo == 'A')
        {
            Aporte(lista, valor, indice, ano);
        }
        if (tipo == 'R')
        {
            Resgate(lista, valor, indice, ano);
        }
    }
    return 0;
}