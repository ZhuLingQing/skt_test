#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sockfd, new_sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    char buffer[1024] = {0};

    // 创建套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Wait msg from P:%u\n", PORT);
    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 绑定套接字到地址
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听连接
    if (listen(sockfd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // 接受客户端连接
    new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addrlen);
    if (new_sockfd < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    // 读取客户端数据
    read(new_sockfd, buffer, 1024);
    printf("Message from client: %s\n", buffer);

    // 关闭套接字
    close(new_sockfd);
    close(sockfd);

    printf("Server closed.\n");
    return 0;
}
