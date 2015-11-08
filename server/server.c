#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) {

	if(argc == 1) {

		int socket_server, socket_client, port, readed, max_clients, tam_buffer, lenght_client;

		tam_buffer = 500;
		char buffer[tam_buffer]; // Store the result of the executed comand.

		struct sockaddr_in struct_servidor; // struct - Store the info of the server.
		struct sockaddr_in struct_cliente; // struct - Store the info of the client.
	
		port = 9999;
		max_clients = 10; // Max number of clients at the same time.
		lenght_client = sizeof(struct_cliente);
	
		// Create socket for the server
		// AF_INET = Connections can be in different or in the same PC.
		// SOCK_STREAM = Communication oriented to connection.
		// 0 = protocol.
		socket_server = socket(AF_INET, SOCK_STREAM, 0);
	
		if(socket_server == -1) {
			perror("Error type: Create socket of the server.");
			exit(1);
		}

		// Initialize struct to 0.
		bzero((char *)&struct_servidor, sizeof(struct_servidor));

		// Fill the struct of the servidor.
		// Connections can be in different or in the same PC.
		struct_servidor.sin_family = AF_INET;
		// "int port" to machine format.
		struct_servidor.sin_port = htons(port);
		// Accept petitions of any address.
		struct_servidor.sin_addr.s_addr = INADDR_ANY;

		// Associate port with the socket of the server.
		if(bind(socket_server, (struct sockaddr*)&struct_servidor, sizeof(struct_servidor)) == -1) {
			perror("Error in the bind() call.");
			exit(1);
		}

		// Check if there are connections in the port.
		listen(socket_server, max_clients);

		do{
			// Check the first client in the queue.
			socket_client = accept(socket_server, (struct sockaddr*)&struct_cliente, &lenght_client);

			if(socket_client == -1) {
				perror("Error type: Accept the connection from the client.");
				exit(1);
			}

			// Initialize buffer to 0.
			bzero(buffer, tam_buffer);
			// Read the comand sended by the client and store it in buffer.
			readed = read(socket_client, buffer, tam_buffer);

			if (readed == -1) {
				perror("Error type: read the comand from the socket.");
				exit(1);
			}

			// Create a son for execute the read comand.
			int pid = fork();

			if(pid == 0){ // If the proccess is the son
				// Se cierra la salida por pantalla (1 = salida por pantalla).
				close(1);
				// Se añade el primer descriptor disponible al socket para que el programa escriba en el socket.
				/* Con close() se cierra un descriptor, por eso se sabe que dup() devolverá el descriptor
				 * de fichero que se acaba de cerrar.
				 */
				dup(socket_client);
				// Execute the read comand and store the result in the buffer.
				execlp((const char*)buffer, (const char*)buffer, NULL);
			}

		// If one client sends the exit comands, the server will close the connection.
		} while(strcmp(buffer, "exit") != 0);

		// When the loop ends
		printf("\n");
		printf("I am the server and I close the connection with the client. \n");
		printf("\n");

		close(socket_client);
		close(socket_server);
	}

	else {
		printf("\n"); 
		printf("Error, only 1 parameter. \n");
		printf("Example: ./server \n");
		printf("\n");
	}
}
