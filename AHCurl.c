#include <stdio.h>
#include <stdlib.h>
#include "curl/include/curl/curl.h"

// Função de retorno de chamada para escrever os dados recebidos do site em um buffer
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

int main(void) {

    // Inicializa a aplicação, pedindo ao usuário o endereço do site para a realização da curl
    char endereco[100];
    printf("Digite o endereco do site para a curl: ");
    scanf("%s", endereco);

    // Inicializa a curlAPI
    CURL *curl_handle;
    CURLcode res;
	
	// Inicializa as variáveis para o nome do arquivo que será gerado com o resultado da curl
	char nome_arquivo[40];
	char c;
    time_t tempo_atual;  
    time(&tempo_atual); //Obtém o tempo atual  
    strftime(nome_arquivo, 40, "%d.%m.%Y-%Hh%Mm%Ss", localtime(&tempo_atual)); //Data e o horário como string   
    strcat(nome_arquivo, ".txt"); // Concatena com a extensão ".txt"

    // Inicializa o curl
    curl_global_init(CURL_GLOBAL_ALL);

    // Cria um novo identificador CURL
    curl_handle = curl_easy_init();

    if(curl_handle) {
        // Define a URL do site que será acessado a partir do endereco digitado pelo usuário
        curl_easy_setopt(curl_handle, CURLOPT_URL, endereco);

        // Define a função de retorno de chamada para escrever os dados recebidos em um buffer
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

        // Cria um arquivo para gravar e depois para ler o resultado do curl
        FILE *fwrite;
        fwrite = fopen(nome_arquivo, "w");
		FILE *fread;
		fread = fopen(nome_arquivo, "r");

        // Verifica se o arquivo foi aberto com sucesso
        if (fwrite == NULL) {
            fprintf(stderr, "Erro ao abrir o arquivo\n");
            return 1;
        }

        // Define o buffer onde os dados recebidos serão armazenados
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fwrite);

        // Desabilita a verificação do certificado SSL do site
        curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);

        // Executa a solicitação CURL e armazena o resultado em uma variável 'res'
        res = curl_easy_perform(curl_handle);

        // Verifica se a solicitação CURL foi bem-sucedida
        if(res != CURLE_OK) {
            fprintf(stderr, "Erro ao acessar o site: %s\n", curl_easy_strerror(res));
        }

        // Fecha o arquivo
        fclose(fwrite);

    	// Verifica se houve erro na abertura do arquivo onde o curl foi armazenado
		if (fread == NULL) {
			printf("Erro ao abrir o arquivo!\n");
			return 1;
		}

		// Lê o conteúdo do arquivo onde o curl foi armazenado e imprime no console
		while ((c = getc(fread)) != EOF) {
			putchar(c);
		}

		// Fecha o arquivo onde o curl foi armazenado
		fclose(fread);
		
        // Libera o identificador CURL
        curl_easy_cleanup(curl_handle);
    }

    // Finaliza o curl
    curl_global_cleanup();

    return 0;
}

/*
COMANDO PARA COMPILAR O PROGRAMA AHCurl
gcc AHCurl.c libcurl.dll curl/lib/libcurl.a -o AHCurl.exe
*/