#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

int main(int argc, char *argv[]) {

	if(argc == 3) {

		int socket_cliente, puerto, leido, escrito, tamano_buffer;

		struct sockaddr_in struct_servidor; // struct - store info about the server sobre el servidor.
		struct hostent *host_remoto; // struct - store info about remote host.

		tamano_buffer = 500;
		char buffer[tamano_buffer];

		puerto = 9999;

		// Create socket for the client.
		// AF_INET = Connections can be in different or in the same PC.
		// SOCK_STREAM = Communication oriented to connection.
		// 0 = protocol.
		socket_cliente = socket(AF_INET, SOCK_STREAM, 0);

		if(socket_cliente == -1) {
			perror("Error type: Create socket of the client.");
			exit(1);
		}

		// Obtain name of the server.
		// 127.0.0.1 = loopback direction, when client and server are in the same PC.
		// host_remoto = gethostbyname("127.0.0.1");
		host_remoto = gethostbyname(argv[1]); // IP address - Parameter

		if(host_remoto == NULL) {
			perror("Error type: host doesn't exist.");
			exit(1);
		}

		// Initialize struct to 0.
		bzero((char *)&struct_servidor, sizeof(struct_servidor));

		// Fill the struct of the servidor.
		// Connections can be in different or in the same PC.
		struct_servidor.sin_family = AF_INET;
		// "int puerto" to machine format.
		struct_servidor.sin_port = htons(puerto);
		// Copy information from hostent struct in the server.
		// h_addr: server address.
		bcopy((char *)host_remoto->h_addr, (char *)&struct_servidor.sin_addr.s_addr, host_remoto->h_length);

		// Connection between client and server
		if(connect(socket_cliente, (struct sockaddr *)&struct_servidor, sizeof(struct_servidor)) == -1) {
			perror("Error type: Connection between client and server.");
			exit(1);
		}

		// Write in the socket.
		escrito = write(socket_cliente, argv[2], strlen(argv[2]));

		if(escrito == -1) {
			perror("Error type: write the comand in the socket.");
			exit(1);
		}

		// Initialize buffer to 0.
		bzero(buffer, tamano_buffer);
		// CLient reads the answer of the server and stores it in the buffer.
		leido = read(socket_cliente, buffer, tamano_buffer);

		if(leido == -1) {
			perror("Error type: read answer from the socket.");
			exit(1);
		}

		printf("\n");
		printf("Server info: \n");
		printf("\n");
		printf("%s \n", buffer);
	
		// Close socket of the client.
		close(socket_cliente);
	}
	
	else {
		printf("\n");
		printf("Error, must be 3 parameters. \n");
		printf("Example: ./client [IP_server] [comand] \n");
		printf("\n");
	}
}
