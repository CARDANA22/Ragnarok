#include "Netlink.h"

#include <iostream>
#include <linux/netlink.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>

#define NETLINK_USER 31

struct sockaddr_nl src_addr, dest_addr;
struct nlmsghdr *nlh = NULL;
struct iovec iov;
int sock_fd;
struct msghdr msg;

int Netlink::SendCommand(struct command_data &cmdData) {

        // Create a new socket
    sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_USER);

    // Check if it was created successfully
    if (sock_fd < 0)
        return -1;

    // Clear the data in the netlink address
    memset(&src_addr, 0, sizeof(src_addr));
    // Set the sockaddr_nl's values
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid();

    // Bind the sockaddr_nl to the sock_fd
    (void)bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr));

    // Clear the data in the destination address
    memset(&dest_addr, 0, sizeof(dest_addr));
    // Make it connect to the kernel as a unicast netlink socket
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0; /* For Linux Kernel */
    dest_addr.nl_groups = 0; /* unicast */

    // Allocate space for the message header
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(sizeof(command_data)));
    // Clear the data at the returned void*
    memset(nlh, 0, NLMSG_SPACE(sizeof(command_data)));
    // Set the msg length, local process id and flags
    nlh->nlmsg_len = NLMSG_SPACE(sizeof(command_data));
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    // Copy the command_data to the nlh message
    memcpy(NLMSG_DATA(nlh), &cmdData, sizeof(cmdData));

    // Set the iov's variables to the message
    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;

    // Set the message
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    // Send the message to the kernel
    sendmsg(sock_fd, &msg, 0);

    /* Read message from kernel */
    (void)recvmsg(sock_fd, &msg, 0);

    struct command_data response;
    memcpy(&response, (char*)NLMSG_DATA(nlh), sizeof(response));

    close(sock_fd);

    return 0;
}