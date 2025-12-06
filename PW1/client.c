#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>


void request_file(int sockfd, const char *filename);


int main(int argc, char *argv[]) {
	int sockfd;
	struct sockaddr_in servinfo;
	char *host;
	int port;
	char *filename;

	if (argc < 4) {
		fprintf(stderr, "Usage: %s <host> <port> <filename>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	host = argv[1];
	port = atoi(argv[2]);
	filename = argv[3];

	if (port == 0) {
		perror("Bad Port Specified\n");
		exit(EXIT_FAILURE);
	}

	servinfo.sin_family = AF_INET;
	servinfo.sin_port = htons(port);

	if (inet_pton(AF_INET, host, &servinfo.sin_addr) <= 0) {
		perror("Invalid address\n");
		exit(EXIT_FAILURE);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Unable to create socket\n");
		exit(EXIT_FAILURE);
	}

	if (connect(sockfd, (struct sockaddr *)&servinfo, sizeof(servinfo)) < 0) {
		perror("Connection failed\n");
		exit(EXIT_FAILURE);
	}

	printf("Connected to %s:%d\n", host, port);

	request_file(sockfd, filename);

	close(sockfd);
	return 0;
}


void request_file(int sockfd, const char *filename) {
	char request[512];
	char buffer[4096];
	int n, total_bytes = 0;
	FILE *fp = NULL;
	char *clean_filename;
	int header_parsed = 0;

	if (filename[0] != '/') {
		snprintf(request, sizeof(request), "BFT 1.0 FILE /%s\n", filename);
		clean_filename = strdup(filename);
	} else {
		snprintf(request, sizeof(request), "BFT 1.0 FILE %s\n", filename);
		clean_filename = strdup(filename + 1);
	}

	printf("Requesting: %s", request);

	if (write(sockfd, request, strlen(request)) < 0) {
		perror("Failed to send request\n");
		free(clean_filename);
		return;
	}

	while ((n = read(sockfd, buffer, sizeof(buffer))) > 0) {
		if (!header_parsed) {
			char *data_start = NULL;
			char response_header[512];
			int header_len = 0;

			for (int i = 0; i < n; i++) {
				if (i >= 3 && 
				    buffer[i-3] == 'F' && buffer[i-2] == 'I' && 
				    buffer[i-1] == 'L' && buffer[i] == 'E') {
					int j = i + 1;
					while (j < n && buffer[j] == ' ') j++;
					while (j < n && buffer[j] != ' ') j++;
					if (j < n && buffer[j] == ' ') {
						j++;
						data_start = buffer + j;
						header_len = j;
						break;
					}
				}
			}

			if (data_start == NULL) {
				buffer[n] = '\0';
				if (strstr(buffer, "404 No such file") != NULL) {
					printf("Error: %s", buffer);
					free(clean_filename);
					return;
				}
				printf("Error: Invalid response from server\n");
				free(clean_filename);
				return;
			}

			strncpy(response_header, buffer, header_len);
			response_header[header_len] = '\0';

			if (strstr(response_header, "404") != NULL) {
				printf("Error: File not found\n");
				free(clean_filename);
				return;
			}

			printf("Receiving file: %s\n", clean_filename);

			fp = fopen(clean_filename, "wb");
			if (fp == NULL) {
				perror("Failed to create output file\n");
				free(clean_filename);
				return;
			}

			int data_len = n - header_len;
			if (data_len > 0) {
				fwrite(data_start, 1, data_len, fp);
				total_bytes += data_len;
			}

			header_parsed = 1;
		} else {
			fwrite(buffer, 1, n, fp);
			total_bytes += n;
		}
	}

	if (fp != NULL) {
		fclose(fp);
		printf("File saved: %s (%d bytes)\n", clean_filename, total_bytes);
	}

	free(clean_filename);
}